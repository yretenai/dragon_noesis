//
// Created by yretenai on 2020-06-09.
//

#include "ActorMaterial.h"

namespace dragon::lumberyard::chunk::emfx {
    ActorMaterial::ActorMaterial(Array<char>* buffer, EMFX_CHUNK_HEADER header, int& ptr) {
        assert(header.Version <= 1);
        Chunk = header;
        Header = buffer->lpcast<ACTOR_MATERIAL_V1_HEADER>(&ptr);
        Layers = Array<std::shared_ptr<ActorMaterialLayer>>(Header.NumLayers);
        int32_t size = buffer->lpcast<int32_t>(&ptr);
        if (size > 0) {
            Array<char> stringBuffer = buffer->lpslice(&ptr, size);
            Name = std::string(stringBuffer.data(), size);
        }
        EMFX_CHUNK_HEADER nodeHeader = {static_cast<CHUNK_TYPE>(ACTOR_CHUNK_TYPE::MaterialLayer), 0, header.Version};
        for (uint32_t i = 0; i < Header.NumLayers; i++) {
            Layers[i] = std::shared_ptr<ActorMaterialLayer>(new ActorMaterialLayer(buffer, nodeHeader, ptr));
        }
    }
} // namespace dragon::lumberyard::chunk::emfx
