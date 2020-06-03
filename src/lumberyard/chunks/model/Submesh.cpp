//
// Created by yretenai on 6/3/2020.
//

#include "Submesh.h"

Submesh::Submesh(vector<char> buffer) {
    char* ptr = buffer.data();
    Header = vector_cast<SUBMESH_HEADER>(&ptr);
    Submeshes = vector_cast_slice<SUBMESH_DATA>(&ptr, Header.Count);
    if((Header.Flags & SUBMESH_HEADER::BoneIndices) == SUBMESH_HEADER::BoneIndices) {
        Bones = vector_cast_slice<SUBMESH_BONE>(&ptr, Header.Count);
    }
    if((Header.Flags & SUBMESH_HEADER::SubsetTexelDensity) == SUBMESH_HEADER::SubsetTexelDensity) {
        TexelDensity = vector_cast_slice<float>(&ptr, Header.Count);
    }
}
