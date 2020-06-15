//
// Created by yretenai on 5/28/2020.
//

#pragma once

#ifndef DRAGON_DRAGON_H
#define DRAGON_DRAGON_H

#define DRAGON_VERSION "1.0.2"

#include "Array.h"
#include <assert.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#ifndef MAKEFOURCC
#define MAKEFOURCC(ch0, ch1, ch2, ch3) \
    ((uint32_t)(uint8_t)(ch0) | ((uint32_t)(uint8_t)(ch1) << 8) | ((uint32_t)(uint8_t)(ch2) << 16) | ((uint32_t)(uint8_t)(ch3) << 24))
#endif // MAKEFOURCC

#ifndef FOURCC_DX10
#define FOURCC_DX10 (MAKEFOURCC('D', 'X', '1', '0'))
#endif

#ifndef DRAGON_LIBRARY_NAME
#define DRAGON_LIBRARY_NAME "dragon"
#endif

extern std::ofstream* LogStream;

#ifdef _WIN32
#ifdef USE_NOESIS

#include "../noesis/pluginshare.h"

#else // USE_NOESIS

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#endif // USE_NOESIS
#endif

#ifndef __PRETTY_FUNCTION__
#define __PRETTY_FUNCTION__ __FUNCTION__
#endif

#ifdef USE_NOESIS
#define LOG(msg)                                                                                    \
    do {                                                                                            \
        std::stringstream s;                                                                        \
        s << "[" << DRAGON_LIBRARY_NAME << "][" << __PRETTY_FUNCTION__ << "] " << msg << std::endl; \
        if (LogStream != nullptr)                                                                   \
            (*LogStream << s.str() << std::flush);                                                  \
        if (g_nfn != nullptr && g_nfn->NPAPI_DebugLogIsOpen())                                      \
            g_nfn->NPAPI_DebugLogStr(const_cast<char*>(s.str().c_str()));                           \
        (std::cout << s.str() << std::flush);                                                       \
    } while (0)
#else
#define LOG(msg)                                                                                                                              \
    ((LogStream != nullptr ? *LogStream : std::cout) << "[" << DRAGON_LIBRARY_NAME << "][" << __PRETTY_FUNCTION__ << "] " << msg << std::endl \
                                                     << std::flush)
#endif

namespace dragon {
    inline int Align(int value, int align) {
        int v = value % align;
        if (v != 0)
            return value + align - v;
        return value;
    }

    inline Array<char> read_file_internal(std::filesystem::path path) {
        std::ifstream file(path, std::ios::binary | std::ios::in);
        uint32_t size = (uint32_t)std::filesystem::file_size(path);
        Array<char> bytes(size);
        file.seekg(0, std::ios::beg);
        file.read(bytes.data(), size);
        return bytes;
    }

    inline void write_file_internal(std::filesystem::path path, Array<char>* buffer) {
        if (buffer->empty())
            return;
        std::ofstream file(path, std::ios::binary | std::ios::out | std::ios::trunc);
        file.write(buffer->data(), buffer->size());
    }

#ifdef USE_NOESIS

    inline Array<char> read_file(std::filesystem::path path) {
        if (g_nfn == nullptr) {
            return read_file_internal(path);
        }
        noeRAPI_t* rapi = g_nfn->NPAPI_GetPreviewRAPI();
        int moduleHandle = -1;
        if (rapi == nullptr) {
            moduleHandle = g_nfn->NPAPI_InstantiateModule(nullptr);
            rapi = g_nfn->NPAPI_GetModuleRAPI(moduleHandle);
        }
        if (rapi == nullptr) {
            if (moduleHandle > -1) {
                g_nfn->NPAPI_FreeModule(moduleHandle);
            }
            return read_file_internal(path);
        }

        std::string spath = path.string();
        wchar_t* wpath = const_cast<wchar_t*>(path.c_str());
        char* cpath = const_cast<char*>(spath.c_str());
        if (!rapi->Noesis_FileExists(cpath)) {
            if (moduleHandle > -1) {
                g_nfn->NPAPI_FreeModule(moduleHandle);
            }
            return Array<char>(0);
        }
        int32_t size;
        BYTE* ptr = rapi->Noesis_ReadFileW(wpath, &size);
        if (ptr == nullptr) {
            if (moduleHandle > -1) {
                g_nfn->NPAPI_FreeModule(moduleHandle);
            }
            return Array<char>(0);
        }
        Array<char> bytes(reinterpret_cast<char*>(ptr), size);
        rapi->Noesis_UnpooledFree(ptr);
        if (moduleHandle > -1) {
            g_nfn->NPAPI_FreeModule(moduleHandle);
        }
        return bytes;
    }

    inline void write_file(std::filesystem::path path, Array<char>* buffer) {
        if (g_nfn == nullptr) {
            write_file_internal(path, buffer);
            return;
        }
        noeRAPI_t* rapi = g_nfn->NPAPI_GetPreviewRAPI();
        int moduleHandle = -1;
        auto noesis_log = [](char* ch) { LOG(ch); };
        if (rapi == nullptr) {
            moduleHandle = g_nfn->NPAPI_InstantiateModule(noesis_log);
            rapi = g_nfn->NPAPI_GetModuleRAPI(moduleHandle);
        }
        if (rapi == nullptr) {
            if (moduleHandle > -1) {
                g_nfn->NPAPI_FreeModule(moduleHandle);
            }
            write_file_internal(path, buffer);
            return;
        }

        if (buffer->empty()) {
            if (moduleHandle > -1) {
                g_nfn->NPAPI_FreeModule(moduleHandle);
            }
            write_file_internal(path, buffer);
            return;
        }

        wchar_t* wpath = const_cast<wchar_t*>(path.c_str());
        rapi->Noesis_WriteFileW(wpath, buffer->data(), buffer->byte_size());
        if (moduleHandle > -1) {
            g_nfn->NPAPI_FreeModule(moduleHandle);
        }
    }
#else
    inline Array<char> read_file(std::filesystem::path path) { return read_file_internal(path); }

    inline void write_file(std::filesystem::path path, Array<char>* buffer) { write_file_internal(path, buffer); }
#endif

    class not_implemented_exception : public std::exception {};
} // namespace dragon

#endif // DRAGON_DRAGON_H
