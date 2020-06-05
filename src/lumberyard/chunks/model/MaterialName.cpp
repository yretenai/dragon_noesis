//
// Created by yretenai on 6/2/2020.
//

#include "MaterialName.h"

namespace dragon::lumberyard::chunk::model {
    MaterialName::MaterialName(std::vector<char> buffer, uint32_t version) {
        super_assert_dragon_log(version == 0x802, "version == 0x802");
        char* ptr = buffer.data();
        Name = std::string(ptr, ptr + 0x80);
        ptr += 0x80;
        uint32_t count = vector_cast<uint32_t>(&ptr);
        Types = vector_cast_slice<int32_t>(&ptr, count);
        Materials = std::vector<std::string>(count);
        if (count == 1) {
            Materials[0] = Name;
        } else {
            for (uint32_t i = 0; i < count; i++) {
                std::string material(ptr);
                Materials[i] = material;
                ptr += material.size() + 1;
            }
        }
    }
} // namespace dragon::lumberyard::chunk::model
