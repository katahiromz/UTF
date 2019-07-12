/* UTF --- UTF-8, UTF-16, UTF-32 conversion library
 * Copyright (C) 2019 Katayama Hirofumi MZ <katayama.hirofumi.mz@gmail.com>
 */
#ifndef UTF_H_
#define UTF_H_  28  /* Version 28 */

/* bool, true, false */
/* uint8_t, uint16_t, uint32_t */
#if defined(__cplusplus) && __cplusplus >= 201103L  /* C++11 */
    #include <cstdbool>
    #include <cstdint>
#elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L  /* C99 */
    #include <stdbool.h>
    #include <stdint.h>
#else
    #include "pstdbool.h"
    #include "pstdint.h"
#endif

#ifdef __cplusplus
    #include <cstdio>
    #include <cstring>
    #define UTF_STATIC_CAST(type, value) static_cast<type>(value)
    #define UTF_REINTERPRET_CAST(type, value) reinterpret_cast<type>(value)
#else
    #include <stdio.h>
    #include <string.h>
    #define UTF_STATIC_CAST(type, value) ((type)(value))
    #define UTF_REINTERPRET_CAST(type, value) ((type)(value))
#endif

/* UTF_C8, UTF_UC8, UTF_UC16, UTF_UC32 --- characters */
typedef char UTF_C8;
typedef uint8_t UTF_UC8;
#if defined(__cplusplus) && (__cplusplus >= 201103L || _MSC_VER >= 1900) /* C++11 */
    #ifdef UTF_WIDE_IS_UTF16
        typedef wchar_t UTF_UC16;
    #else
        typedef char16_t UTF_UC16;
    #endif
    typedef char32_t UTF_UC32;
#else
    #ifdef UTF_WIDE_IS_UTF16
        typedef wchar_t UTF_UC16;
    #else
        typedef uint16_t UTF_UC16;
    #endif
    typedef uint32_t UTF_UC32;
#endif

#ifndef UTF_DEFAULT_CHAR
    #define UTF_DEFAULT_CHAR '?'
    /* #define UTF_DEFAULT_CHAR ' ' */
    /* #define UTF_DEFAULT_CHAR 0 */
#endif

#ifndef UTF_u
    #ifdef UTF_WIDE_IS_UTF16
        #define UTF_u(str) L##str
    #else
        #define UTF_u(str) u##str
    #endif
#endif

#ifndef UTF_u8
    #define UTF_u8(str) u8##str
#endif

#ifndef UTF_L
    #define UTF_L(str) L##str
#endif

#ifndef UTF_U
    #define UTF_U(str) U##str
#endif

#ifndef UTF_SIZE_T
    /* #define UTF_SIZE_T int */
    #define UTF_SIZE_T size_t
#endif

typedef enum UTF_RET
{
    UTF_INVALID = 0,
    UTF_SUCCESS = 1,
    UTF_INSUFFICIENT_BUFFER = 2
} UTF_RET;

static __inline int
UTF_uc8_count(UTF_UC8 uc8)
{
    if (uc8 < 0x80) return 1;
    if (0xC2 <= uc8 && uc8 < 0xE0) return 2;
    if (0xE0 <= uc8 && uc8 < 0xF0) return 3;
    if (0xF0 <= uc8 && uc8 < 0xF8) return 4;
    return 0;
}

static __inline bool
UTF_uc8_is_trail(UTF_UC8 uc8)
{
    return 0x80 <= uc8 && uc8 < 0xC0;
}

static __inline bool
UTF_uc16_is_surrogate_high(UTF_UC16 uc16)
{
    return 0xD800 <= uc16 && uc16 < 0xDC00;
}

static __inline bool
UTF_uc16_is_surrogate_low(UTF_UC16 uc16)
{
    return 0xDC00 <= uc16 && uc16 < 0xE000;
}

static __inline UTF_SIZE_T
UTF_uj8_len(const UTF_UC8 *uj8)
{
    UTF_SIZE_T len;
    for (len = 0; *uj8; ++len, ++uj8)
    {
    }
    return len;
}

static __inline UTF_SIZE_T
UTF_j8_len(const UTF_C8 *j8)
{
    UTF_SIZE_T len;
    for (len = 0; *j8; ++len, ++j8)
    {
    }
    return len;
}

static __inline UTF_SIZE_T
UTF_uj16_len(const UTF_UC16 *uj16)
{
    UTF_SIZE_T len;
    for (len = 0; *uj16; ++len, ++uj16)
    {
    }
    return len;
}

static __inline UTF_SIZE_T
UTF_uj32_len(const UTF_UC32 *uj32)
{
    UTF_SIZE_T len;
    for (len = 0; *uj32; ++len, ++uj32)
    {
    }
    return len;
}

static __inline int
UTF_uj8_cmp(const UTF_UC8 *uj8a, const UTF_UC8 *uj8b)
{
    while (*uj8a && *uj8b)
    {
        if (*uj8a < *uj8b) return -1;
        if (*uj8a > *uj8b) return 1;
        ++uj8a;
        ++uj8b;
    }
    if (*uj8a < *uj8b) return -1;
    if (*uj8a > *uj8b) return 1;
    return 0;
}

static __inline int
UTF_j8_cmp(const UTF_C8 *j8a, const UTF_C8 *j8b)
{
    while (*j8a && *j8b)
    {
        if (*j8a < *j8b) return -1;
        if (*j8a > *j8b) return 1;
        ++j8a;
        ++j8b;
    }
    if (*j8a < *j8b) return -1;
    if (*j8a > *j8b) return 1;
    return 0;
}

static __inline int
UTF_uj16_cmp(const UTF_UC16 *uj16a, const UTF_UC16 *uj16b)
{
    while (*uj16a && *uj16b)
    {
        if (*uj16a < *uj16b) return -1;
        if (*uj16a > *uj16b) return 1;
        ++uj16a;
        ++uj16b;
    }
    if (*uj16a < *uj16b) return -1;
    if (*uj16a > *uj16b) return 1;
    return 0;
}

static __inline int
UTF_uj32_cmp(const UTF_UC32 *uj32a, const UTF_UC32 *uj32b)
{
    while (*uj32a && *uj32b)
    {
        if (*uj32a < *uj32b) return -1;
        if (*uj32a > *uj32b) return 1;
        ++uj32a;
        ++uj32b;
    }
    if (*uj32a < *uj32b) return -1;
    if (*uj32a > *uj32b) return 1;
    return 0;
}

static __inline int
UTF_uj8_cmpn(const UTF_UC8 *uj8a, const UTF_UC8 *uj8b, UTF_SIZE_T len)
{
    while (*uj8a && *uj8b)
    {
        if (len-- == 0) return 0;
        if (*uj8a < *uj8b) return -1;
        if (*uj8a > *uj8b) return 1;
        ++uj8a;
        ++uj8b;
    }
    if (*uj8a < *uj8b) return -1;
    if (*uj8a > *uj8b) return 1;
    return 0;
}

static __inline int
UTF_j8_cmpn(const UTF_C8 *j8a, const UTF_C8 *j8b, UTF_SIZE_T len)
{
    while (*j8a && *j8b)
    {
        if (len-- == 0) return 0;
        if (*j8a < *j8b) return -1;
        if (*j8a > *j8b) return 1;
        ++j8a;
        ++j8b;
    }
    if (*j8a < *j8b) return -1;
    if (*j8a > *j8b) return 1;
    return 0;
}

static __inline int
UTF_uj16_cmpn(const UTF_UC16 *uj16a, const UTF_UC16 *uj16b, UTF_SIZE_T len)
{
    while (*uj16a && *uj16b)
    {
        if (len-- == 0) return 0;
        if (*uj16a < *uj16b) return -1;
        if (*uj16a > *uj16b) return 1;
        ++uj16a;
        ++uj16b;
    }
    if (*uj16a < *uj16b) return -1;
    if (*uj16a > *uj16b) return 1;
    return 0;
}

static __inline int
UTF_uj32_cmpn(const UTF_UC32 *uj32a, const UTF_UC32 *uj32b, UTF_SIZE_T len)
{
    while (*uj32a && *uj32b)
    {
        if (len-- == 0) return 0;
        if (*uj32a < *uj32b) return -1;
        if (*uj32a > *uj32b) return 1;
        ++uj32a;
        ++uj32b;
    }
    if (*uj32a < *uj32b) return -1;
    if (*uj32a > *uj32b) return 1;
    return 0;
}

static __inline bool
UTF_uc32_to_uc8(UTF_UC32 uc32, UTF_UC8 uc8[4])
{
    if (uc32 > 0x10FFFF)
    {
        uc8[0] = 0;
        return false;
    }

    if (uc32 < 0x80)
    {
        uc8[0] = UTF_STATIC_CAST(UTF_UC8, uc32);
        uc8[1] = 0;
    }
    else if (uc32 < 0x800)
    {
        uc8[0] = 0xC0 | UTF_STATIC_CAST(UTF_UC8, uc32 >> 6);
        uc8[1] = 0x80 | (UTF_STATIC_CAST(UTF_UC8, uc32) & 0x3F);
        uc8[2] = 0;
    }
    else if (uc32 < 0x10000)
    {
        uc8[0] = 0xE0 | UTF_STATIC_CAST(UTF_UC8, uc32 >> 12);
        uc8[1] = 0x80 | (UTF_STATIC_CAST(UTF_UC8, uc32 >> 6) & 0x3F);
        uc8[2] = 0x80 | (UTF_STATIC_CAST(UTF_UC8, uc32) & 0x3F);
        uc8[3] = 0;
    }
    else
    {
        uc8[0] = 0xF0 | UTF_STATIC_CAST(UTF_UC8, uc32 >> 18);
        uc8[1] = 0x80 | (UTF_STATIC_CAST(UTF_UC8, uc32 >> 12) & 0x3F);
        uc8[2] = 0x80 | (UTF_STATIC_CAST(UTF_UC8, uc32 >> 6) & 0x3F);
        uc8[3] = 0x80 | (UTF_STATIC_CAST(UTF_UC8, uc32) & 0x3F);
    }

    return true;
}

static __inline bool
UTF_uc8_to_uc32(const UTF_UC8 uc8[4], UTF_UC32 *uc32)
{
    int count = UTF_uc8_count(uc8[0]);
    if (!count)
        return false;

    switch (count)
    {
    case 1:
        *uc32 = uc8[0];
        break;

    case 2:
        if (!UTF_uc8_is_trail(uc8[1]))
            return false;

        if ((uc8[0] & 0x1E) == 0)
            return false;

        *uc32 = (UTF_STATIC_CAST(UTF_UC32, uc8[0] & 0x1F) << 6);
        *uc32 |= UTF_STATIC_CAST(UTF_UC32, uc8[1] & 0x3F);
        break;

    case 3:
        if (!UTF_uc8_is_trail(uc8[1]) || !UTF_uc8_is_trail(uc8[2]))
            return false;

        if ((uc8[0] & 0x0F) == 0 && (uc8[1] & 0x20) == 0)
            return false;

        *uc32 = UTF_STATIC_CAST(UTF_UC32, uc8[0] & 0x0F) << 12;
        *uc32 |= UTF_STATIC_CAST(UTF_UC32, uc8[1] & 0x3F) << 6;
        *uc32 |= UTF_STATIC_CAST(UTF_UC32, uc8[2] & 0x3F);
        break;

    case 4:
        if (!UTF_uc8_is_trail(uc8[1]) ||
            !UTF_uc8_is_trail(uc8[2]) ||
            !UTF_uc8_is_trail(uc8[3]))
        {
            return false;
        }

        if ((uc8[0] & 0x07) == 0 && (uc8[1] & 0x30) == 0)
            return false;

        *uc32 = UTF_STATIC_CAST(UTF_UC32, uc8[0] & 0x07) << 18;
        *uc32 |= UTF_STATIC_CAST(UTF_UC32, uc8[1] & 0x3F) << 12;
        *uc32 |= UTF_STATIC_CAST(UTF_UC32, uc8[2] & 0x3F) << 6;
        *uc32 |= UTF_STATIC_CAST(UTF_UC32, uc8[3] & 0x3F);
        break;

    default:
        return false;
    }

    return true;
}

static __inline bool
UTF_uc32_to_uc16(UTF_UC32 uc32, UTF_UC16 uc16[2])
{
    if (uc32 > 0x10FFFF)
        return false;

    if (uc32 < 0x10000)
    {
        uc16[0] = UTF_STATIC_CAST(UTF_UC16, uc32);
        uc16[1] = 0;
    }
    else
    {
        uc16[0] = UTF_STATIC_CAST(UTF_UC16, (uc32 - 0x10000) / 0x400 + 0xD800);
        uc16[1] = UTF_STATIC_CAST(UTF_UC16, (uc32 - 0x10000) % 0x400 + 0xDC00);
    }

    return true;
}

static __inline bool
UTF_uc16_to_uc32(const UTF_UC16 uc16[4], UTF_UC32 *uc32)
{
    if (UTF_uc16_is_surrogate_high(uc16[0]))
    {
        if (UTF_uc16_is_surrogate_low(uc16[1]))
        {
            *uc32 = 0x10000 + (UTF_STATIC_CAST(UTF_UC32, uc16[0]) - 0xD800) * 0x400 +
                    (UTF_STATIC_CAST(UTF_UC32, uc16[1]) - 0xDC00);
            return true;
        }
        else if (uc16[1] == 0)
        {
            *uc32 = uc16[0];
            return true;
        }
        return false;
    }
    else if (UTF_uc16_is_surrogate_low(uc16[0]))
    {
        if (uc16[1] != 0)
            return false;

        *uc32 = uc16[0];
        return true;
    }
    else
    {
        *uc32 = uc16[0];
        return true;
    }
}

static __inline bool
UTF_uc8_to_uc16(const UTF_UC8 uc8[4], UTF_UC16 uc16[2])
{
    UTF_UC32 uc32;
    return UTF_uc8_to_uc32(uc8, &uc32) && UTF_uc32_to_uc16(uc32, uc16);
}

static __inline bool
UTF_uc16_to_uc8(const UTF_UC16 uc16[2], UTF_UC8 uc8[4])
{
    UTF_UC32 uc32;
    return UTF_uc16_to_uc32(uc16, &uc32) && UTF_uc32_to_uc8(uc32, uc8);
}

static __inline UTF_RET
UTF_uj8_to_uj16(const UTF_UC8 *uj8, UTF_SIZE_T uj8size, UTF_UC16 *uj16, UTF_SIZE_T uj16size)
{
    UTF_UC8 uc8[4];
    UTF_UC16 uc16[2];
    int i, count;
    const UTF_UC8 *uj8end = uj8 + uj8size;
    UTF_UC16 *uj16end = uj16 + uj16size - 1;

    if (!uj16size && uj8size)
        return UTF_INSUFFICIENT_BUFFER;

    for (; uj8 != uj8end; ++uj8)
    {
        count = UTF_uc8_count(*uj8);
        if (!count)
        {
            if (!UTF_DEFAULT_CHAR)
            {
                *uj16 = 0;
                return UTF_INVALID;
            }
            if (uj16 == uj16end)
            {
                *uj16 = 0;
                return UTF_INSUFFICIENT_BUFFER;
            }
            *uj16++ = UTF_STATIC_CAST(UTF_UC8, UTF_DEFAULT_CHAR);
            continue;
        }

        uc8[0] = *uj8;
        for (i = 1; i < count; ++i)
        {
            if (++uj8 == uj8end)
            {
                if (!UTF_DEFAULT_CHAR)
                {
                    *uj16 = 0;
                    return UTF_INVALID;
                }
                if (uj16 == uj16end)
                {
                    *uj16 = 0;
                    return UTF_INSUFFICIENT_BUFFER;
                }
                *uj16++ = UTF_STATIC_CAST(UTF_UC8, UTF_DEFAULT_CHAR);
                *uj16 = 0;
                return UTF_SUCCESS;
            }
            uc8[i] = *uj8;
        }

        if (!UTF_uc8_to_uc16(uc8, uc16))
        {
            if (!UTF_DEFAULT_CHAR)
            {
                *uj16 = 0;
                return UTF_INVALID;
            }
            uc16[0] = UTF_DEFAULT_CHAR;
            uc16[1] = 0;
        }

        if (uj16 == uj16end)
        {
            *uj16 = 0;
            return UTF_INSUFFICIENT_BUFFER;
        }

        *uj16++ = uc16[0];
        if (uc16[1])
        {
            if (uj16 == uj16end)
            {
                *uj16 = 0;
                return UTF_INSUFFICIENT_BUFFER;
            }
            *uj16++ = uc16[1];
        }
    }
    *uj16 = 0;
    return UTF_SUCCESS;
}

static __inline UTF_RET
UTF_j8_to_uj16(const UTF_C8 *j8, UTF_SIZE_T j8size, UTF_UC16 *uj16, UTF_SIZE_T uj16size)
{
    return UTF_uj8_to_uj16(UTF_REINTERPRET_CAST(const UTF_UC8 *, j8), j8size, uj16, uj16size);
}

static __inline UTF_RET
UTF_uj8_to_uj32(const UTF_UC8 *uj8, UTF_SIZE_T uj8size, UTF_UC32 *uj32, UTF_SIZE_T uj32size)
{
    UTF_UC8 uc8[4];
    UTF_UC32 uc32;
    int i, count;
    const UTF_UC8 *uj8end = uj8 + uj8size;
    UTF_UC32 *uj32end = uj32 + uj32size - 1;

    if (!uj32size && uj8size)
        return UTF_INSUFFICIENT_BUFFER;

    for (; uj8 != uj8end; ++uj8)
    {
        count = UTF_uc8_count(*uj8);
        if (!count)
        {
            if (!UTF_DEFAULT_CHAR)
            {
                *uj32 = 0;
                return UTF_INVALID;
            }
            uc32 = UTF_DEFAULT_CHAR;
        }
        else
        {
            uc8[0] = *uj8;
            for (i = 1; i < count; ++i)
            {
                if (++uj8 == uj8end)
                {
                    if (!UTF_DEFAULT_CHAR)
                    {
                        *uj32 = 0;
                        return UTF_INVALID;
                    }
                    if (uj32 == uj32end)
                    {
                        *uj32 = 0;
                        return UTF_INSUFFICIENT_BUFFER;
                    }
                    *uj32++ = UTF_STATIC_CAST(UTF_UC8, UTF_DEFAULT_CHAR);
                    *uj32 = 0;
                    return UTF_SUCCESS;
                }
                uc8[i] = *uj8;
            }

            if (!UTF_uc8_to_uc32(uc8, &uc32))
            {
                *uj32 = 0;
                return UTF_INVALID;
            }
        }

        if (uj32 == uj32end)
        {
            *uj32 = 0;
            return UTF_INSUFFICIENT_BUFFER;
        }

        *uj32++ = uc32;
    }
    *uj32 = 0;
    return UTF_SUCCESS;
}

static __inline UTF_RET
UTF_j8_to_uj32(const UTF_C8 *j8, UTF_SIZE_T j8size, UTF_UC32 *uj32, UTF_SIZE_T uj32size)
{
    return UTF_uj8_to_uj32(UTF_REINTERPRET_CAST(const UTF_UC8 *, j8), j8size, uj32, uj32size);
}

static __inline UTF_RET
UTF_uj16_to_uj8(const UTF_UC16 *uj16, UTF_SIZE_T uj16size, UTF_UC8 *uj8, UTF_SIZE_T uj8size)
{
    UTF_UC16 uc16[2];
    UTF_UC8 uc8[4];
    const UTF_UC16 *uj16end = uj16 + uj16size;
    UTF_UC8 *uj8end = uj8 + uj8size - 1;

    if (!uj8size && uj16size)
        return UTF_INSUFFICIENT_BUFFER;

    for (; uj16 != uj16end; ++uj16)
    {
        if (UTF_uc16_is_surrogate_high(*uj16))
        {
            uc16[0] = *uj16;
            if (++uj16 == uj16end)
            {
                if (!UTF_DEFAULT_CHAR)
                {
                    *uj8 = 0;
                    return UTF_INVALID;
                }
                if (uj8 == uj8end)
                {
                    *uj8 = 0;
                    return UTF_INSUFFICIENT_BUFFER;
                }
                *uj8++ = UTF_STATIC_CAST(UTF_UC8, UTF_DEFAULT_CHAR);
                *uj8 = 0;
                return UTF_SUCCESS;
            }
            uc16[1] = *uj16;
        }
        else
        {
            uc16[0] = *uj16;
            uc16[1] = 0;
        }

        if (!UTF_uc16_to_uc8(uc16, uc8))
        {
            if (!UTF_DEFAULT_CHAR)
            {
                *uj8 = 0;
                return UTF_INVALID;
            }
            uc16[0] = UTF_DEFAULT_CHAR;
            uc16[1] = 0;
        }

        if (uj8 == uj8end)
        {
            *uj8 = 0;
            return UTF_INSUFFICIENT_BUFFER;
        }

        *uj8++ = uc8[0];
        if (uc8[1])
        {
            if (uj8 == uj8end)
            {
                *uj8 = 0;
                return UTF_INSUFFICIENT_BUFFER;
            }
            *uj8++ = uc8[1];
            if (uc8[2])
            {
                if (uj8 == uj8end)
                {
                    *uj8 = 0;
                    return UTF_INSUFFICIENT_BUFFER;
                }
                *uj8++ = uc8[2];
                if (uc8[3])
                {
                    if (uj8 == uj8end)
                    {
                        *uj8 = 0;
                        return UTF_INSUFFICIENT_BUFFER;
                    }
                    *uj8++ = uc8[3];
                }
            }
        }
    }
    *uj8 = 0;
    return UTF_SUCCESS;
}

static __inline UTF_RET
UTF_uj16_to_j8(const UTF_UC16 *uj16, UTF_SIZE_T uj16size, UTF_C8 *j8, UTF_SIZE_T c8size)
{
    return UTF_uj16_to_uj8(uj16, uj16size, UTF_REINTERPRET_CAST(UTF_UC8 *, j8), c8size);
}

static __inline UTF_RET
UTF_uj16_to_uj32(const UTF_UC16 *uj16, UTF_SIZE_T uj16size, UTF_UC32 *uj32, UTF_SIZE_T uj32size)
{
    UTF_UC16 uc16[2];
    UTF_UC32 uc32;
    const UTF_UC16 *uj16end = uj16 + uj16size;
    UTF_UC32 *uj32end = uj32 + uj32size - 1;

    if (!uj32size && uj16size)
        return UTF_INSUFFICIENT_BUFFER;

    for (; uj16 != uj16end; ++uj16)
    {
        if (UTF_uc16_is_surrogate_high(*uj16))
        {
            uc16[0] = *uj16;
            if (++uj16 == uj16end)
            {
                if (!UTF_DEFAULT_CHAR)
                {
                    *uj32 = 0;
                    return UTF_INVALID;
                }
                if (uj32 == uj32end)
                {
                    *uj32 = 0;
                    return UTF_INSUFFICIENT_BUFFER;
                }
                *uj32++ = UTF_STATIC_CAST(UTF_UC8, UTF_DEFAULT_CHAR);
                *uj32 = 0;
                return UTF_SUCCESS;
            }
            uc16[1] = *uj16;
        }
        else
        {
            uc16[0] = *uj16;
            uc16[1] = 0;
        }

        if (!UTF_uc16_to_uc32(uc16, &uc32))
        {
            if (!UTF_DEFAULT_CHAR)
            {
                *uj32 = 0;
                return UTF_INVALID;
            }
            uc32 = UTF_DEFAULT_CHAR;
        }

        if (uj32 == uj32end)
        {
            *uj32 = 0;
            return UTF_INSUFFICIENT_BUFFER;
        }
        *uj32++ = uc32;
    }
    *uj32 = 0;
    return UTF_SUCCESS;
}

static __inline UTF_RET
UTF_uj32_to_uj8(const UTF_UC32 *uj32, UTF_SIZE_T uj32size, UTF_UC8 *uj8, UTF_SIZE_T uj8size)
{
    UTF_UC8 uc8[4];
    const UTF_UC32 *uj32end = uj32 + uj32size;
    UTF_UC8 *uj8end = uj8 + uj8size - 1;

    if (!uj8size && uj32size)
        return UTF_INSUFFICIENT_BUFFER;

    for (; uj32 != uj32end; ++uj32)
    {
        if (!UTF_uc32_to_uc8(*uj32, uc8))
        {
            if (!UTF_DEFAULT_CHAR)
            {
                *uj8 = 0;
                return UTF_INVALID;
            }
            uc8[0] = UTF_DEFAULT_CHAR;
            uc8[1] = 0;
        }

        if (uj8 == uj8end)
        {
            *uj8 = 0;
            return UTF_INSUFFICIENT_BUFFER;
        }

        *uj8++ = uc8[0];
        if (uc8[1])
        {
            if (uj8 == uj8end)
            {
                *uj8 = 0;
                return UTF_INSUFFICIENT_BUFFER;
            }

            *uj8++ = uc8[1];
            if (uc8[2])
            {
                if (uj8 == uj8end)
                {
                    *uj8 = 0;
                    return UTF_INSUFFICIENT_BUFFER;
                }

                *uj8++ = uc8[2];
                if (uc8[3])
                {
                    if (uj8 == uj8end)
                    {
                        *uj8 = 0;
                        return UTF_INSUFFICIENT_BUFFER;
                    }
                    *uj8++ = uc8[3];
                }
            }
        }
    }
    *uj8 = 0;
    return UTF_SUCCESS;
}

static __inline UTF_RET
UTF_uj32_to_j8(const UTF_UC32 *uj32, UTF_SIZE_T uj32size, UTF_C8 *j8, UTF_SIZE_T j8size)
{
    return UTF_uj32_to_uj8(uj32, uj32size, UTF_REINTERPRET_CAST(UTF_UC8 *, j8), j8size);
}

static __inline UTF_RET
UTF_uj32_to_uj16(const UTF_UC32 *uj32, UTF_SIZE_T uj32size, UTF_UC16 *uj16, UTF_SIZE_T uj16size)
{
    UTF_UC16 uc16[2];
    const UTF_UC32 *uj32end = uj32 + uj32size;
    UTF_UC16 *uj16end = uj16 + uj16size - 1;

    if (!uj16size && uj32size)
        return UTF_INSUFFICIENT_BUFFER;

    for (; uj32 != uj32end; ++uj32)
    {
        if (!UTF_uc32_to_uc16(*uj32, uc16))
        {
            if (!UTF_DEFAULT_CHAR)
            {
                *uj16 = 0;
                return UTF_INVALID;
            }
            uc16[0] = UTF_DEFAULT_CHAR;
            uc16[1] = 0;
        }

        if (uj16 == uj16end)
        {
            *uj16 = 0;
            return UTF_INSUFFICIENT_BUFFER;
        }
        *uj16++ = uc16[0];
        if (uc16[1])
        {
            if (uj16 == uj16end)
            {
                *uj16 = 0;
                return UTF_INSUFFICIENT_BUFFER;
            }
            *uj16++ = uc16[1];
        }
    }
    *uj16 = 0;
    return UTF_SUCCESS;
}

static __inline UTF_UC8 *
UTF8_fgets(UTF_UC8 *str, int count, FILE *fp)
{
    size_t i, cw;
    long diff;
    if (count <= 0 || feof(fp))
        return NULL;

    cw = fread(str, sizeof(UTF_UC8), UTF_STATIC_CAST(size_t, count), fp);
    if (!cw)
        return NULL;

    for (i = 0; i < cw; ++i)
    {
        if (str[i] == '\n')
        {
            if (i && str[i - 1] == '\r')
            {
                str[i - 1] = '\n';
                str[i] = 0;
                ++i;
            }
            else
            {
                if (i + 1 != count)
                {
                    ++i;
                }
                str[i] = 0;
            }
            break;
        }
    }

    if (i == count)
    {
        --i;
        str[i] = 0;
    }
    else if (i == cw && cw != count)
    {
        str[i] = 0;
    }

    if (i != cw)
    {
        diff = UTF_STATIC_CAST(long, i) - UTF_STATIC_CAST(long, cw);
        diff *= UTF_STATIC_CAST(long, sizeof(UTF_UC8));
        if (fseek(fp, diff, SEEK_CUR) != 0)
            return NULL;
    }
    return str[0] ? str : NULL;
}

static __inline UTF_UC16 *
UTF16_fgets(UTF_UC16 *str, int count, FILE *fp)
{
    size_t i, cw;
    long diff;
    if (count <= 0 || feof(fp))
        return NULL;

    cw = fread(str, sizeof(UTF_UC16), UTF_STATIC_CAST(size_t, count), fp);
    if (!cw)
        return NULL;

    for (i = 0; i < cw; ++i)
    {
        if (str[i] == '\n')
        {
            if (i && str[i - 1] == '\r')
            {
                str[i - 1] = '\n';
                str[i] = 0;
                ++i;
            }
            else
            {
                if (i + 1 != count)
                {
                    ++i;
                }
                str[i] = 0;
            }
            break;
        }
    }

    if (i == count)
    {
        --i;
        str[i] = 0;
    }
    else if (i == cw && cw != count)
    {
        str[i] = 0;
    }

    if (i != cw)
    {
        diff = UTF_STATIC_CAST(long, i) - UTF_STATIC_CAST(long, cw);
        diff *= UTF_STATIC_CAST(long, sizeof(UTF_UC16));
        if (fseek(fp, diff, SEEK_CUR) != 0)
            return NULL;
    }
    return str[0] ? str : NULL;
}

static __inline UTF_UC16
UTF_XE(UTF_UC16 uc16)
{
    UTF_UC8 lo = (UTF_UC8)uc16;
    UTF_UC8 hi = (UTF_UC8)(uc16 >> 8);
    return (((UTF_UC16)lo) << 8) | hi;
}

static __inline UTF_UC16 *
UTF16XE_fgets(UTF_UC16 *str, int count, FILE *fp)
{
    size_t i, cw;
    long diff;
    if (count <= 0 || feof(fp))
        return NULL;

    cw = fread(str, sizeof(UTF_UC16), UTF_STATIC_CAST(size_t, count), fp);
    if (!cw)
        return NULL;

    for (i = 0; i < cw; ++i)
    {
        if (str[i] == UTF_XE('\n'))
        {
            if (i && str[i - 1] == UTF_XE('\r'))
            {
                str[i - 1] = UTF_XE('\n');
                str[i] = 0;
                ++i;
            }
            else
            {
                if (i + 1 != count)
                {
                    ++i;
                }
                str[i] = 0;
            }
            break;
        }
    }

    if (i == count)
    {
        --i;
        str[i] = 0;
    }
    else if (i == cw && cw != count)
    {
        str[i] = 0;
    }

    if (i != cw)
    {
        diff = UTF_STATIC_CAST(long, i) - UTF_STATIC_CAST(long, cw);
        diff *= UTF_STATIC_CAST(long, sizeof(UTF_UC16));
        if (fseek(fp, diff, SEEK_CUR) != 0)
            return NULL;
    }

    for (i = 0; str[i]; ++i)
    {
        str[i] = UTF_XE(str[i]);
    }

    return str[0] ? str : NULL;
}

static __inline UTF_UC32 *
UTF32_fgets(UTF_UC32 *str, int count, FILE *fp)
{
    size_t i, cw;
    long diff;
    if (count <= 0 || feof(fp))
        return NULL;

    cw = fread(str, sizeof(UTF_UC32), UTF_STATIC_CAST(size_t, count), fp);
    if (!cw)
        return NULL;

    for (i = 0; i < cw; ++i)
    {
        if (str[i] == '\n')
        {
            if (i && str[i - 1] == '\r')
            {
                str[i - 1] = '\n';
                str[i] = 0;
                ++i;
            }
            else
            {
                if (i + 1 != count)
                {
                    ++i;
                }
                str[i] = 0;
            }
            break;
        }
    }

    if (i == count)
    {
        --i;
        str[i] = 0;
    }
    else if (i == cw && cw != count)
    {
        str[i] = 0;
    }

    if (i != cw)
    {
        diff = UTF_STATIC_CAST(long, i) - UTF_STATIC_CAST(long, cw);
        diff *= UTF_STATIC_CAST(long, sizeof(UTF_UC32));
        if (fseek(fp, diff, SEEK_CUR) != 0)
            return NULL;
    }
    return str[0] ? str : NULL;
}

#endif  /* ndef UTF_H_ */
