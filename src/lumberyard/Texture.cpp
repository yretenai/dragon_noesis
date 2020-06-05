//
// Created by yretenai on 5/31/2020.
//

#include "Lumberyard.h"

namespace dragon::lumberyard {
    Texture::Texture(std::filesystem::path path) {
        std::vector<char> dds = read_file(path);
        bool is_alpha =
            path.has_extension() && path.extension().compare(".a") == 0;
        uint32_t* pointer = reinterpret_cast<uint32_t*>(dds.data());

        // get DDS Header data.
        uint32_t size = 0x80;
        if (pointer[21] == FOURCC_DX10)
            size += 0x14;
        Header = vector_slice(dds.data(), size);

        std::filesystem::path tmp(path);
        if (!is_alpha)
            tmp.replace_extension(".dds.1");
        char* buffer = new char[3];
        for (uint32_t i = pointer[7]; i > 0; i--) {
            // try and find color texture
            sprintf(buffer, "%d", i);
            tmp.replace_extension(buffer);
            if (is_alpha)
                tmp += "a";
            if (std::filesystem::exists(tmp)) {
                Data = read_file(tmp);
                break;
            }
        }
        delete[] buffer;
    }

    std::vector<char> Texture::cook() {
        if (Header.empty() || Data.empty())
            return std::vector<char>();
        std::vector<char> data(Header.size() + Data.size());
        char* pointer = data.data();
        // oh god.
        memcpy(pointer, Header.data(), Header.size());
        memcpy(pointer + Header.size(), Data.data(), Data.size());

        uint32_t* ints = reinterpret_cast<uint32_t*>(pointer);
        ints[7] = 1;  // dwMipCount
        ints[31] = 0; // dwReserved2 - FYRC
        return data;
    }

    bool Texture::check(std::vector<char> buffer) {
        uint32_t* pointer = reinterpret_cast<uint32_t*>(buffer.data());
        return buffer.size() >= 0x80 && pointer[0] == FOURCC_DDS &&
               pointer[1] == 0x7C && pointer[31] == FOURCC_FYRC;
    }

#if USE_NOESIS

    bool Texture::noesis_load([[maybe_unused]] BYTE* buffer,
                              [[maybe_unused]] int length,
                              CArrayList<noesisTex_t*>& noe_tex,
                              noeRAPI_t* rapi) {
        wchar_t* path = new wchar_t[MAX_NOESIS_PATH];
        g_nfn->NPAPI_GetSelectedFile(path);
        if (wcslen(path) < 2) {
            delete[] path;
            return 0;
        }
        std::filesystem::path wpath(path);
        delete[] path;
        if (!wpath.has_extension())
            return false;

        if (wpath.extension().compare(".a") != 0)
            wpath.replace_extension();

        Texture texture(wpath);

        std::vector<char> data = texture.cook();
        if (data.empty())
            return false;
        noesisTex_t* tex = rapi->Noesis_LoadTexByHandler(
            reinterpret_cast<BYTE*>(data.data()), data.size(), (char*)".dds");
        if (tex == nullptr)
            return false;
        noe_tex.Append(tex);

        return true;
    }

    bool Texture::noesis_check([[maybe_unused]] BYTE* buffer,
                               [[maybe_unused]] int length,
                               [[maybe_unused]] noeRAPI_t* rapi) {
        wchar_t* path = new wchar_t[MAX_NOESIS_PATH];
        g_nfn->NPAPI_GetSelectedFile(path);
        if (wcslen(path) < 2) {
            delete[] path;
            return 0;
        }

        return noesis_tool_visibility(0, path, nullptr, nullptr);
    }

    int Texture::noesis_tool([[maybe_unused]] int handle,
                             [[maybe_unused]] void* user_data) {
        wchar_t* path = new wchar_t[MAX_NOESIS_PATH];
        g_nfn->NPAPI_GetSelectedFile(path);
        if (wcslen(path) < 2) {
            delete[] path;
            return 0;
        }
        std::filesystem::path wpath(path);
        delete[] path;
        if (!wpath.has_extension())
            return 0;

        bool is_alpha = wpath.extension().compare(".a") == 0;
        if (!is_alpha)
            wpath.replace_extension();

        Texture texture(wpath);

        // handle color
        // handler is set to .dds.1, so strip .1
        wpath.replace_extension();
        if (is_alpha)
            wpath.replace_extension(".cooked-alpha.dds");
        else
            wpath.replace_extension(".cooked.dds");
        std::vector<char> data = texture.cook();
        if (data.empty())
            return 0;
        write_file(wpath, data);

        return 1;
    }

    int Texture::noesis_tool_visibility([[maybe_unused]] int handle,
                                        const wchar_t* path,
                                        [[maybe_unused]] void* resv_a,
                                        [[maybe_unused]] void* resv_b) {
        if (path == nullptr)
            return false;

        if (wcslen(path) < 7)
            return false;

        std::filesystem::path wpath(path);
        if (!wpath.has_extension())
            return false;

        // test.dds
        bool is_alpha = wpath.extension().compare(L".a") == 0;
        if (!is_alpha && wpath.extension().compare(L".1") != 0)
            return false;
        if (!is_alpha)
            wpath.replace_extension();

        std::vector<char> data = read_file(wpath);
        return Texture::check(data);
    }

#endif // USE_NOESIS
} // namespace dragon::lumberyard
