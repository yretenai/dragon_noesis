//
// Created by yretenai on 2020-06-09.
//

#include "ActorMesh.h"

namespace dragon::lumberyard::chunk::emfx {
    ActorMesh::ActorMesh(Array<char>* buffer, EMFX_CHUNK_HEADER header, int& ptr) {
        assert(header.Version <= 2);
        Chunk = header;
        Header = buffer->lpcast<ACTOR_MESH_V1_HEADER>(&ptr);

        VBOs = Array<std::shared_ptr<ActorVertexBuffer>>(Header.NumVBOs);
        Submeshes = Array<std::shared_ptr<ActorSubmesh>>(Header.NumSubMeshes);

        for (uint32_t i = 0; i < Header.NumVBOs; i++) {
            VBOs[i] = std::shared_ptr<ActorVertexBuffer>(new ActorVertexBuffer(buffer, header, Header, ptr));
        }

        for (uint32_t i = 0; i < Header.NumSubMeshes; i++) {
            Submeshes[i] = std::shared_ptr<ActorSubmesh>(new ActorSubmesh(buffer, header, ptr));
        }
    }
} // namespace dragon::lumberyard::chunk::emfx
