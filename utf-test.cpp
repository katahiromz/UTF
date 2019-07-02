/* UTF --- UTF-8, UTF-16, UTF-32 conversion library
 * Copyright (C) 2019 Katayama Hirofumi MZ <katayama.hirofumi.mz@gmail.com>
 */
#include <iostream>
#include "utf.hpp"

int g_failures = 0;

bool UTF_test(int line, bool value)
{
    if (!value)
    {
        std::cout << "Line " << line << ": assertion failure" << std::endl;
        ++g_failures;
        return false;
    }
    return true;
}

void u8_to_u_test(int line, const UTF_S8& str1, const UTF_US16& str2, bool ok)
{
    UTF_US16 tmp;
    UTF_UC16 buf[256];
    UTF_test(line, UTF_u8_to_u<UTF_DEFAULT_CHAR, UTF_TRUNCATABLE>(str1, tmp) == ok);
    UTF_test(line, tmp == str2);
    UTF_test(line, UTF_j8_to_uj16(str1.c_str(), str1.size(), buf, 256) == UTF_SUCCESS);
    UTF_test(line, UTF_uj16_cmp(tmp.c_str(), buf) == 0);
}

void u8_to_U_test(int line, const UTF_S8& str1, const UTF_US32& str2, bool ok)
{
    UTF_US32 tmp;
    UTF_UC32 buf[256];
    UTF_test(line, UTF_u8_to_U<UTF_DEFAULT_CHAR, UTF_TRUNCATABLE>(str1, tmp) == ok);
    UTF_test(line, tmp == str2);
    UTF_test(line, UTF_j8_to_uj32(str1.c_str(), str1.size(), buf, 256) == UTF_SUCCESS);
    UTF_test(line, UTF_uj32_cmp(tmp.c_str(), buf) == 0);
}

void u_to_u8_test(int line, const UTF_US16& str1, const UTF_S8& str2, bool ok)
{
    UTF_US8 tmp;
    UTF_UC8 buf[256];
    UTF_test(line, UTF_u_to_u8<UTF_DEFAULT_CHAR, UTF_TRUNCATABLE>(str1, tmp) == ok);
    UTF_test(line, tmp == reinterpret_cast<const UTF_US8&>(str2));
    UTF_test(line, UTF_uj16_to_uj8(str1.c_str(), str1.size(), buf, 256) == UTF_SUCCESS);
    UTF_test(line, UTF_uj8_cmp(tmp.c_str(), buf) == 0);
}

void us16_to_us32_test(int line, const UTF_US16& str1, const UTF_US32& str2, bool ok)
{
    UTF_US32 tmp;
    UTF_UC32 buf[256];
    UTF_test(line, UTF_u_to_U<UTF_DEFAULT_CHAR, UTF_TRUNCATABLE>(str1, tmp) == ok);
    UTF_test(line, tmp == str2);
    UTF_test(line, UTF_uj16_to_uj32(str1.c_str(), str1.size(), buf, 256) == UTF_SUCCESS);
    UTF_test(line, UTF_uj32_cmp(tmp.c_str(), buf) == 0);
}

void U_to_u8_test(int line, const UTF_US32& str1, const UTF_S8& str2, bool ok)
{
    UTF_US8 tmp;
    UTF_UC8 buf[256];
    UTF_test(line, UTF_U_to_u8<UTF_DEFAULT_CHAR, UTF_TRUNCATABLE>(str1, tmp) == ok);
    UTF_test(line, tmp == reinterpret_cast<const UTF_US8&>(str2));
    UTF_test(line, UTF_uj32_to_uj8(str1.c_str(), str1.size(), buf, 256) == UTF_SUCCESS);
    UTF_test(line, UTF_uj8_cmp(tmp.c_str(), buf) == 0);
}

void us32_to_us16_test(int line, const UTF_US32& str1, const UTF_US16& str2, bool ok)
{
    UTF_US16 tmp;
    UTF_UC16 buf[256];
    UTF_test(line, UTF_U_to_u<UTF_DEFAULT_CHAR, UTF_TRUNCATABLE>(str1, tmp) == ok);
    UTF_test(line, tmp == str2);
    UTF_test(line, UTF_uj32_to_uj16(str1.c_str(), str1.size(), buf, 256) == UTF_SUCCESS);
    UTF_test(line, UTF_uj16_cmp(tmp.c_str(), buf) == 0);
}

int main(void)
{
    g_failures = 0;

    u8_to_u_test(__LINE__, "", UTF_WIDE(""), true);
    u8_to_U_test(__LINE__, "", U"", true);
    u_to_u8_test(__LINE__, UTF_WIDE(""), "", true);
    U_to_u8_test(__LINE__, U"", "", true);

    u8_to_u_test(__LINE__, UTF_S8("\0", 1), UTF_US16(UTF_WIDE("\0"), 1), true);
    u8_to_U_test(__LINE__, UTF_S8("\0", 1), UTF_US32(U"\U00000000", 1), true);
    u_to_u8_test(__LINE__, UTF_US16(UTF_WIDE("\u0000"), 1), UTF_S8("\0", 1), true);
    U_to_u8_test(__LINE__, UTF_US32(U"\U00000000", 1), UTF_S8("\0", 1), true);

    u8_to_u_test(__LINE__, UTF_S8("M\0M", 3), UTF_US16(UTF_WIDE("M\0M"), 3), true);
    u8_to_U_test(__LINE__, UTF_S8("M\0M", 3), UTF_US32(U"M\U00000000M", 3), true);
    u_to_u8_test(__LINE__, UTF_US16(UTF_WIDE("M\u0000M"), 3), UTF_S8("M\0M", 3), true);
    U_to_u8_test(__LINE__, UTF_US32(U"M\U00000000M", 3), UTF_S8("M\0M", 3), true);

    u8_to_u_test(__LINE__, "TEST", UTF_WIDE("TEST"), true);
    u8_to_U_test(__LINE__, "TEST", U"TEST", true);
    u_to_u8_test(__LINE__, UTF_WIDE("TEST"), "TEST", true);
    U_to_u8_test(__LINE__, U"TEST", "TEST", true);

    u8_to_U_test(__LINE__, u8"z\u00df\u6c34\U0001d10b", U"\u007a\u00df\u6c34\U0001d10b", true);
    u8_to_u_test(__LINE__, u8"z\u00df\u6c34\U0001d10b", UTF_WIDE("\u007a\u00df\u6c34\U0001d10b"), true);
    u_to_u8_test(__LINE__, UTF_WIDE("z\u00df\u6c34\U0001d10b"), u8"\u007a\u00df\u6c34\U0001d10b", true);
    U_to_u8_test(__LINE__, U"\u007a\u00df\u6c34\U0001d10b", u8"z\u00df\u6c34\U0001d10b", true);

    u8_to_u_test(__LINE__, u8"私はガラスを食べられます。それは私を傷つけません。",
                 UTF_WIDE("私はガラスを食べられます。それは私を傷つけません。"), true);
    u8_to_U_test(__LINE__, u8"私はガラスを食べられます。それは私を傷つけません。",
                 U"私はガラスを食べられます。それは私を傷つけません。", true);
    u_to_u8_test(__LINE__, UTF_WIDE("私はガラスを食べられます。それは私を傷つけません。"),
                 u8"私はガラスを食べられます。それは私を傷つけません。", true);
    U_to_u8_test(__LINE__, U"私はガラスを食べられます。それは私を傷つけません。",
                 u8"私はガラスを食べられます。それは私を傷つけません。", true);

    u8_to_u_test(__LINE__, u8"ᚠᛇᚻ᛫ᛒᛦᚦ᛫ᚠᚱᚩᚠᚢᚱ᛫ᚠᛁᚱᚪ᛫ᚷᛖᚻᚹᛦᛚᚳᚢᛗ", UTF_WIDE("ᚠᛇᚻ᛫ᛒᛦᚦ᛫ᚠᚱᚩᚠᚢᚱ᛫ᚠᛁᚱᚪ᛫ᚷᛖᚻᚹᛦᛚᚳᚢᛗ"), true);
    u_to_u8_test(__LINE__, UTF_WIDE("ᚠᛇᚻ᛫ᛒᛦᚦ᛫ᚠᚱᚩᚠᚢᚱ᛫ᚠᛁᚱᚪ᛫ᚷᛖᚻᚹᛦᛚᚳᚢᛗ"), u8"ᚠᛇᚻ᛫ᛒᛦᚦ᛫ᚠᚱᚩᚠᚢᚱ᛫ᚠᛁᚱᚪ᛫ᚷᛖᚻᚹᛦᛚᚳᚢᛗ", true);

#ifdef UTF_WIDE_IS_UTF16
    puts("UTF_WIDE_IS_UTF16");
    u8_to_u_test(__LINE__, u8"ᚠᛇᚻ᛫ᛒᛦᚦ᛫ᚠᚱᚩᚠᚢᚱ᛫ᚠᛁᚱᚪ᛫ᚷᛖᚻᚹᛦᛚᚳᚢᛗ", L"ᚠᛇᚻ᛫ᛒᛦᚦ᛫ᚠᚱᚩᚠᚢᚱ᛫ᚠᛁᚱᚪ᛫ᚷᛖᚻᚹᛦᛚᚳᚢᛗ", true);
    u_to_u8_test(__LINE__, L"ᚠᛇᚻ᛫ᛒᛦᚦ᛫ᚠᚱᚩᚠᚢᚱ᛫ᚠᛁᚱᚪ᛫ᚷᛖᚻᚹᛦᛚᚳᚢᛗ", u8"ᚠᛇᚻ᛫ᛒᛦᚦ᛫ᚠᚱᚩᚠᚢᚱ᛫ᚠᛁᚱᚪ᛫ᚷᛖᚻᚹᛦᛚᚳᚢᛗ", true);
#endif

    u8_to_u_test(__LINE__, u8"\xA0", UTF_WIDE("?"), true); //
    u8_to_u_test(__LINE__, u8"\xC3\x28", UTF_WIDE("?"), true);
    u8_to_u_test(__LINE__, u8"\xE0\x80\x80", UTF_WIDE("?"), true);
    u8_to_u_test(__LINE__, u8"\xE2\x28\xA1", UTF_WIDE("?"), true);
    u8_to_u_test(__LINE__, u8"\xE2\x82\x28", UTF_WIDE("?"), true);
    u8_to_u_test(__LINE__, u8"\xF0\x80\x80\x80", UTF_WIDE("?"), true);
    u8_to_u_test(__LINE__, u8"\xF4\xBF\xBF\xBF", UTF_WIDE("?"), true);
    u8_to_u_test(__LINE__, u8"\xF0\x28\x8C\xBC", UTF_WIDE("?"), true);
    u8_to_u_test(__LINE__, u8"\xF0\x90\x28\xBC", UTF_WIDE("?"), true);
    u8_to_u_test(__LINE__, u8"\xF0\x28\x8C\x28", UTF_WIDE("?"), true);

    if (g_failures)
    {
        puts("FAILED");
        return EXIT_FAILURE;
    }

    puts("ok");
    return EXIT_SUCCESS;
}
