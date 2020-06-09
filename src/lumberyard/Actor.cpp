//
// Created by yretenai on 5/28/2020.
//

#include "Actor.h"

#include "globals.h"
#include <memory>

#define CAST_EMFX_CHUNK(chunk) (std::shared_ptr<AbstractEMFXChunk>(reinterpret_cast<AbstractEMFXChunk*>(chunk)))

using namespace dragon::lumberyard::chunk::emfx;

namespace dragon::lumberyard {
    Actor::Actor(Array<char>* buffer) {
        assert(check(buffer));

        Header = buffer->cast<ACTOR_HEADER>(0);
        int ptr = sizeof(ACTOR_HEADER);
        while (ptr < buffer->size()) {
            EMFX_CHUNK_HEADER chunkHeader = buffer->lpcast<EMFX_CHUNK_HEADER>(&ptr);
            int expectedPtr = ptr + chunkHeader.Size;
            switch ((ACTOR_CHUNK_TYPE)chunkHeader.Type) {
            case ACTOR_CHUNK_TYPE::Mesh:
                LOG("Found Mesh");
                throw not_implemented_exception();
            case ACTOR_CHUNK_TYPE::SkinningInfo:
                LOG("Found SkinningInfo");
                throw not_implemented_exception();
            case ACTOR_CHUNK_TYPE::Material:
                LOG("Found Material");
                ptr += chunkHeader.Size;
                break;
            case ACTOR_CHUNK_TYPE::Info:
                LOG("Found Info");
                Chunks.push_back(CAST_EMFX_CHUNK(new ActorInfo(buffer, chunkHeader, ptr)));
                break;
            case ACTOR_CHUNK_TYPE::MeshLOD:
                LOG("Found MeshLOD");
                throw not_implemented_exception();
            case ACTOR_CHUNK_TYPE::Nodes:
                LOG("Found Nodes");
                Chunks.push_back(CAST_EMFX_CHUNK(new ActorNodes(buffer, chunkHeader, ptr)));
                return;
            case ACTOR_CHUNK_TYPE::MaterialInfo:
                LOG("Found MaterialInfo");
                ptr += chunkHeader.Size;
                break;
            case ACTOR_CHUNK_TYPE::NodeMotionSources:
                LOG("Found NodeMotionSources");
                Chunks.push_back(CAST_EMFX_CHUNK(new ActorNodeMotionSources(buffer, chunkHeader, ptr)));
                break;
            case ACTOR_CHUNK_TYPE::MaterialAttributeSet:
                LOG("Found MaterialAttributeSet");
                ptr += chunkHeader.Size;
                break;
            default:
                LOG("Unhandled chunk type " << chunkHeader.Type);
                ptr += chunkHeader.Size;
            }

            if (ptr != expectedPtr) {
                LOG("WARNING! Expected to be at pos " << expectedPtr << " but we're at " << ptr << " behavior might be unpredictable.");
            }
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
