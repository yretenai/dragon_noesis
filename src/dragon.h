//
// Created by yretenai on 5/28/2020.
//

#pragma once

#ifndef FMT_DRAGON_DRAGON_H
#define FMT_DRAGON_DRAGON_H

#include <cstdio>
#include <cstdint>
#include <stdexcept>
#include <vector>
#include <string>
#include <filesystem>
#include <iostream>
#include <fstream>

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

extern FILE* DragonLog;

vector<char> read_file(filesystem::path path);

void write_file(filesystem::path path, vector<char> buffer);

inline void open_dragon_log() {
    if((DragonLog == nullptr || DragonLog == stdout) && !fopen_s(&DragonLog, "fmt_dragon.log", "w")) {
        DragonLog = nullptr;
    }
}

inline void close_dragon_log() {
    if(DragonLog != nullptr && DragonLog != stdout) {
        fclose(DragonLog);
    }
}

inline void flush_dragon_log() {
    if(DragonLog != nullptr) {
        fflush(DragonLog);
    }
}

inline void open_dragon_log_stdout() {
    if(DragonLog != nullptr) {
        flush_dragon_log();
        close_dragon_log();
    }
    DragonLog = stdout;
}

inline void write_dragon_log(const char* fmt, ...) {
    if(DragonLog != nullptr) {
        va_list args;
                va_start(args, fmt);
        vfprintf(DragonLog, fmt, args);
                va_end(args);
    }
}

inline void assert_dragon_log(bool check, const char* fmt, ...) {
    if(!check) {
        va_list args;
                va_start(args, fmt);
        vfprintf(DragonLog, fmt, args);
                va_end(args);
    }
}

template<class exception>
inline void super_assert_dragon_log(bool check, const char* error, const char* fmt, ...) {
    if(!check) {
        va_list args;
                va_start(args, fmt);
        vfprintf(DragonLog, fmt, args);
                va_end(args);
        flush_dragon_log();
        throw exception(error);
    }
}

#endif //FMT_DRAGON_DRAGON_H
