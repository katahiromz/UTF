# UTF

[![Travis Build Status](https://travis-ci.org/katahiromz/UTF.svg?branch=master)](https://travis-ci.org/katahiromz/UTF)
[![AppVeyor Build status](https://ci.appveyor.com/api/projects/status/o1vl6i1pd35sw2n6?svg=true)](https://ci.appveyor.com/project/katahiromz/utf)

UTF is C/C++ UTF-8, UTF-16, UTF-32 conversion library by Katayama Hirofumi MZ.

License: MIT

## Note

If you are programming on Windows, you may want to use these APIs:

```c
#include <windows.h>
MultiByteToWideChar(CP_UTF8, 0, ...);
MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, ...);
WideCharToMultiByte(CP_UTF8, 0, ...);
IsTextUnicode(...);
```
