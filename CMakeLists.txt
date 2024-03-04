# CMakeLists.txt --- CMake project settings
#    ex) cmake -G "Visual Studio 9 2008" .
#    ex) cmake -DCMAKE_BUILD_TYPE=Release -G "MSYS Makefiles" .
##############################################################################

# CMake minimum version
cmake_minimum_required(VERSION 3.5)

# enable testing
enable_testing()

# project name and language
project(UTF LANGUAGES C CXX)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # using Clang
    if (WIN32)
        set(CMAKE_C_FLAGS "-static -s")
        set(CMAKE_CXX_FLAGS "-static -s")
    endif()
    set(CMAKE_CXX_STANDARD 11)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # using GCC
    if (WIN32)
        set(CMAKE_C_FLAGS "-static -static-libgcc -static-libstdc++ -s")
        set(CMAKE_CXX_FLAGS "-static -static-libgcc -static-libstdc++ -s")
    endif()
    set(CMAKE_CXX_STANDARD 11)
elseif (MSVC)
    # replace "/MD" with "/MT" (building without runtime DLLs)
    set(CompilerFlags
        CMAKE_C_FLAGS
        CMAKE_C_FLAGS_DEBUG
        CMAKE_C_FLAGS_RELEASE
        CMAKE_C_FLAGS_RELWITHDEBINFO
        CMAKE_CXX_FLAGS
        CMAKE_CXX_FLAGS_DEBUG
        CMAKE_CXX_FLAGS_RELEASE
        CMAKE_CXX_FLAGS_RELWITHDEBINFO)
    foreach(CompilerFlags ${CompilerFlags})
        string(REPLACE "/MD" "/MT" ${CompilerFlags} "${${CompilerFlags}}")
    endforeach()
endif()

##############################################################################

option(WIDE_IS_UTF16 "Wide string is UTF-16" OFF)
if (WIDE_IS_UTF16)
    add_definitions(-DUTF_WIDE_IS_UTF16)
endif()

# executable
add_executable(utf-test utf-test.cpp)
if (MSVC)
    target_compile_options(utf-test PRIVATE /source-charset:utf-8 /execution-charset:utf-8)
endif()

# test
add_test(NAME utf-test COMMAND $<TARGET_FILE:utf-test> ${CMAKE_CURRENT_SOURCE_DIR}/DATA1.dat ${CMAKE_CURRENT_SOURCE_DIR}/DATA2.dat)

##############################################################################
