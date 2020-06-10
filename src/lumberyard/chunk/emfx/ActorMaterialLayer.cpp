//
// Created by yretenai on 2020-06-09.
//

#include "ActorMaterialLayer.h"

namespace dragon::lumberyard::chunk::emfx {
    ActorMaterialLayer::ActorMaterialLayer(Array<char>* buffer, EMFX_CHUNK_HEADER header, int& ptr) {
        assert(header.Version <= 1);
        Chunk = header;
        ptr = Align(ptr, 4);
        Header = buffer->lpcast<ACTOR_MATERIAL_LAYER_V1_HEADER>(&ptr);
        ptr = Align(ptr, 4);
        int32_t size = buffer->lpcast<int32_t>(&ptr);
        if (size > 0) {
            Array<char> stringBuffer = buffer->lpslice(&ptr, size);
            Name = std::string(stringBuffer.data(), size);
        }
    }
} // namespace dragon::lumberyard::chunk::emfx
