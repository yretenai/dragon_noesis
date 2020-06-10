//
// Created by yretenai on 2020-06-10.
//

#include "ActorMaterialAttributeValue.h"

namespace dragon::lumberyard::chunk::emfx {
    ActorMaterialAttributeValue::ActorMaterialAttributeValue(Array<char> *buffer, EMFX_CHUNK_HEADER header, int &ptr) {
        Chunk = header;
        Header = buffer->lpcast<ACTOR_MATERIAL_ATTRIBUTE_V1_HEADER>(&ptr);

        int32_t size = buffer->lpcast<int32_t>(&ptr);
        if (size > 0) {
            Array<char> stringBuffer = buffer->lpslice(&ptr, size);
            Name = std::string(stringBuffer.data(), size);
        }
        Buffer = buffer->lpslice(&ptr, Header.Size);
    }
}

