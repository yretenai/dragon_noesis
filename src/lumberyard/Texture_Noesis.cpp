//
// Created by yretenai on 5/31/2020.
//

#if USE_NOESIS

#include "Texture.h"

namespace dragon::lumberyard {
    bool Texture::noesis_load([[maybe_unused]] BYTE* buffer, [[maybe_unused]] int length, CArrayList<noesisTex_t*>& noe_tex, noeRAPI_t* rapi) {
        std::filesystem::path wpath(rapi->Noesis_GetInputNameW());
        if (!wpath.has_extension())
            return false;

        return noesies_load_direct(wpath, noe_tex, rapi);
    }

    bool Texture::noesies_load_direct(std::filesystem::path& imagePath, CArrayList<noesisTex_t*>& texList, const noeRAPI_t* rapi) {
        if (imagePath.extension().compare(".a") != 0)
            imagePath.replace_extension();

        Texture texture(imagePath);

        Array<char> data = texture.cook();
        if (data.empty())
            return false;
        noesisTex_t* tex = rapi->Noesis_LoadTexByHandler(reinterpret_cast<BYTE*>(data.data()), data.size(), (char*)".dds");
        if (tex == nullptr)
            return false;
        tex->filename = rapi->Noesis_PooledString(const_cast<char*>(imagePath.filename().string().c_str()));
        texList.Append(tex);

        return true;
    }

    bool Texture::noesis_check([[maybe_unused]] BYTE* buffer, [[maybe_unused]] int length, noeRAPI_t* rapi) {
        wchar_t* path = rapi->Noesis_GetInputNameW();
        bool test = noesis_tool_visibility(0, path, nullptr, nullptr);
        return test;
    }

    int Texture::noesis_tool([[maybe_unused]] int handle, [[maybe_unused]] void* user_data) {
        wchar_t path[MAX_NOESIS_PATH];
        g_nfn->NPAPI_GetSelectedFile(path);
        if (wcslen(path) < 2) {
            return 0;
        }
        std::filesystem::path wpath(path);
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
        Array<char> data = texture.cook();
        if (data.empty())
            return 0;
        write_file(wpath, &data);
        return 1;
    }

    int Texture::noesis_tool_visibility([[maybe_unused]] int handle, const wchar_t* path, [[maybe_unused]] void* resv_a,
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

        Array<char> data = read_file(wpath);
        return Texture::check(&data);
    }
} // namespace dragon::lumberyard

#endif // USE_NOESIS
