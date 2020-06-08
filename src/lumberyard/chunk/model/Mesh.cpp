//
// Created by yretenai on 6/5/2020.
//

#include "Mesh.h"

namespace dragon::lumberyard::chunk::model {
    Mesh::Mesh(Array<char>* buffer, CRCH_CHUNK_HEADER chunk_header) {
        Chunk = chunk_header;
        assert(Chunk.Version == 0x802);
        Header = buffer->cast<MESH_HEADER>(0);
    }
} // namespace dragon::lumberyard::chunk::model