//
// Created by yretenai on 2020-06-09.
//

#include "ActorNodes.h"

namespace dragon::lumberyard::chunk::emfx {
    ActorNodes::ActorNodes(Array<char>* buffer, EMFX_CHUNK_HEADER header, int& ptr) {
        assert(header.Version <= 1);
        Chunk = header;
        ptr = Align(ptr, 4);
        Header = buffer->lpcast<ACTOR_NODES_V1_HEADER>(&ptr);
        ptr = Align(ptr, 4);
        Nodes = Array<std::shared_ptr<ActorNode>>(Header.NumNodes, nullptr);
        EMFX_CHUNK_HEADER nodeHeader = {static_cast<CHUNK_TYPE>(ACTOR_CHUNK_TYPE::Node), 0, header.Version};
        for (uint32_t i = 0; i < Header.NumNodes; i++) {
            Nodes[i] = std::shared_ptr<ActorNode>(new ActorNode(buffer, nodeHeader, ptr));
        }
    }
} // namespace dragon::lumberyard::chunk::emfx
