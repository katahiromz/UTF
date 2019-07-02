/* UTF --- UTF-8, UTF-16, UTF-32 conversion library
 * Copyright (C) 2019 Katayama Hirofumi MZ <katayama.hirofumi.mz@gmail.com>
 */
#ifndef UTF_H_
#define UTF_H_  4   /* Version 4  */

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
    #include <cstring>
    #define UTF_STATIC_CAST(type, value) static_cast<type>(value)
    #define UTF_REINTERPRET_CAST(type, value) reinterpret_cast<type>(value)
#else
    #include <string.h>
    #define UTF_STATIC_CAST(type, value) ((type)(value))
    #define UTF_REINTERPRET_CAST(type, value) ((type)(value))
#endif

/* UTF_C8, UTF_UC8, UTF_UC16, UTF_UC32 --- characters */
typedef char UTF_C8;
typedef uint8_t UTF_UC8;
#if defined(__cplusplus) && __cplusplus >= 201103L  /* C++11 */
    #ifdef UTF_WIDE_IS_UTF16
        typedef wchar_t UTF_UC16;
    #else
        typedef char16_t UTF_UC16;
    #endif
    typedef char32_t UTF_UC32;
#elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L  /* C11 */
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
    /* #define UTF_DEFAULT_CHAR 0 */
#endif

#ifndef UTF_WIDE
    #ifdef UTF_WIDE_IS_UTF16
        #define UTF_WIDE(str) L##str
    #else
        #define UTF_WIDE(str) u##str
    #endif
#endif

#ifndef UTF_TRUNCATABLE
    #define UTF_TRUNCATABLE true
#endif

enum UTF_RET
{
    UTF_FOLDED = -1,
    UTF_INVALID = 0,
    UTF_SUCCESS = 1,
    UTF_INSUFFICIENT_BUFFER = 2
};

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
            *uc32 = 0x10000 + (UTF_STATIC_CAST(UTF_UC32, uc16[0]) - 0xD800) * 0x400 + (UTF_STATIC_CAST(UTF_UC32, uc16[1]) - 0xDC00);
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
UTF_uj8_to_uj16(const UTF_UC8 *uj8, size_t uj8size, UTF_UC16 *uj16, size_t uj16size)
{
    UTF_UC8 uc8[4];
    UTF_UC16 uc16[2];
    int i, count;
    const UTF_UC8 *uj8end = uj8 + uj8size;
    UTF_UC16 *uj16end = uj16 + uj16size;

    if (!uj16size && uj8size)
        return UTF_INSUFFICIENT_BUFFER;

    memset(uj16, 0, uj16size * sizeof(*uj16));

    for (; uj8 != uj8end; ++uj8)
    {
        count = UTF_uc8_count(*uj8);
        if (!count)
        {
            if (!UTF_DEFAULT_CHAR)
                return UTF_INVALID;

            if (uj16 == uj16end)
                return UTF_INSUFFICIENT_BUFFER;

            *uj16++ = uc16[0];
            continue;
        }

        uc8[0] = *uj8;
        for (i = 1; i < count; ++i)
        {
            if (++uj8 == uj8end)
                return UTF_FOLDED;

            uc8[i] = *uj8;
        }

        if (!UTF_uc8_to_uc16(uc8, uc16))
        {
            if (!UTF_DEFAULT_CHAR)
                return UTF_INVALID;

            uc16[0] = UTF_DEFAULT_CHAR;
            uc16[1] = 0;
        }

        if (uj16 == uj16end)
            return UTF_INSUFFICIENT_BUFFER;

        *uj16++ = uc16[0];
        if (uc16[1])
        {
            if (uj16 == uj16end)
                return UTF_INSUFFICIENT_BUFFER;

            *uj16++ = uc16[1];
        }
    }
    return UTF_SUCCESS;
}

static __inline UTF_RET
UTF_j8_to_uj16(const UTF_C8 *j8, size_t j8size, UTF_UC16 *uj16, size_t uj16size)
{
    return UTF_uj8_to_uj16(UTF_REINTERPRET_CAST(const UTF_UC8 *, j8), j8size, uj16, uj16size);
}

static __inline UTF_RET
UTF_uj8_to_uj32(const UTF_UC8 *uj8, size_t uj8size, UTF_UC32 *uj32, size_t uj32size)
{
    UTF_UC8 uc8[4];
    UTF_UC32 uc32;
    int i, count;
    const UTF_UC8 *uj8end = uj8 + uj8size;
    UTF_UC32 *uj32end = uj32 + uj32size;

    if (!uj32size && uj8size)
        return UTF_INSUFFICIENT_BUFFER;

    memset(uj32, 0, uj32size * sizeof(*uj32));

    for (; uj8 != uj8end; ++uj8)
    {
        count = UTF_uc8_count(*uj8);
        if (!count)
        {
            if (!UTF_DEFAULT_CHAR)
                return UTF_INVALID;

            uc32 = UTF_DEFAULT_CHAR;
        }
        else
        {

            uc8[0] = *uj8;
            for (i = 1; i < count; ++i)
            {
                if (++uj8 == uj8end)
                    return UTF_FOLDED;

                uc8[i] = *uj8;
            }

            if (!UTF_uc8_to_uc32(uc8, &uc32))
                return UTF_INVALID;
        }

        if (uj32 == uj32end)
            return UTF_INSUFFICIENT_BUFFER;

        *uj32++ = uc32;
    }
    return UTF_SUCCESS;
}

static __inline UTF_RET
UTF_j8_to_uj32(const UTF_C8 *j8, size_t j8size, UTF_UC32 *uj32, size_t uj32size)
{
    return UTF_uj8_to_uj32(UTF_REINTERPRET_CAST(const UTF_UC8 *, j8), j8size, uj32, uj32size);
}

static __inline UTF_RET
UTF_uj16_to_uj8(const UTF_UC16 *uj16, size_t uj16size, UTF_UC8 *uj8, size_t uj8size)
{
    UTF_UC16 uc16[2];
    UTF_UC8 uc8[4];
    const UTF_UC16 *uj16end = uj16 + uj16size;
    UTF_UC8 *uj8end = uj8 + uj8size;

    if (!uj8size && uj16size)
        return UTF_INSUFFICIENT_BUFFER;

    memset(uj8, 0, uj8size * sizeof(*uj8));

    for (; uj16 != uj16end; ++uj16)
    {
        if (UTF_uc16_is_surrogate_high(*uj16))
        {
            uc16[0] = *uj16;
            if (++uj16 == uj16end)
                return UTF_FOLDED;

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
                return UTF_INVALID;

            uc16[0] = UTF_DEFAULT_CHAR;
            uc16[1] = 0;
        }

        if (uj8 == uj8end)
            return UTF_INSUFFICIENT_BUFFER;

        *uj8++ = uc8[0];
        if (uc8[1])
        {
            if (uj8 == uj8end)
                return UTF_INSUFFICIENT_BUFFER;

            *uj8++ = uc8[1];
            if (uc8[2])
            {
                if (uj8 == uj8end)
                    return UTF_INSUFFICIENT_BUFFER;

                *uj8++ = uc8[2];
                if (uc8[3])
                {
                    if (uj8 == uj8end)
                        return UTF_INSUFFICIENT_BUFFER;

                    *uj8++ = uc8[3];
                }
            }
        }
    }

    return UTF_SUCCESS;
}

static __inline UTF_RET
UTF_uj16_to_j8(const UTF_UC16 *uj16, size_t uj16size, UTF_C8 *j8, size_t c8size)
{
    return UTF_uj16_to_uj8(uj16, uj16size, UTF_REINTERPRET_CAST(UTF_UC8 *, j8), c8size);
}

static __inline UTF_RET
UTF_uj16_to_uj32(const UTF_UC16 *uj16, size_t uj16size, UTF_UC32 *uj32, size_t uj32size)
{
    UTF_UC16 uc16[2];
    UTF_UC32 uc32;
    const UTF_UC16 *uj16end = uj16 + uj16size;
    UTF_UC32 *uj32end = uj32 + uj32size;

    if (!uj32size && uj16size)
        return UTF_INSUFFICIENT_BUFFER;

    memset(uj32, 0, uj32size * sizeof(*uj32));

    for (; uj16 != uj16end; ++uj16)
    {
        if (UTF_uc16_is_surrogate_high(*uj16))
        {
            uc16[0] = *uj16;
            if (++uj16 == uj16end)
                return UTF_FOLDED;

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
                return UTF_INVALID;

            uc32 = UTF_DEFAULT_CHAR;
        }

        if (uj32 == uj32end)
            return UTF_INSUFFICIENT_BUFFER;

        *uj32++ = uc32;
    }

    return UTF_SUCCESS;
}

static __inline UTF_RET
UTF_uj32_to_uj8(const UTF_UC32 *uj32, size_t uj32size, UTF_UC8 *uj8, size_t uj8size)
{
    UTF_UC8 uc8[4];
    const UTF_UC32 *uj32end = uj32 + uj32size;
    UTF_UC8 *uj8end = uj8 + uj8size;

    if (!uj8size && uj32size)
        return UTF_INSUFFICIENT_BUFFER;

    memset(uj8, 0, uj8size * sizeof(*uj8));

    for (; uj32 != uj32end; ++uj32)
    {
        if (!UTF_uc32_to_uc8(*uj32, uc8))
        {
            if (!UTF_DEFAULT_CHAR)
                return UTF_INVALID;

            uc8[0] = UTF_DEFAULT_CHAR;
            uc8[1] = 0;
        }

        if (uj8 == uj8end)
            return UTF_INSUFFICIENT_BUFFER;

        *uj8++ = uc8[0];
        if (uc8[1])
        {
            if (uj8 == uj8end)
                return UTF_INSUFFICIENT_BUFFER;

            *uj8++ = uc8[1];
            if (uc8[2])
            {
                if (uj8 == uj8end)
                    return UTF_INSUFFICIENT_BUFFER;

                *uj8++ = uc8[2];
                if (uc8[3])
                {
                    if (uj8 == uj8end)
                        return UTF_INSUFFICIENT_BUFFER;

                    *uj8++ = uc8[3];
                }
            }
        }
    }

    return UTF_SUCCESS;
}

static __inline UTF_RET
UTF_uj32_to_j8(const UTF_UC32 *uj32, size_t uj32size, UTF_C8 *j8, size_t j8size)
{
    return UTF_uj32_to_uj8(uj32, uj32size, UTF_REINTERPRET_CAST(UTF_UC8 *, j8), j8size);
}

static __inline UTF_RET
UTF_uj32_to_uj16(const UTF_UC32 *uj32, size_t uj32size, UTF_UC16 *uj16, size_t uj16size)
{
    UTF_UC16 uc16[2];
    const UTF_UC32 *uj32end = uj32 + uj32size;
    UTF_UC16 *uj16end = uj16 + uj16size;

    if (!uj16size && uj32size)
        return UTF_INSUFFICIENT_BUFFER;

    memset(uj16, 0, uj16size * sizeof(*uj16));

    for (; uj32 != uj32end; ++uj32)
    {
        if (!UTF_uc32_to_uc16(*uj32, uc16))
        {
            if (!UTF_DEFAULT_CHAR)
                return UTF_INVALID;

            uc16[0] = UTF_DEFAULT_CHAR;
            uc16[1] = 0;
        }

        if (uj16 == uj16end)
            return UTF_INSUFFICIENT_BUFFER;

        *uj16++ = uc16[0];
        if (uc16[1])
        {
            if (uj16 == uj16end)
                return UTF_INSUFFICIENT_BUFFER;

            *uj16++ = uc16[1];
        }
    }

    return UTF_SUCCESS;
}

#endif  /* ndef UTF_H_ */
