//
// Created by yretenai on 5/31/2020.
//

#pragma once

#ifndef FMT_LUMBERYARD_TEXTURE_H
#define FMT_LUMBERYARD_TEXTURE_H

#include "../dragon/dragon.h"
#include "export.h"

#define FOURCC_DDS (MAKEFOURCC('D', 'D', 'S', ' '))
#define FOURCC_FYRC (MAKEFOURCC('F', 'Y', 'R', 'C'))

namespace dragon::lumberyard {
    class LUMBERYARD_EXPORT Texture {
      public:
        Texture(std::filesystem::path path);

        Array<char> cook();

        Array<char> Header;
        Array<char> Data;

        static bool check(Array<char>* buffer);

#ifdef USE_NOESIS

        static bool noesis_load([[maybe_unused]] BYTE* buffer, [[maybe_unused]] int length, CArrayList<noesisTex_t*>& noe_tex, noeRAPI_t* rapi);

        static bool noesis_check([[maybe_unused]] BYTE* buffer, [[maybe_unused]] int length, [[maybe_unused]] noeRAPI_t* rapi);

        static int noesis_tool([[maybe_unused]] int handle, [[maybe_unused]] void* user_data);

        static int noesis_tool_visibility([[maybe_unused]] int handle, const wchar_t* path, [[maybe_unused]] void* resv_a,
                                          [[maybe_unused]] void* resv_b);

#endif // USE_NOESIS
    };
} // namespace dragon::lumberyard

#endif // FMT_LUMBERYARD_TEXTURE_H
