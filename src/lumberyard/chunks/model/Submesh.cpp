//
// Created by yretenai on 6/3/2020.
//

#include "Submesh.h"

namespace dragon::lumberyard::chunk::model {
    Submesh::Submesh(Array<char>* buffer, CRCH_CHUNK_HEADER chunk_header) {
        Chunk = chunk_header;
        super_assert_dragon_log(Chunk.Version == 0x800, "version == 0x800");
        int ptr = 0;
        Header = buffer->lpcast<SUBMESH_HEADER>(&ptr);
        Submeshes = buffer->lpcast<SUBMESH_DATA>(&ptr, Header.Count);
        if ((Header.Flags & SUBMESH_HEADER::BoneIndices) ==
            SUBMESH_HEADER::BoneIndices) {
            Bones = buffer->lpcast<SUBMESH_BONE>(&ptr, Header.Count);
        }
        if ((Header.Flags & SUBMESH_HEADER::SubsetTexelDensity) ==
            SUBMESH_HEADER::SubsetTexelDensity) {
            TexelDensity = buffer->lpcast<float>(&ptr, Header.Count);
        }
    }
} // namespace dragon::lumberyard::chunk::model
