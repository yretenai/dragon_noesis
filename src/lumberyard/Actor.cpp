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
            switch ((ACTOR_CHUNK_TYPE)chunkHeader.Type) {
            case ACTOR_CHUNK_TYPE::Node:
                LOG("Found Node");
                break;
            case ACTOR_CHUNK_TYPE::Mesh:
                LOG("Found Mesh");
                break;
            case ACTOR_CHUNK_TYPE::SkinningInfo:
                LOG("Found SkinningInfo");
                break;
            case ACTOR_CHUNK_TYPE::StandardMaterial:
                LOG("Found StandardMaterial");
                break;
            case ACTOR_CHUNK_TYPE::StandardMaterialLayer:
                LOG("Found StandardMaterialLayer");
                break;
            case ACTOR_CHUNK_TYPE::EffectMaterial:
                LOG("Found EffectMaterial");
                break;
            case ACTOR_CHUNK_TYPE::Limit:
                LOG("Found Limit");
                break;
            case ACTOR_CHUNK_TYPE::Info:
                LOG("Found Info");
                Chunks.push_back(CAST_EMFX_CHUNK(new ActorInfo(buffer, chunkHeader, ptr)));
                break;
            case ACTOR_CHUNK_TYPE::MeshLOD:
                LOG("Found MeshLOD");
                break;
            case ACTOR_CHUNK_TYPE::StandardProgMorphTargets:
                LOG("Found StandardProgMorphTargets");
                break;
            case ACTOR_CHUNK_TYPE::NodeGroups:
                LOG("Found NodeGroups");
                break;
            case ACTOR_CHUNK_TYPE::Nodes:
                LOG("Found Nodes");
                break;
            case ACTOR_CHUNK_TYPE::StandardMorphTargets:
                LOG("Found StandardMorphTargets");
                break;
            case ACTOR_CHUNK_TYPE::MaterialInfo:
                LOG("Found MaterialInfo");
                break;
            case ACTOR_CHUNK_TYPE::NodeMotionSources:
                LOG("Found NodeMotionSources");
                break;
            case ACTOR_CHUNK_TYPE::AttachmentSources:
                LOG("Found AttachmentSources");
                break;
            case ACTOR_CHUNK_TYPE::MaterialAttributeSet:
                LOG("Found MaterialAttributeSet");
                break;
            case ACTOR_CHUNK_TYPE::GenericMaterial:
                LOG("Found GenericMaterial");
                break;
            case ACTOR_CHUNK_TYPE::PhysicsSetup:
                LOG("Found PhysicsSetup");
                break;
            case ACTOR_CHUNK_TYPE::SimulatedObjectSetup:
                LOG("Found SimulatedObjectSetup");
                break;
            default:
                LOG("Unhandled chunk type " << chunkHeader.Type);
                abort();
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
