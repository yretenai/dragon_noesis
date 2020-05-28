//
// Created by yretenai on 5/31/2020.
//

#ifndef FMT_DRAGON_LUMBERYARDTEXTURE_H
#define FMT_DRAGON_LUMBERYARDTEXTURE_H

#include "../dragon.h"

#define FOURCC_DDS (MAKEFOURCC('D','D','S',' '))
#define FOURCC_FYRC (MAKEFOURCC('F','Y','R','C'))

class DRAGON_EXPORT LumberyardTexture {
public:
    LumberyardTexture(filesystem::path path);

    vector<char> cook();

    vector<char> Header;
    vector<vector<char>> Textures;

    static bool check(vector<char> buffer);

#ifdef USE_NOESIS

    static bool
    noesis_load([[maybe_unused]] BYTE* buffer, [[maybe_unused]] int length, CArrayList<noesisTex_t*> &noe_tex,
                noeRAPI_t* rapi);

    static bool noesis_check([[maybe_unused]] BYTE* buffer, [[maybe_unused]] int length, [[maybe_unused]] noeRAPI_t* rapi);

    static int noesis_tool([[maybe_unused]] int handle, [[maybe_unused]] void* user_data);

    static int noesis_tool_visibility([[maybe_unused]] int handle, const wchar_t* path, [[maybe_unused]] void* resv_a,
                                      [[maybe_unused]] void* resv_b);

#endif // USE_NOESIS
};


#endif //FMT_DRAGON_LUMBERYARDTEXTURE_H
