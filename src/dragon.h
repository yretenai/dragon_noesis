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

#ifndef FOURCC_DX10
#define FOURCC_DX10  (MAKEFOURCC('D','X','1','0'))
#endif

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

template<typename T>
inline vector<T> vector_cast_slice(char* ptr, int count) {
    return vector<T>(reinterpret_cast<T*>(ptr), reinterpret_cast<T*>(ptr + sizeof(T) * count));
}

template<typename T>
inline vector<T> vector_cast_slice(char** ptr, int count) {
    vector<T> data = vector_cast_slice<T>(*ptr, count);
    *ptr += sizeof(T) * count;
    return data;
}

inline vector<char> vector_slice(char* ptr, int count) {
    return vector<char>(ptr, ptr + count);
}

inline vector<char> vector_slice(char** ptr, int count) {
    vector<char> data = vector_slice(*ptr, count);
    *ptr += count;
    return data;
}

template<typename T>
inline T vector_cast(char* ptr, int index = 0) {
    return reinterpret_cast<T*>(ptr)[index];
}

template<typename T>
inline T vector_cast(char** ptr, int index = 0) {
    T data = vector_cast<T>(*ptr, index);
    *ptr += sizeof(T);
    return data;
}

inline vector<char> read_file(filesystem::path path) {
    ifstream file(path, ios::binary | ios::ate | ios::in);
    uint32_t pos = file.tellg();
    std::vector<char> bytes(pos);
    file.seekg(0, ios::beg);
    file.read(bytes.data(), pos);
    return bytes;
}

inline void write_file(filesystem::path path, vector<char> buffer) {
    if(buffer.empty())
        return;
    ofstream file(path, ios::binary | ios::out | ios::trunc);
    file.write(buffer.data(), buffer.size());
}

DRAGON_EXPORT void open_dragon_log();

DRAGON_EXPORT void close_dragon_log();

DRAGON_EXPORT void flush_dragon_log();

DRAGON_EXPORT void open_dragon_log_stdout();

DRAGON_EXPORT void write_dragon_log(const char* fmt, ...);

DRAGON_EXPORT void assert_dragon_log(bool check, const char* fmt, ...);

DRAGON_EXPORT void super_assert_dragon_log(bool check, const char* error, const char* fmt, ...);

#endif //FMT_DRAGON_DRAGON_H
