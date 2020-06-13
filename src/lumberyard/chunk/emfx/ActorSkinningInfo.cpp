//
// Created by yretenai on 2020-06-10.
//

#include "ActorSkinningInfo.h"

#include "ActorMesh.h"

#define CAST_ABSTRACT_CHUNK(target, chunk) (std::reinterpret_pointer_cast<target>(chunk).get())

namespace dragon::lumberyard::chunk::emfx {
    ActorSkinningInfo::ActorSkinningInfo(Array<char>* buffer, EMFX_CHUNK_HEADER header,
                                         std::vector<std::shared_ptr<chunk::emfx::AbstractEMFXChunk>> meshChunks, int& ptr) {
        assert(header.Version <= 1);
        Chunk = header;
        ptr = Align(ptr, 4);
        Header = buffer->lpcast<ACTOR_SKINNING_INFO_v1_HEADER>(&ptr);
        ptr = Align(ptr, 4);

        ActorMesh* mesh = nullptr;
        for (std::shared_ptr<chunk::emfx::AbstractEMFXChunk> chunk : meshChunks) {
            ActorMesh* meshPtr = CAST_ABSTRACT_CHUNK(ActorMesh, chunk);
            if (meshPtr != nullptr && meshPtr->Header.NodeIndex == Header.NodeIndex && meshPtr->Header.LOD == Header.LOD) {
                mesh = meshPtr;
                break;
            }
        }

        if (mesh == nullptr) {
            LOG("Could not find related mesh for SkinningInfo#" << Header.NodeIndex << "!");
            ptr = Chunk.Size;
            return;
        }

        Influences = buffer->lpcast<ACTOR_SKINNING_INFO_v1_INFLUENCE>(&ptr, Header.NumTotalInfluences);
        ptr = Align(ptr, 4);
        Table = buffer->lpcast<ACTOR_SKINNING_INFO_v1_ENTRY>(&ptr, mesh->Header.NumOriginalVerts);
    }
} // namespace dragon::lumberyard::chunk::emfx
