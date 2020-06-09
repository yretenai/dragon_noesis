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
                throw not_implemented_exception();
            case ACTOR_CHUNK_TYPE::MaterialLayer:
                LOG("Found MaterialLayer");
                throw not_implemented_exception();
            case ACTOR_CHUNK_TYPE::Info:
                LOG("Found Info");
                Chunks.push_back(CAST_EMFX_CHUNK(new ActorInfo(buffer, chunkHeader, ptr)));
                break;
            case ACTOR_CHUNK_TYPE::MeshLOD:
                LOG("Found MeshLOD");
                throw not_implemented_exception();
            case ACTOR_CHUNK_TYPE::MorphTarget:
                LOG("Found MorphTarget");
                throw not_implemented_exception();
            case ACTOR_CHUNK_TYPE::NodeGroups:
                LOG("Found NodeGroups");
                throw not_implemented_exception();
            case ACTOR_CHUNK_TYPE::Nodes:
                LOG("Found Nodes");
                Chunks.push_back(CAST_EMFX_CHUNK(new ActorNodes(buffer, chunkHeader, ptr)));
            case ACTOR_CHUNK_TYPE::MorphTargets:
                LOG("Found MorphTargets");
                throw not_implemented_exception();
            case ACTOR_CHUNK_TYPE::MaterialInfo:
                LOG("Found MaterialInfo");
                throw not_implemented_exception();
            case ACTOR_CHUNK_TYPE::NodeMotionSources:
                LOG("Found NodeMotionSources");
                throw not_implemented_exception();
            case ACTOR_CHUNK_TYPE::AttachmentSources:
                LOG("Found AttachmentSources");
                throw not_implemented_exception();
            case ACTOR_CHUNK_TYPE::MaterialAttributeSet:
                LOG("Found MaterialAttributeSet");
                throw not_implemented_exception();
            case ACTOR_CHUNK_TYPE::GenericMaterial:
                LOG("Found GenericMaterial");
                throw not_implemented_exception();
            case ACTOR_CHUNK_TYPE::PhysicsSetup:
                LOG("Found PhysicsSetup");
                throw not_implemented_exception();
            case ACTOR_CHUNK_TYPE::SimulatedObjectSetup:
                LOG("Found SimulatedObjectSetup");
                throw not_implemented_exception();
            default:
                LOG("Unhandled chunk type " << chunkHeader.Type);
                ptr += chunkHeader.Size;
            }

            if (ptr != expectedPtr) {
                LOG("WARNING! Expected to be at pos " << expectedPtr << " but we're at " << ptr << " behavior might be unpredictable.");
            }
        }
    }

    void Actor::get_chunks_of_type(ACTOR_CHUNK_TYPE type, std::vector<std::shared_ptr<AbstractEMFXChunk>>* chunks) {}

    bool Actor::check(Array<char>* buffer) {
        if (buffer->size() < sizeof(ACTOR_HEADER))
            return false;
        ACTOR_HEADER header = buffer->cast<ACTOR_HEADER>(0);
        return buffer->size() >= sizeof(ACTOR_HEADER) && header.Magic == FOURCC_ACTR && header.IsBigEndian == 0;
    }
} // namespace dragon::lumberyard
