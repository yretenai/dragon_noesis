//
// Created by yretenai on 5/31/2020.
//

#include "Lumberyard.h"

LumberyardTexture::LumberyardTexture(filesystem::path path) {

}

vector<char> LumberyardTexture::cook() {
    if(Header.empty())
        return vector<char>();
}

bool LumberyardTexture::check(vector<char> buffer) {
    uint32_t* pointer = reinterpret_cast<uint32_t*>(buffer.data());
    return buffer.size() >= 0x80 && pointer[0] == FOURCC_DDS && pointer[1] == 0x7C && pointer[31] == FOURCC_FYRC;
}

#if USE_NOESIS

bool LumberyardTexture::noesis_load([[maybe_unused]] BYTE* buffer, [[maybe_unused]] int length,
                                    CArrayList<noesisTex_t*> &noe_tex, noeRAPI_t* rapi) {
    wchar_t* path = new wchar_t[MAX_NOESIS_PATH];
    g_nfn->NPAPI_GetSelectedFile(path);
    if(wcslen(path) < 2) {
        delete[] path;
        return 0;
    }
    filesystem::path wpath(path);
    delete[] path;
    wpath.replace_extension();
    LumberyardTexture texture(wpath);
    vector<char> data = texture.cook();
    if(data.empty())
        return false;
    noesisTex_t* tex = rapi->Noesis_LoadTexByHandler(reinterpret_cast<BYTE*>(data.data()), data.size(), (char*) ".dds");
    if(tex == nullptr)
        return false;
    noe_tex.Append(tex);
    return true;
}

bool LumberyardTexture::noesis_check([[maybe_unused]] BYTE* buffer, [[maybe_unused]] int length, [[maybe_unused]] noeRAPI_t* rapi) {
    wchar_t* path = new wchar_t[MAX_NOESIS_PATH];
    g_nfn->NPAPI_GetSelectedFile(path);
    if(wcslen(path) < 2) {
        delete[] path;
        return 0;
    }
    return noesis_tool_visibility(0, path, nullptr, nullptr);
}

int LumberyardTexture::noesis_tool([[maybe_unused]] int handle, [[maybe_unused]] void* user_data) {
    wchar_t* path = new wchar_t[MAX_NOESIS_PATH];
    g_nfn->NPAPI_GetOpenPreviewFile(path);
    if(wcslen(path) < 2) {
        delete[] path;
        return 0;
    }
    filesystem::path wpath(path);
    delete[] path;
    // handler is set to .dds.1, so strip .1
    wpath.replace_extension();
    LumberyardTexture texture(wpath);
    wpath.replace_extension(".merged.dds");
    vector<char> data = texture.cook();
    if(data.empty())
        return 0;
    return 0;
}

int LumberyardTexture::noesis_tool_visibility([[maybe_unused]] int handle, const wchar_t* path,
                                              [[maybe_unused]] void* resv_a, [[maybe_unused]] void* resv_b) {
    if(path == nullptr)
        return false;
    if(wcslen(path) < 7)
        return false;
    filesystem::path wpath(path);
    if(!wpath.has_extension())
        return false;
    // test.dds
    if(wpath.extension().compare(L".dds") != 0 && wpath.extension().compare(L".DDS") != 0) {
        wpath.replace_extension();
        // test.dds.1
        if(wpath.extension().compare(L".dds") != 0 && wpath.extension().compare(L".DDS") != 0)
            return false;
    }
    vector<char> data = read_file(wpath);
    return LumberyardTexture::check(data);
}

#endif // USE_NOESIS
