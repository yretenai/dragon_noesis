//
// Created by yretenai on 2020-06-10.
//

#include "ActorVertexBuffer.h"

namespace dragon::lumberyard::chunk::emfx {
    ActorVertexBuffer::ActorVertexBuffer(Array<char> *buffer, EMFX_CHUNK_HEADER header, ACTOR_MESH_V1_HEADER mesh, int &ptr) {
        assert(header.Version <= 2);
        Chunk = header;
        ptr = Align(ptr, 4);
        Header = buffer->lpcast<ACTOR_VBO_V1_HEADER>(&ptr);
        ptr = Align(ptr, 4);
        int32_t size = buffer->lpcast<int32_t>(&ptr);
        if (size > 0) {
            Array<char> stringBuffer = buffer->lpslice(&ptr, size);
            Name = std::string(stringBuffer.data(), size);
        }
        Buffer = buffer->lpslice(&ptr, Header.AttribSizeInBytes * mesh.TotalVerts);
    }
}
