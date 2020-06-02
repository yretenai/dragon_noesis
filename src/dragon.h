//
// Created by yretenai on 5/28/2020.
//

#pragma once

#ifndef FMT_DRAGON_DRAGON_H
#define FMT_DRAGON_DRAGON_H

#include "export.h"

#include <cstdio>
#include <cstdint>
#include <stdexcept>
#include <vector>
#include <string>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <map>

#ifndef MAKEFOURCC
#define MAKEFOURCC(ch0, ch1, ch2, ch3)                                        \
                    ((DWORD)(BYTE)(ch0) | ((DWORD)(BYTE)(ch1) << 8) |         \
                    ((DWORD)(BYTE)(ch2) << 16) | ((DWORD)(BYTE)(ch3) << 24 ))
#endif // MAKEFOURCC

#ifdef USE_NOESIS

#include "noesis/pluginshare.h"

#else // USE_NOESIS
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include <new>

#endif // USE_NOESIS

using namespace std;

DRAGON_EXPORT vector<char> read_file(filesystem::path path);

DRAGON_EXPORT void write_file(filesystem::path path, vector<char> buffer);

DRAGON_EXPORT void open_dragon_log();

DRAGON_EXPORT void close_dragon_log();

DRAGON_EXPORT void flush_dragon_log();

DRAGON_EXPORT void open_dragon_log_stdout();

DRAGON_EXPORT void write_dragon_log(const char* fmt, ...);

DRAGON_EXPORT void assert_dragon_log(bool check, const char* fmt, ...);

DRAGON_EXPORT void super_assert_dragon_log(bool check, const char* error, const char* fmt, ...);

#endif //FMT_DRAGON_DRAGON_H
