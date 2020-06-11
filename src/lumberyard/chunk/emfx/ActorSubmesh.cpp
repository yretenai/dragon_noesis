//
// Created by yretenai on 2020-06-10.
//

#include "ActorSubmesh.h"

namespace dragon::lumberyard::chunk::emfx {
    ActorSubmesh::ActorSubmesh(Array<char>* buffer, EMFX_CHUNK_HEADER header, int& ptr) {
        assert(header.Version <= 2);
        Chunk = header;
        Header = buffer->lpcast<ACTOR_SUBMESH_V1_HEADER>(&ptr);

        Indices = buffer->lpcast<uint32_t>(&ptr, Header.NumIndices);
        Polygons = buffer->lpcast<uint8_t>(&ptr, Header.NumPolygons);
        Bones = buffer->lpcast<uint32_t>(&ptr, Header.NumBones);
    }
} // namespace dragon::lumberyard::chunk::emfx
