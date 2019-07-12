/* UTF --- UTF-8, UTF-16, UTF-32 conversion library
 * Copyright (C) 2019 Katayama Hirofumi MZ <katayama.hirofumi.mz@gmail.com>
 */
#ifndef UTF_HPP_
#define UTF_HPP_    6   // Version 6

#include "utf.h"

#ifndef __cplusplus
    #error Use C++ compiler.
#endif

#include <string>

/* UTF_US8, UTF_US16, UTF_US32 --- string classes */
typedef std::string UTF_S8;
typedef std::basic_string<UTF_UC8> UTF_US8;
#if __cplusplus >= 201103L
    #ifdef UTF_WIDE_IS_UTF16
        typedef std::wstring UTF_US16;
    #else
        typedef std::u16string UTF_US16;
    #endif
    typedef std::u32string UTF_US32;
#else
    #ifdef UTF_WIDE_IS_UTF16
        typedef std::wstring UTF_US16;
    #else
        typedef std::basic_string<UTF_UC16> UTF_US16;
    #endif
    typedef std::basic_string<UTF_UC32> UTF_US32;
#endif

#ifndef UTF_OPT_
    #if __cplusplus >= 201103L
        #define UTF_OPT_(arg) = arg
    #else
        #define UTF_OPT_(arg)
    #endif
#endif

template <char t_default_char UTF_OPT_(UTF_DEFAULT_CHAR)>
inline bool
UTF_u8_to_u(const UTF_US8& us8, UTF_US16& us16)
{
    UTF_UC8 uc8[4];
    UTF_UC16 uc16[2];
    UTF_US8::const_iterator it, end = us8.end();
    for (it = us8.begin(); it != end; ++it)
    {
        int count = UTF_uc8_count(*it);
        if (!count)
        {
            if (!t_default_char)
                return false;

            us16.push_back(UTF_STATIC_CAST(UTF_UC8, t_default_char));
            continue;
        }

        uc8[0] = *it;
        for (int i = 1; i < count; ++i)
        {
            ++it;
            if (it == end)
            {
                if (!t_default_char)
                    return false;

                us16.push_back(UTF_STATIC_CAST(UTF_UC8, t_default_char));
                return true;
            }

            uc8[i] = *it;
        }

        if (!UTF_uc8_to_uc16(uc8, uc16))
        {
            if (!t_default_char)
                return false;

            us16.push_back(UTF_STATIC_CAST(UTF_UC8, t_default_char));
            continue;
        }

        us16.push_back(uc16[0]);
        if (uc16[1])
            us16.push_back(uc16[1]);
    }
    return true;
}

template <char t_default_char UTF_OPT_(UTF_DEFAULT_CHAR)>
inline bool
UTF_u8_to_u(const UTF_S8& s8, UTF_US16& us16)
{
    return UTF_u8_to_u<t_default_char>(reinterpret_cast<const UTF_US8&>(s8), us16);
}

template <char t_default_char UTF_OPT_(UTF_DEFAULT_CHAR)>
inline bool
UTF_u8_to_U(const UTF_US8& us8, UTF_US32& us32)
{
    UTF_UC8 uc8[4];
    UTF_UC32 uc32;
    UTF_US8::const_iterator it, end = us8.end();
    for (it = us8.begin(); it != end; ++it)
    {
        int count = UTF_uc8_count(UTF_STATIC_CAST(UTF_UC8, *it));
        if (!count)
        {
            if (!t_default_char)
                return false;

            us32.push_back(UTF_STATIC_CAST(UTF_UC8, t_default_char));
            continue;
        }

        uc8[0] = *it;
        for (int i = 1; i < count; ++i)
        {
            ++it;
            if (it == end)
            {
                if (!t_default_char)
                    return false;

                us32.push_back(UTF_STATIC_CAST(UTF_UC8, t_default_char));
                return true;
            }
            uc8[i] = *it;
        }

        if (!UTF_uc8_to_uc32(uc8, &uc32))
        {
            if (!t_default_char)
                return false;

            us32.push_back(UTF_STATIC_CAST(UTF_UC8, t_default_char));
            continue;
        }

        us32.push_back(uc32);
    }
    return true;
}

template <char t_default_char UTF_OPT_(UTF_DEFAULT_CHAR)>
inline bool
UTF_u8_to_U(const UTF_S8& s8, UTF_US32& us32)
{
    return UTF_u8_to_U<t_default_char>(reinterpret_cast<const UTF_US8&>(s8), us32);
}

template <char t_default_char UTF_OPT_(UTF_DEFAULT_CHAR)>
inline bool
UTF_u_to_u8(const UTF_US16& us16, UTF_US8& us8)
{
    UTF_UC16 uc16[2];
    UTF_UC8 uc8[4];
    UTF_US16::const_iterator it, end = us16.end();
    for (it = us16.begin(); it != end; ++it)
    {
        if (UTF_uc16_is_surrogate_high(*it))
        {
            uc16[0] = *it;
            ++it;
            if (it == end)
            {
                if (!t_default_char)
                    return false;

                us8.push_back(UTF_STATIC_CAST(UTF_UC8, t_default_char));
                return true;
            }

            uc16[1] = *it;
        }
        else
        {
            uc16[0] = *it;
            uc16[1] = 0;
        }

        if (!UTF_uc16_to_uc8(uc16, uc8))
        {
            if (!t_default_char)
                return false;

            us8.push_back(UTF_STATIC_CAST(UTF_UC8, t_default_char));
            continue;
        }

        us8.push_back(uc8[0]);
        if (uc8[1])
        {
            us8.push_back(uc8[1]);
            if (uc8[2])
            {
                us8.push_back(uc8[2]);
                if (uc8[3])
                {
                    us8.push_back(uc8[3]);
                }
            }
        }
    }

    return true;
}

template <char t_default_char UTF_OPT_(UTF_DEFAULT_CHAR)>
inline bool
UTF_u_to_u8(const UTF_US16& us16, UTF_S8& s8)
{
    return UTF_u_to_u8<t_default_char>(us16, reinterpret_cast<UTF_US8&>(s8));
}

template <char t_default_char UTF_OPT_(UTF_DEFAULT_CHAR)>
inline bool
UTF_u_to_U(const UTF_US16& us16, UTF_US32& us32)
{
    UTF_UC16 uc16[2];
    UTF_UC32 uc32;
    UTF_US16::const_iterator it, end = us16.end();
    for (it = us16.begin(); it != end; ++it)
    {
        if (UTF_uc16_is_surrogate_high(*it))
        {
            uc16[0] = *it;
            ++it;
            if (it == end)
            {
                if (!t_default_char)
                    return false;

                us32.push_back(UTF_STATIC_CAST(UTF_UC8, t_default_char));
                return true;
            }

            uc16[1] = *it;
        }
        else
        {
            uc16[0] = *it;
            uc16[1] = 0;
        }

        if (!UTF_uc16_to_uc32(uc16, &uc32))
        {
            if (!t_default_char)
                return false;

            uc32 = UTF_STATIC_CAST(UTF_UC8, t_default_char);
        }

        us32.push_back(uc32);
    }

    return true;
}

template <char t_default_char UTF_OPT_(UTF_DEFAULT_CHAR)>
inline bool
UTF_U_to_u8(const UTF_US32& us32, UTF_US8& us8)
{
    UTF_UC8 uc8[4];
    UTF_US32::const_iterator it, end = us32.end();
    for (it = us32.begin(); it != end; ++it)
    {
        if (!UTF_uc32_to_uc8(*it, uc8))
        {
            if (!t_default_char)
                return false;

            us8.push_back(UTF_STATIC_CAST(UTF_UC8, t_default_char));
            continue;
        }

        us8.push_back(uc8[0]);
        if (uc8[1])
        {
            us8.push_back(uc8[1]);
            if (uc8[2])
            {
                us8.push_back(uc8[2]);
                if (uc8[3])
                {
                    us8.push_back(uc8[3]);
                }
            }
        }
    }

    return true;
}

template <char t_default_char UTF_OPT_(UTF_DEFAULT_CHAR)>
inline bool
UTF_U_to_u8(const UTF_US32& us32, UTF_S8& s8)
{
    return UTF_U_to_u8<t_default_char>(us32, reinterpret_cast<UTF_US8&>(s8));
}

template <char t_default_char UTF_OPT_(UTF_DEFAULT_CHAR)>
inline bool
UTF_U_to_u(const UTF_US32& us32, UTF_US16& us16)
{
    UTF_UC16 uc16[2];
    UTF_US32::const_iterator it, end = us32.end();
    for (it = us32.begin(); it != end; ++it)
    {
        if (!UTF_uc32_to_uc16(*it, uc16))
        {
            if (!t_default_char)
                return false;

            us16.push_back(UTF_STATIC_CAST(UTF_UC8, t_default_char));
            continue;
        }

        us16.push_back(uc16[0]);
        if (uc16[1])
        {
            us16.push_back(uc16[1]);
        }
    }

    return true;
}

template <char t_default_char UTF_OPT_(UTF_DEFAULT_CHAR)>
inline bool
UTF_u8_to_u8(const UTF_US8& src, UTF_US8& dest)
{
    dest = src;
    return true;
}

template <char t_default_char UTF_OPT_(UTF_DEFAULT_CHAR)>
inline bool
UTF_u_to_u(const UTF_US16& src, UTF_US16& dest)
{
    dest = src;
    return true;
}

template <char t_default_char UTF_OPT_(UTF_DEFAULT_CHAR)>
inline bool
UTF_U_to_U(const UTF_US32& src, UTF_US32& dest)
{
    dest = src;
    return true;
}

template <typename T>
inline int UTF_cmp(const T *a, const T *b)
{
    while (*a && *b)
    {
        if (*a < *b) return -1;
        if (*a > *b) return 1;
        ++a;
        ++b;
    }
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

template <typename T>
inline int UTF_cmpn(const T *a, const T *b, size_t len)
{
    while (*a && *b)
    {
        if (len-- == 0) return 0;
        if (*a < *b) return -1;
        if (*a > *b) return 1;
        ++a;
        ++b;
    }
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

template <typename UT>
inline UT *
UTF_fgets(UT *str, int count, FILE *fp)
{
    size_t i, cw;
    long diff;
    if (count <= 0 || feof(fp))
        return NULL;

    cw = fread(str, sizeof(UT), UTF_STATIC_CAST(size_t, count), fp);
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
        diff *= UTF_STATIC_CAST(long, sizeof(UT));
        if (fseek(fp, diff, SEEK_CUR) != 0)
            return NULL;
    }
    return str;
}

#ifdef UTF_WIDE_IS_UTF16
    #define UTF_L_to_U UTF_u_to_U
    #define UTF_L_to_u UTF_u_to_u
    #define UTF_L_to_u8 UTF_u_to_u8

    #define UTF_u8_to_L UTF_u8_to_u
    #define UTF_u_to_L UTF_u_to_u
    #define UTF_U_to_L UTF_U_to_u

    #define UTF_L_to_L UTF_u_to_u
#endif

#endif   /* ndef UTF_HPP_ */
