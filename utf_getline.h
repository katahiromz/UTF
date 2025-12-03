/* utf_getline.h */

#ifndef UTF_GETLINE_H_
#define UTF_GETLINE_H_

#pragma once

#include "utf.h"
#include <stdlib.h> /* malloc, realloc, free */
#include <stdio.h>  /* FILE, feof, fread, fseek, SEEK_CUR */
#include <string.h> /* memcpy */
#include <limits.h> /* SIZE_MAX */

#ifndef UTF_GETLINE_CHUNK_BYTES
#define UTF_GETLINE_CHUNK_BYTES 8192U
#endif

/* internal: ensure capacity for element_count elements (not bytes) */
static __inline int
utf_ensure_capacity(void **pbuf, UTF_SIZE_T *pcap, UTF_SIZE_T needed_elems, size_t elem_size)
{
	UTF_SIZE_T cap = *pcap;
	if (cap >= needed_elems)
		return 0;

	/* start reasonable */
	if (cap == 0)
		cap = 128;

	/* increase exponentially but avoid overflow */
	while (cap < needed_elems)
	{
		if (cap <= (SIZE_MAX / 2))
			cap *= 2;
		else
		{
			/* try to set to needed_elems, guard overflow */
			if (needed_elems > SIZE_MAX / elem_size)
				return -1;
			cap = needed_elems;
			break;
		}
	}

	if (*pbuf == NULL)
	{
		void *p = malloc(cap * elem_size);
		if (!p) return -1;
		*pbuf = p;
	}
	else
	{
		void *p = realloc(*pbuf, cap * elem_size);
		if (!p) return -1;
		*pbuf = p;
	}
	*pcap = cap;
	return 0;
}

/* UTF-16 host-endian, chunked reader */
inline UTF_UC16 *
UTF16_getline(FILE *fp)
{
	if (!fp || feof(fp))
		return NULL;

	UTF_UC16 *result = NULL;
	UTF_SIZE_T cap = 0;
	UTF_SIZE_T len = 0;
	int got_any = 0;

	size_t chunk_elems = UTF_GETLINE_CHUNK_BYTES / sizeof(UTF_UC16);
	if (chunk_elems == 0) chunk_elems = 1;
	UTF_UC16 *tmp = (UTF_UC16 *)malloc(chunk_elems * sizeof(UTF_UC16));
	if (!tmp)
		return NULL;

	for (;;)
	{
		size_t read = fread(tmp, sizeof(UTF_UC16), chunk_elems, fp);
		if (read == 0)
			break;
		got_any = 1;

		/* look for newline in block */
		size_t i;
		for (i = 0; i < read; ++i)
		{
			if (tmp[i] == (UTF_UC16)'\n')
				break;
		}

		/* append up to i (or all read if no newline) */
		if (i > 0)
		{
			if (utf_ensure_capacity((void **)&result, &cap, len + i + 1, sizeof(UTF_UC16)) != 0)
			{
				free(tmp);
				free(result);
				return NULL;
			}
			memcpy(result + len, tmp, i * sizeof(UTF_UC16));
			len += i;
		}

		if (i < read) /* newline found at tmp[i] */
		{
			/* handle CRLF folding: check previous char (if any) */
			if (len > 0 && result[len - 1] == (UTF_UC16)'\r')
			{
				result[len - 1] = (UTF_UC16)'\n';
			}
			else
			{
				/* append newline */
				if (utf_ensure_capacity((void **)&result, &cap, len + 2, sizeof(UTF_UC16)) != 0)
				{
					free(tmp);
					free(result);
					return NULL;
				}
				result[len++] = (UTF_UC16)'\n';
			}

			/* push back unread elements (read - (i+1)) */
			long unread = (long)(read - (i + 1));
			if (unread > 0)
			{
				long diff = -unread * (long)sizeof(UTF_UC16);
				if (fseek(fp, diff, SEEK_CUR) != 0)
				{
					/* fseek failed; treat as EOF */
				}
			}

			/* terminate and return */
			if (utf_ensure_capacity((void **)&result, &cap, len + 1, sizeof(UTF_UC16)) != 0)
			{
				free(tmp);
				free(result);
				return NULL;
			}
			result[len] = 0;
			free(tmp);
			return result;
		}
		else /* no newline in this chunk: append all */
		{
			if (read > 0)
			{
				if (utf_ensure_capacity((void **)&result, &cap, len + read + 1, sizeof(UTF_UC16)) != 0)
				{
					free(tmp);
					free(result);
					return NULL;
				}
				memcpy(result + len, tmp, read * sizeof(UTF_UC16));
				len += read;
			}
			/* continue reading */
		}
	}

	/* EOF or error */
	free(tmp);
	if (!got_any)
	{
		free(result);
		return NULL;
	}
	/* terminate and return partial line */
	if (utf_ensure_capacity((void **)&result, &cap, len + 1, sizeof(UTF_UC16)) != 0)
	{
		free(result);
		return NULL;
	}
	result[len] = 0;
	return result;
}

/* UTF-16 file-endian (raw/byte-swapped) */
inline UTF_UC16 *
UTF16XE_getline(FILE *fp)
{
	if (!fp || feof(fp))
		return NULL;

	UTF_UC16 *rawbuf = NULL;
	UTF_SIZE_T cap = 0;
	UTF_SIZE_T len = 0;
	int got_any = 0;

	size_t chunk_elems = UTF_GETLINE_CHUNK_BYTES / sizeof(UTF_UC16);
	if (chunk_elems == 0) chunk_elems = 1;
	UTF_UC16 *tmp = (UTF_UC16 *)malloc(chunk_elems * sizeof(UTF_UC16));
	if (!tmp) return NULL;

	for (;;)
	{
		size_t read = fread(tmp, sizeof(UTF_UC16), chunk_elems, fp);
		if (read == 0)
			break;
		got_any = 1;

		size_t i;
		for (i = 0; i < read; ++i)
		{
			if (tmp[i] == UTF16_XE('\n'))
				break;
		}

		/* append up to i */
		if (i > 0)
		{
			if (utf_ensure_capacity((void **)&rawbuf, &cap, len + i + 1, sizeof(UTF_UC16)) != 0)
			{
				free(tmp);
				free(rawbuf);
				return NULL;
			}
			memcpy(rawbuf + len, tmp, i * sizeof(UTF_UC16));
			len += i;
		}

		if (i < read) /* newline found */
		{
			/* CRLF folding in raw form */
			if (len > 0 && rawbuf[len - 1] == UTF16_XE('\r'))
			{
				rawbuf[len - 1] = UTF16_XE('\n');
			}
			else
			{
				if (utf_ensure_capacity((void **)&rawbuf, &cap, len + 2, sizeof(UTF_UC16)) != 0)
				{
					free(tmp);
					free(rawbuf);
					return NULL;
				}
				rawbuf[len++] = UTF16_XE('\n');
			}

			/* push back unread raw elements */
			long unread = (long)(read - (i + 1));
			if (unread > 0)
			{
				long diff = -unread * (long)sizeof(UTF_UC16);
				if (fseek(fp, diff, SEEK_CUR) != 0)
				{
					/* ignore */
				}
			}

			/* terminate raw buffer */
			if (utf_ensure_capacity((void **)&rawbuf, &cap, len + 1, sizeof(UTF_UC16)) != 0)
			{
				free(tmp);
				free(rawbuf);
				return NULL;
			}
			rawbuf[len] = 0;

			/* convert in-place to host-endian */
			{
				UTF_SIZE_T j;
				for (j = 0; j < len; ++j)
					rawbuf[j] = UTF16_XE(rawbuf[j]);
			}

			free(tmp);
			return rawbuf;
		}
		else /* append all raw */
		{
			if (read > 0)
			{
				if (utf_ensure_capacity((void **)&rawbuf, &cap, len + read + 1, sizeof(UTF_UC16)) != 0)
				{
					free(tmp);
					free(rawbuf);
					return NULL;
				}
				memcpy(rawbuf + len, tmp, read * sizeof(UTF_UC16));
				len += read;
			}
		}
	}

	free(tmp);
	if (!got_any)
	{
		free(rawbuf);
		return NULL;
	}

	/* terminate raw and convert */
	if (utf_ensure_capacity((void **)&rawbuf, &cap, len + 1, sizeof(UTF_UC16)) != 0)
	{
		free(rawbuf);
		return NULL;
	}
	rawbuf[len] = 0;
	{
		UTF_SIZE_T j;
		for (j = 0; j < len; ++j)
			rawbuf[j] = UTF16_XE(rawbuf[j]);
	}
	return rawbuf;
}

/* UTF-32 host-endian, chunked reader */
inline UTF_UC32 *
UTF32_getline(FILE *fp)
{
	if (!fp || feof(fp))
		return NULL;

	UTF_UC32 *result = NULL;
	UTF_SIZE_T cap = 0;
	UTF_SIZE_T len = 0;
	int got_any = 0;

	size_t chunk_elems = UTF_GETLINE_CHUNK_BYTES / sizeof(UTF_UC32);
	if (chunk_elems == 0) chunk_elems = 1;
	UTF_UC32 *tmp = (UTF_UC32 *)malloc(chunk_elems * sizeof(UTF_UC32));
	if (!tmp) return NULL;

	for (;;)
	{
		size_t read = fread(tmp, sizeof(UTF_UC32), chunk_elems, fp);
		if (read == 0) break;
		got_any = 1;

		size_t i;
		for (i = 0; i < read; ++i)
		{
			if (tmp[i] == (UTF_UC32)'\n') break;
		}

		if (i > 0)
		{
			if (utf_ensure_capacity((void **)&result, &cap, len + i + 1, sizeof(UTF_UC32)) != 0)
			{
				free(tmp);
				free(result);
				return NULL;
			}
			memcpy(result + len, tmp, i * sizeof(UTF_UC32));
			len += i;
		}

		if (i < read) /* newline found */
		{
			if (len > 0 && result[len - 1] == (UTF_UC32)'\r')
				result[len - 1] = (UTF_UC32)'\n';
			else
			{
				if (utf_ensure_capacity((void **)&result, &cap, len + 2, sizeof(UTF_UC32)) != 0)
				{
					free(tmp);
					free(result);
					return NULL;
				}
				result[len++] = (UTF_UC32)'\n';
			}

			long unread = (long)(read - (i + 1));
			if (unread > 0)
			{
				long diff = -unread * (long)sizeof(UTF_UC32);
				if (fseek(fp, diff, SEEK_CUR) != 0) { }
			}

			if (utf_ensure_capacity((void **)&result, &cap, len + 1, sizeof(UTF_UC32)) != 0)
			{
				free(tmp);
				free(result);
				return NULL;
			}
			result[len] = 0;
			free(tmp);
			return result;
		}
		else /* append all */
		{
			if (read > 0)
			{
				if (utf_ensure_capacity((void **)&result, &cap, len + read + 1, sizeof(UTF_UC32)) != 0)
				{
					free(tmp);
					free(result);
					return NULL;
				}
				memcpy(result + len, tmp, read * sizeof(UTF_UC32));
				len += read;
			}
		}
	}

	free(tmp);
	if (!got_any)
	{
		free(result);
		return NULL;
	}
	if (utf_ensure_capacity((void **)&result, &cap, len + 1, sizeof(UTF_UC32)) != 0)
	{
		free(result);
		return NULL;
	}
	result[len] = 0;
	return result;
}

/* UTF-32 file-endian (raw/byte-swapped) */
inline UTF_UC32 *
UTF32XE_getline(FILE *fp)
{
	if (!fp || feof(fp))
		return NULL;

	UTF_UC32 *rawbuf = NULL;
	UTF_SIZE_T cap = 0;
	UTF_SIZE_T len = 0;
	int got_any = 0;

	size_t chunk_elems = UTF_GETLINE_CHUNK_BYTES / sizeof(UTF_UC32);
	if (chunk_elems == 0) chunk_elems = 1;
	UTF_UC32 *tmp = (UTF_UC32 *)malloc(chunk_elems * sizeof(UTF_UC32));
	if (!tmp) return NULL;

	for (;;)
	{
		size_t read = fread(tmp, sizeof(UTF_UC32), chunk_elems, fp);
		if (read == 0) break;
		got_any = 1;

		size_t i;
		for (i = 0; i < read; ++i)
		{
			if (tmp[i] == UTF32_XE('\n')) break;
		}

		if (i > 0)
		{
			if (utf_ensure_capacity((void **)&rawbuf, &cap, len + i + 1, sizeof(UTF_UC32)) != 0)
			{
				free(tmp);
				free(rawbuf);
				return NULL;
			}
			memcpy(rawbuf + len, tmp, i * sizeof(UTF_UC32));
			len += i;
		}

		if (i < read) /* newline */
		{
			if (len > 0 && rawbuf[len - 1] == UTF32_XE('\r'))
				rawbuf[len - 1] = UTF32_XE('\n');
			else
			{
				if (utf_ensure_capacity((void **)&rawbuf, &cap, len + 2, sizeof(UTF_UC32)) != 0)
				{
					free(tmp);
					free(rawbuf);
					return NULL;
				}
				rawbuf[len++] = UTF32_XE('\n');
			}

			long unread = (long)(read - (i + 1));
			if (unread > 0)
			{
				long diff = -unread * (long)sizeof(UTF_UC32);
				if (fseek(fp, diff, SEEK_CUR) != 0) { }
			}

			if (utf_ensure_capacity((void **)&rawbuf, &cap, len + 1, sizeof(UTF_UC32)) != 0)
			{
				free(tmp);
				free(rawbuf);
				return NULL;
			}
			rawbuf[len] = 0;

			/* convert to host-endian */
			{
				UTF_SIZE_T j;
				for (j = 0; j < len; ++j)
					rawbuf[j] = UTF32_XE(rawbuf[j]);
			}

			free(tmp);
			return rawbuf;
		}
		else /* append all */
		{
			if (read > 0)
			{
				if (utf_ensure_capacity((void **)&rawbuf, &cap, len + read + 1, sizeof(UTF_UC32)) != 0)
				{
					free(tmp);
					free(rawbuf);
					return NULL;
				}
				memcpy(rawbuf + len, tmp, read * sizeof(UTF_UC32));
				len += read;
			}
		}
	}

	free(tmp);
	if (!got_any)
	{
		free(rawbuf);
		return NULL;
	}

	if (utf_ensure_capacity((void **)&rawbuf, &cap, len + 1, sizeof(UTF_UC32)) != 0)
	{
		free(rawbuf);
		return NULL;
	}
	rawbuf[len] = 0;
	{
		UTF_SIZE_T j;
		for (j = 0; j < len; ++j)
			rawbuf[j] = UTF32_XE(rawbuf[j]);
	}
	return rawbuf;
}

#endif /* UTF_GETLINE_H_ */
