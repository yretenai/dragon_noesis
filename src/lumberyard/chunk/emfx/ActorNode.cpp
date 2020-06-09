//
// Created by yretenai on 2020-06-09.
//

#include "ActorNode.h"

namespace dragon::lumberyard::chunk::emfx {
    ActorNode::ActorNode(Array<char>* buffer, EMFX_CHUNK_HEADER header, int& ptr) {
        assert(header.Version <= 1);
        Chunk = header;
        Header = buffer->lpcast<ACTOR_NODE_V1_HEADER>(&ptr);
        int32_t size = buffer->lpcast<int32_t>(&ptr);
        if (size > 0) {
            Array<char> stringBuffer = buffer->lpslice(&ptr, size);
            Name = std::string(stringBuffer.data(), size);
        }
    }
} // namespace dragon::lumberyard::chunk::emfx
