//
// Created by yretenai on 5/31/2020.
//

#include "Texture.h"

namespace dragon::lumberyard {
    Texture::Texture() {}

    Texture::Texture(std::filesystem::path path) {
        Array<char> dds = read_file(path);
        bool is_alpha = path.has_extension() && path.extension().compare(".a") == 0;
        uint32_t* pointer = reinterpret_cast<uint32_t*>(dds.data());

        // get DDS Header data.
        uint32_t size = 0x80;
        if (pointer[21] == FOURCC_DX10)
            size += 0x14;
        Header = dds.slice(0, size);

        std::filesystem::path tmp(path);
        if (!is_alpha)
            tmp.replace_extension(".dds.1");
        char buffer[3];
        for (uint32_t i = pointer[7]; i > 0; i--) {
            // try and find color texture
            sprintf(buffer, "%d", i);
            tmp.replace_extension(buffer);
            if (is_alpha)
                tmp += "a";
            if (std::filesystem::is_regular_file(tmp)) {
                Data = read_file(tmp);
                break;
            }
        }
    }

    Array<char> Texture::cook() {
        if (Header.empty() || Data.empty())
            return Array<char>();
        Array<char> data(Header.size() + Data.size(), nullptr);
        char* pointer = data.data();
        // oh god.
        memcpy(pointer, Header.data(), Header.size());
        memcpy(pointer + Header.size(), Data.data(), Data.size());

        uint32_t* ints = reinterpret_cast<uint32_t*>(pointer);
        ints[7] = 1;  // dwMipCount
        ints[31] = 0; // dwReserved2 - FYRC
        return data;
    }

    bool Texture::check(Array<char>* buffer) {
        uint32_t* pointer = reinterpret_cast<uint32_t*>(buffer->data());
        return buffer->size() >= 0x80 && pointer[0] == FOURCC_DDS && pointer[1] == 0x7C && pointer[31] == FOURCC_FYRC;
    }
} // namespace dragon::lumberyard
