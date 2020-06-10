//
// Created by yretenai on 5/28/2020.
//

#include "Actor.h"

#include "globals.h"
#include <memory>

#define CAST_EMFX_CHUNK(chunk) (std::shared_ptr<AbstractEMFXChunk>(reinterpret_cast<AbstractEMFXChunk*>(chunk)))

using namespace dragon::lumberyard::chunk::emfx;

namespace dragon::lumberyard {
    Actor::Actor(Array<char>* baseBuffer) {
        assert(check(baseBuffer));

        Header = baseBuffer->cast<ACTOR_HEADER>(0);
        int ptr = 0;
        Array<char> buffer = baseBuffer->slice(sizeof(ACTOR_HEADER), baseBuffer->size() - sizeof(ACTOR_HEADER));
        while (ptr < buffer.size()) {
            EMFX_CHUNK_HEADER chunkHeader = buffer.lpcast<EMFX_CHUNK_HEADER>(&ptr);
            if (chunkHeader.Type >= (CHUNK_TYPE)ACTOR_CHUNK_TYPE::END) {
                LOG("Chunk Id is out range, aborting.");
                return;
            }
            Array<char> slice = buffer.shift(ptr);
            int localPtr = 0;
            switch ((ACTOR_CHUNK_TYPE)chunkHeader.Type) {
            case ACTOR_CHUNK_TYPE::Mesh:
                LOG("Found Mesh");
                Chunks.push_back(CAST_EMFX_CHUNK(new ActorMesh(&slice, chunkHeader, localPtr)));
                break;
            case ACTOR_CHUNK_TYPE::SkinningInfo: {
                LOG("Found SkinningInfo");
                std::vector<std::shared_ptr<chunk::emfx::AbstractEMFXChunk>> meshChunks;
                get_chunks_of_type(ACTOR_CHUNK_TYPE::Mesh, &meshChunks);
                Chunks.push_back(CAST_EMFX_CHUNK(new ActorSkinningInfo(&slice, chunkHeader, meshChunks, localPtr)));
                break;
            }
            case ACTOR_CHUNK_TYPE::Material:
                LOG("Found Material");
                Chunks.push_back(CAST_EMFX_CHUNK(new ActorMaterial(&slice, chunkHeader, localPtr)));
                break;
            case ACTOR_CHUNK_TYPE::Info:
                LOG("Found Info");
                Chunks.push_back(CAST_EMFX_CHUNK(new ActorInfo(&slice, chunkHeader, localPtr)));
                break;
            case ACTOR_CHUNK_TYPE::MeshLOD:
                LOG("Found MeshLOD");
                throw not_implemented_exception();
            case ACTOR_CHUNK_TYPE::Nodes:
                LOG("Found Nodes");
                Chunks.push_back(CAST_EMFX_CHUNK(new ActorNodes(&slice, chunkHeader, localPtr)));
                break;
            case ACTOR_CHUNK_TYPE::MaterialInfo:
                LOG("Found MaterialInfo");
                Chunks.push_back(CAST_EMFX_CHUNK(new ActorMaterialInfo(&slice, chunkHeader, localPtr)));
                break;
            case ACTOR_CHUNK_TYPE::NodeMotionSources:
                LOG("Found NodeMotionSources");
                Chunks.push_back(CAST_EMFX_CHUNK(new ActorNodeMotionSources(&slice, chunkHeader, localPtr)));
                break;
            case ACTOR_CHUNK_TYPE::MaterialAttributeSet:
                LOG("Found MaterialAttributeSet");
                    Chunks.push_back(CAST_EMFX_CHUNK(new ActorMaterialAttributeSet(&slice, chunkHeader, localPtr)));
                break;
            default:
                LOG("Unhandled chunk type " << chunkHeader.Type);
                localPtr = chunkHeader.Size;
                    break;
            }

            ptr += localPtr;
        }
    }

    void Actor::get_chunks_of_type(ACTOR_CHUNK_TYPE type, std::vector<std::shared_ptr<AbstractEMFXChunk>>* chunks) {
        for (uint32_t i = 0; i < Chunks.size(); i++) {
            if (static_cast<ACTOR_CHUNK_TYPE>(Chunks[i].get()->Chunk.Type) == type) {
                chunks->push_back(Chunks[i]);
            }
        }
    }

    std::shared_ptr<AbstractEMFXChunk> Actor::get_chunk(ACTOR_CHUNK_TYPE type) {
        for (uint32_t i = 0; i < Chunks.size(); i++) {
            if (static_cast<ACTOR_CHUNK_TYPE>(Chunks[i].get()->Chunk.Type) == type) {
                return Chunks[i];
            }
        }
        return nullptr;
    }

    bool Actor::check(Array<char>* buffer) {
        if (buffer->size() < sizeof(ACTOR_HEADER))
            return false;
        ACTOR_HEADER header = buffer->cast<ACTOR_HEADER>(0);
        return buffer->size() >= sizeof(ACTOR_HEADER) && header.Magic == FOURCC_ACTR && header.IsBigEndian == 0;
    }
} // namespace dragon::lumberyard
