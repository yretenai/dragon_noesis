//
// Created by yretenai on 6/2/2020.
//

#include "MaterialName.h"

namespace dragon::lumberyard::chunk::model {
    MaterialName::MaterialName(Array<char>* buffer, CRCH_CHUNK_HEADER chunk_header) {
        Chunk = chunk_header;
        assert(Chunk.Version == 0x802);
        char* ptr = buffer->data();
        Name = std::string(ptr, ptr + 0x80);
        int offset = 0x80;
        uint32_t count = buffer->lpcast<uint32_t>(&offset);
        Types = buffer->lpcast<int32_t>(&offset, count);
        Materials = Array<std::string>(count);
        if (count == 1) {
            Materials[0] = Name;
        } else {
            ptr += offset;
            for (uint32_t i = 0; i < count; i++) {
                std::string material(ptr);
                Materials[i] = material;
                ptr += material.size() + 1;
            }
        }
    }
} // namespace dragon::lumberyard::chunk::model
