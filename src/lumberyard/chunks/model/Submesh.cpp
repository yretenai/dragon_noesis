//
// Created by yretenai on 6/3/2020.
//

#include "Submesh.h"

namespace dragon::lumberyard::chunk::model {
    Submesh::Submesh(Array<char>* buffer, CRCH_CHUNK_HEADER chunk_header) {
        Chunk = chunk_header;
        assert(Chunk.Version == 0x800);
        int ptr = 0;
        Header = buffer->lpcast<SUBMESH_HEADER>(&ptr);
        Submeshes = buffer->lpcast<SUBMESH_DATA>(&ptr, Header.Count);
        if (((uint32_t)Header.Flags &
             (uint32_t)SUBMESH_HEADER::FLAGS::BoneIndices) ==
            (uint32_t)SUBMESH_HEADER::FLAGS::BoneIndices) {
            Bones = buffer->lpcast<SUBMESH_BONE>(&ptr, Header.Count);
        }
        if (((uint32_t)Header.Flags &
             (uint32_t)SUBMESH_HEADER::FLAGS::SubsetTexelDensity) ==
            (uint32_t)SUBMESH_HEADER::FLAGS::SubsetTexelDensity) {
            TexelDensity = buffer->lpcast<float>(&ptr, Header.Count);
        }
    }
} // namespace dragon::lumberyard::chunk::model
