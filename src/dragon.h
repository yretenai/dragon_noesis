//
// Created by yretenai on 5/28/2020.
//

#pragma once

#ifndef FMT_DRAGON_DRAGON_H
#define FMT_DRAGON_DRAGON_H

#include "export.h"
#include "Array.h"
#include <cstdint>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>

#ifndef MAKEFOURCC
#define MAKEFOURCC(ch0, ch1, ch2, ch3)                                         \
    ((uint32_t)(uint8_t)(ch0) | ((uint32_t)(uint8_t)(ch1) << 8) |              \
     ((uint32_t)(uint8_t)(ch2) << 16) | ((uint32_t)(uint8_t)(ch3) << 24))
#endif // MAKEFOURCC

#ifndef FOURCC_DX10
#define FOURCC_DX10 (MAKEFOURCC('D', 'X', '1', '0'))
#endif

#ifdef WIN32
#ifdef USE_NOESIS

#include "noesis/pluginshare.h"

#else // USE_NOESIS

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#endif // USE_NOESIS
#endif

namespace dragon {
    inline Array<char> read_file(std::filesystem::path path) {
        std::ifstream file(path, std::ios::binary | std::ios::in);
        uint32_t size = (uint32_t) std::filesystem::file_size(path);
        Array<char> bytes(size);
        file.seekg(0, std::ios::beg);
        file.read(bytes.data(), size);
        return bytes;
    }

    inline void write_file(std::filesystem::path path,
                           Array<char>* buffer) {
        if (buffer->empty())
            return;
        std::ofstream file(path,
                           std::ios::binary | std::ios::out | std::ios::trunc);
        file.write(buffer->data(), buffer->size());
    }

    DRAGON_EXPORT void open_dragon_log();

    DRAGON_EXPORT void close_dragon_log();

    DRAGON_EXPORT void flush_dragon_log();

    DRAGON_EXPORT void open_dragon_log_stdout();

    DRAGON_EXPORT void write_dragon_log(const char *fmt, ...);

    DRAGON_EXPORT void assert_dragon_log(bool check, const char *error);

    DRAGON_EXPORT void super_assert_dragon_log(bool check, const char *error);
} // namespace dragon

#endif // FMT_DRAGON_DRAGON_H
