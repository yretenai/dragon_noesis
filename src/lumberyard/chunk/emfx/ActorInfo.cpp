//
// Created by yretenai on 2020-06-08.
//

#include "ActorInfo.h"

namespace dragon::lumberyard::chunk::emfx {
    ActorInfo::ActorInfo(dragon::Array<char>* buffer, dragon::lumberyard::chunk::emfx::EMFX_CHUNK_HEADER header, int& ptr) {
        assert(header.Version <= 3);
        Chunk = header;
        if (header.Version == 3) {
            ptr = Align(ptr, 4);
            Header = buffer->lpcast<ACTOR_INFO_V3_HEADER>(&ptr);
            ptr = Align(ptr, 4);
        } else if (header.Version == 2) {
            Header = buffer->lpcast<ACTOR_INFO_V2_HEADER>(&ptr);
        } else {
            Header = buffer->lpcast<ACTOR_INFO_V1_HEADER>(&ptr);
        }

        int32_t size = buffer->lpcast<int32_t>(&ptr);
        if (size > 0) {
            Array<char> stringBuffer = buffer->lpslice(&ptr, size);
            Source = std::string(stringBuffer.data(), size);
        }
        size = buffer->lpcast<int32_t>(&ptr);
        if (size > 0) {
            Array<char> stringBuffer = buffer->lpslice(&ptr, size);
            Filename = std::string(stringBuffer.data(), size);
        }
        size = buffer->lpcast<int32_t>(&ptr);
        if (size > 0) {
            Array<char> stringBuffer = buffer->lpslice(&ptr, size);
            Date = std::string(stringBuffer.data(), size);
        }
        size = buffer->lpcast<int32_t>(&ptr);
        if (size > 0) {
            Array<char> stringBuffer = buffer->lpslice(&ptr, size);
            Name = std::string(stringBuffer.data(), size);
        }
    }
} // namespace dragon::lumberyard::chunk::emfx
