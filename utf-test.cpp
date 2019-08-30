/* UTF --- UTF-8, UTF-16, UTF-32 conversion library
 * Copyright (C) 2019 Katayama Hirofumi MZ <katayama.hirofumi.mz@gmail.com>
 */
#define _CRT_SECURE_NO_WARNINGS
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
    UTF_test(line, UTF_u8_to_u<'?'>(str1, tmp) == ok);
    UTF_test(line, tmp == str2);
    UTF_test(line, UTF_j8_to_uj16(str1.c_str(), str1.size(), buf, 256) == UTF_SUCCESS);
    UTF_test(line, UTF_uj16_cmpn(tmp.c_str(), buf, UTF_SIZE_T(tmp.size() + 1)) == 0);
}

void u8_to_U_test(int line, const UTF_S8& str1, const UTF_US32& str2, bool ok)
{
    UTF_US32 tmp;
    UTF_UC32 buf[256];
    UTF_test(line, UTF_u8_to_U<'?'>(str1, tmp) == ok);
    UTF_test(line, tmp == str2);
    UTF_test(line, UTF_j8_to_uj32(str1.c_str(), str1.size(), buf, 256) == UTF_SUCCESS);
    UTF_test(line, UTF_uj32_cmpn(tmp.c_str(), buf, UTF_SIZE_T(tmp.size() + 1)) == 0);
}

void u_to_u8_test(int line, const UTF_US16& str1, const UTF_S8& str2, bool ok)
{
    UTF_US8 tmp;
    UTF_UC8 buf[256];
    UTF_test(line, UTF_u_to_u8<'?'>(str1, tmp) == ok);
    UTF_test(line, tmp == reinterpret_cast<const UTF_US8&>(str2));
    UTF_test(line, UTF_uj16_to_uj8(str1.c_str(), str1.size(), buf, 256) == UTF_SUCCESS);
    UTF_test(line, UTF_uj8_cmpn(tmp.c_str(), buf, UTF_SIZE_T(tmp.size() + 1)) == 0);
}

void us16_to_us32_test(int line, const UTF_US16& str1, const UTF_US32& str2, bool ok)
{
    UTF_US32 tmp;
    UTF_UC32 buf[256];
    UTF_test(line, UTF_u_to_U<'?'>(str1, tmp) == ok);
    UTF_test(line, tmp == str2);
    UTF_test(line, UTF_uj16_to_uj32(str1.c_str(), str1.size(), buf, 256) == UTF_SUCCESS);
    UTF_test(line, UTF_uj32_cmpn(tmp.c_str(), buf, UTF_SIZE_T(tmp.size() + 1)) == 0);
}

void U_to_u8_test(int line, const UTF_US32& str1, const UTF_S8& str2, bool ok)
{
    UTF_US8 tmp;
    UTF_UC8 buf[256];
    UTF_test(line, UTF_U_to_u8<'?'>(str1, tmp) == ok);
    UTF_test(line, tmp == reinterpret_cast<const UTF_US8&>(str2));
    UTF_test(line, UTF_uj32_to_uj8(str1.c_str(), str1.size(), buf, 256) == UTF_SUCCESS);
    UTF_test(line, UTF_uj8_cmpn(tmp.c_str(), buf, UTF_SIZE_T(tmp.size() + 1)) == 0);
}

void us32_to_us16_test(int line, const UTF_US32& str1, const UTF_US16& str2, bool ok)
{
    UTF_US16 tmp;
    UTF_UC16 buf[256];
    UTF_test(line, UTF_U_to_u<'?'>(str1, tmp) == ok);
    UTF_test(line, tmp == str2);
    UTF_test(line, UTF_uj32_to_uj16(str1.c_str(), str1.size(), buf, 256) == UTF_SUCCESS);
    UTF_test(line, UTF_uj16_cmpn(tmp.c_str(), buf, UTF_SIZE_T(tmp.size() + 1)) == 0);
}

void UTF_fgets_test(const char *fname)
{
    FILE *fp;
    UTF_UC16 buf[64];
    UTF_C8 buf2[128];

    fp = fopen(fname, "rb");
    if (!fp)
    {
        UTF_test(__LINE__, fp != NULL);
        return;
    }

    UTF_test(__LINE__, UTF_fgets(buf, 64, fp) == buf);
    UTF_test(__LINE__, UTF_uj16_cmp(buf, UTF_u("TEST\n")) == 0);
    UTF_uj16_to_j8(buf, UTF_uj16_len(buf) + 1, buf2, 128);
    UTF_test(__LINE__, UTF_j8_cmp(buf2, UTF_u8("TEST\n")) == 0);

    UTF_test(__LINE__, UTF_fgets(buf, 64, fp) == buf);
    UTF_test(__LINE__, UTF_uj16_cmp(buf, UTF_u("ABC123\n")) == 0);
    UTF_uj16_to_j8(buf, UTF_uj16_len(buf) + 1, buf2, 128);
    UTF_test(__LINE__, UTF_j8_cmp(buf2, UTF_u8("ABC123\n")) == 0);

    UTF_test(__LINE__, UTF16_fgets(buf, 64, fp) == buf);
    UTF_test(__LINE__, UTF_uj16_cmp(buf, UTF_u("\u3042\u3044\u3046\u3048\u304A\n")) == 0);
    UTF_uj16_to_j8(buf, UTF_uj16_len(buf) + 1, buf2, 128);
    UTF_test(__LINE__, UTF_j8_cmp(buf2, "\xE3\x81\x82\xE3\x81\x84\xE3\x81\x86\xE3\x81\x88\xE3\x81\x8A\n") == 0);

    UTF_test(__LINE__, UTF_fgets(buf, 64, fp) == buf);
    UTF_test(__LINE__, UTF_uj16_cmp(buf, UTF_u("\u6F22\u5B57\n")) == 0);
    UTF_uj16_to_j8(buf, UTF_uj16_len(buf) + 1, buf2, 128);
    UTF_test(__LINE__, UTF_j8_cmp(buf2, "\xE6\xBC\xA2\xE5\xAD\x97\n") == 0);

    UTF_test(__LINE__, UTF_fgets(buf, 64, fp) == NULL);

    fclose(fp);
}

void UTF_fgets_test2(const char *fname)
{
    FILE *fp;
    char buf[128];

    fp = fopen(fname, "rb");
    if (!fp)
    {
        UTF_test(__LINE__, fp != NULL);
        return;
    }

    UTF_test(__LINE__, UTF_fgets(buf, 10, fp) == buf);
    UTF_test(__LINE__, strcmp(buf, "012345678") == 0);
    UTF_test(__LINE__, UTF_fgets(buf, 10, fp) == buf);
    UTF_test(__LINE__, strcmp(buf, "901234567") == 0);
    UTF_test(__LINE__, UTF_fgets(buf, 10, fp) == buf);
    UTF_test(__LINE__, strcmp(buf, "890123456") == 0);
    UTF_test(__LINE__, UTF_fgets(buf, 10, fp) == buf);
    UTF_test(__LINE__, strcmp(buf, "78901234") == 0);
    UTF_test(__LINE__, UTF_fgets(buf, 10, fp) == NULL);

    fclose(fp);
}

int main(int argc, char **argv)
{
    g_failures = 0;

    u8_to_u_test(__LINE__, "", UTF_u(""), true);
    u8_to_U_test(__LINE__, "", UTF_U(""), true);
    u_to_u8_test(__LINE__, UTF_u(""), "", true);
    U_to_u8_test(__LINE__, UTF_U(""), "", true);

    u8_to_u_test(__LINE__, UTF_S8(1, 0), UTF_US16(1, 0), true);
    u8_to_U_test(__LINE__, UTF_S8(1, 0), UTF_US32(1, 0), true);
    u_to_u8_test(__LINE__, UTF_US16(1, 0), UTF_S8(1, 0), true);
    U_to_u8_test(__LINE__, UTF_US32(1, 0), UTF_S8(1, 0), true);

    u8_to_u_test(__LINE__, UTF_S8("M\0M", 3), UTF_US16(UTF_u("M\0M"), 3), true);
    u8_to_U_test(__LINE__, UTF_S8("M\0M", 3), UTF_US32(UTF_U("M\0M"), 3), true);
    u_to_u8_test(__LINE__, UTF_US16(UTF_u("M\0M"), 3), UTF_S8("M\0M", 3), true);
    U_to_u8_test(__LINE__, UTF_US32(UTF_U("M\0M"), 3), UTF_S8("M\0M", 3), true);

    u8_to_u_test(__LINE__, "TEST", UTF_u("TEST"), true);
    u8_to_U_test(__LINE__, "TEST", UTF_U("TEST"), true);
    u_to_u8_test(__LINE__, UTF_u("TEST"), "TEST", true);
    U_to_u8_test(__LINE__, UTF_U("TEST"), "TEST", true);

    u8_to_U_test(__LINE__, UTF_u8("z\u00df\u6c34\U0001d10b"), UTF_U("\u007a\u00df\u6c34\U0001d10b"), true);
    u8_to_u_test(__LINE__, UTF_u8("z\u00df\u6c34\U0001d10b"), UTF_u("\u007a\u00df\u6c34\U0001d10b"), true);
    u_to_u8_test(__LINE__, UTF_u("z\u00df\u6c34\U0001d10b"), UTF_u8("\u007a\u00df\u6c34\U0001d10b"), true);
    U_to_u8_test(__LINE__, UTF_U("\u007a\u00df\u6c34\U0001d10b"), UTF_u8("z\u00df\u6c34\U0001d10b"), true);

    u8_to_u_test(__LINE__, UTF_u8("私はガラスを食べられます。それは私を傷つけません。"),
                 UTF_u("私はガラスを食べられます。それは私を傷つけません。"), true);
    u8_to_U_test(__LINE__, UTF_u8("私はガラスを食べられます。それは私を傷つけません。"),
                 UTF_U("私はガラスを食べられます。それは私を傷つけません。"), true);
    u_to_u8_test(__LINE__, UTF_u("私はガラスを食べられます。それは私を傷つけません。"),
                 UTF_u8("私はガラスを食べられます。それは私を傷つけません。"), true);
    U_to_u8_test(__LINE__, UTF_U("私はガラスを食べられます。それは私を傷つけません。"),
                 UTF_u8("私はガラスを食べられます。それは私を傷つけません。"), true);

    u8_to_u_test(__LINE__, UTF_u8("ᚠᛇᚻ᛫ᛒᛦᚦ᛫ᚠᚱᚩᚠᚢᚱ᛫ᚠᛁᚱᚪ᛫ᚷᛖᚻᚹᛦᛚᚳᚢᛗ"), UTF_u("ᚠᛇᚻ᛫ᛒᛦᚦ᛫ᚠᚱᚩᚠᚢᚱ᛫ᚠᛁᚱᚪ᛫ᚷᛖᚻᚹᛦᛚᚳᚢᛗ"), true);
    u_to_u8_test(__LINE__, UTF_u("ᚠᛇᚻ᛫ᛒᛦᚦ᛫ᚠᚱᚩᚠᚢᚱ᛫ᚠᛁᚱᚪ᛫ᚷᛖᚻᚹᛦᛚᚳᚢᛗ"), UTF_u8("ᚠᛇᚻ᛫ᛒᛦᚦ᛫ᚠᚱᚩᚠᚢᚱ᛫ᚠᛁᚱᚪ᛫ᚷᛖᚻᚹᛦᛚᚳᚢᛗ"), true);

#ifdef UTF_WIDE_IS_UTF16
    puts("UTF_WIDE_IS_UTF16");
    u8_to_u_test(__LINE__, UTF_u8("ᚠᛇᚻ᛫ᛒᛦᚦ᛫ᚠᚱᚩᚠᚢᚱ᛫ᚠᛁᚱᚪ᛫ᚷᛖᚻᚹᛦᛚᚳᚢᛗ"), UTF_L("ᚠᛇᚻ᛫ᛒᛦᚦ᛫ᚠᚱᚩᚠᚢᚱ᛫ᚠᛁᚱᚪ᛫ᚷᛖᚻᚹᛦᛚᚳᚢᛗ"), true);
    u_to_u8_test(__LINE__, UTF_L("ᚠᛇᚻ᛫ᛒᛦᚦ᛫ᚠᚱᚩᚠᚢᚱ᛫ᚠᛁᚱᚪ᛫ᚷᛖᚻᚹᛦᛚᚳᚢᛗ"), UTF_u8("ᚠᛇᚻ᛫ᛒᛦᚦ᛫ᚠᚱᚩᚠᚢᚱ᛫ᚠᛁᚱᚪ᛫ᚷᛖᚻᚹᛦᛚᚳᚢᛗ"), true);
#endif

    u8_to_u_test(__LINE__, "\xA0", UTF_u("?"), true); 
    u8_to_u_test(__LINE__, "\xC3\x28", UTF_u("?"), true);
    u8_to_u_test(__LINE__, "\xE0\x80\x80", UTF_u("?"), true);
    u8_to_u_test(__LINE__, "\xE2\x28\xA1", UTF_u("?"), true);
    u8_to_u_test(__LINE__, "\xE2\x82\x28", UTF_u("?"), true);
    u8_to_u_test(__LINE__, "\xF0\x80\x80\x80", UTF_u("?"), true);
    u8_to_u_test(__LINE__, "\xF4\xBF\xBF\xBF", UTF_u("?"), true);
    u8_to_u_test(__LINE__, "\xF0\x28\x8C\xBC", UTF_u("?"), true);
    u8_to_u_test(__LINE__, "\xF0\x90\x28\xBC", UTF_u("?"), true);
    u8_to_u_test(__LINE__, "\xF0\x28\x8C\x28", UTF_u("?"), true);

    if (argc >= 2)
        UTF_fgets_test(argv[1]);

    if (argc >= 3)
        UTF_fgets_test2(argv[2]);

    if (g_failures)
    {
        puts("FAILED");
        return EXIT_FAILURE;
    }

    puts("ok");
    return EXIT_SUCCESS;
}
