//
// Created by yretenai on 2020-06-10.
//

#include "ActorMaterialAttributeSet.h"

namespace dragon::lumberyard::chunk::emfx {
    ActorMaterialAttributeSet::ActorMaterialAttributeSet(Array<char>* buffer, EMFX_CHUNK_HEADER header, int& ptr) {
        assert(header.Version <= 1);
        Chunk = header;
        ptr = Align(ptr, 4);
        Header = buffer->lpcast<ACTOR_MATERIAL_ATTRIBUTE_SET_V1_HEADER>(&ptr);
        // ptr = Align(ptr, 4); - We merged two headers :       ^)
        Attributes = Array<std::shared_ptr<ActorMaterialAttribute>>(Header.NumAttributes);
        for (uint32_t i = 0; i < Header.NumAttributes; i++) {
            Attributes[i] = std::shared_ptr<ActorMaterialAttribute>(new ActorMaterialAttribute(buffer, header, ptr));
        }
    }
} // namespace dragon::lumberyard::chunk::emfx
