//
// Created by yretenai on 5/28/2020.
//

#ifdef USE_NOESIS

#include "Actor.h"

#define CAST_ABSTRACT_CHUNK(target, chunk) (std::reinterpret_pointer_cast<target>(chunk).get())

using namespace dragon::lumberyard::chunk::emfx;

namespace dragon::lumberyard {
    noesisModel_t* Actor::noesis_load(BYTE* buffer, int length, int& numMdl, noeRAPI_t* rapi) {
        Array<char> dataBuffer = Array<char>(reinterpret_cast<char*>(buffer), length);
        Actor actor(&dataBuffer);

        void* context = rapi->rpgCreateContext();
        CArrayList<noesisModel_t*> models = CArrayList<noesisModel_t*>();
        ActorNodes* nodes = CAST_ABSTRACT_CHUNK(ActorNodes, actor.get_chunk(ACTOR_CHUNK_TYPE::Nodes));
        ActorInfo* info = CAST_ABSTRACT_CHUNK(ActorInfo, actor.get_chunk(ACTOR_CHUNK_TYPE::Info));
        rapi->rpgClearBufferBinds();

        // IDK if this is needed, skeleton is extremely large compared to reference CGF.
        // TODO: Remove this if model positions are also large.
        float RESCALE = 1.0f;
        switch (info->Header.UnitType) {
        case UNIT_TYPE::Inches:
            RESCALE = 2.54f;
            break;
        case UNIT_TYPE::Feet:
            RESCALE = 30.48f;
            break;
        case UNIT_TYPE::Yards:
            RESCALE = 91.44f;
            break;
        case UNIT_TYPE::Miles:
            RESCALE = 160934.4f;
            break;
        case UNIT_TYPE::Milimeters:
            RESCALE = 0.1f;
            break;
        case UNIT_TYPE::Centimeters:
            RESCALE = 1.0f;
            break;
        case UNIT_TYPE::Decimeters:
            RESCALE = 10.0f;
            break;
        case UNIT_TYPE::Meters:
            RESCALE = 100.0f;
            break;
        case UNIT_TYPE::Kilometers:
            RESCALE = 1000.0f;
            break;
        }

        // calculate bones
        modelBone_t* bones = rapi->Noesis_AllocBones(nodes->Header.NumNodes);
        std::set<std::string> addedBones;
        for (int i = 0; i < nodes->Header.NumNodes; i++) {
            ActorNode* node = nodes->Nodes[i].get();
            modelBone_t* bone = &bones[i];
            assert(node->Name.length() < MAX_BONE_NAME_LEN);
            assert(addedBones.find(node->Name) == addedBones.end());
            addedBones.insert(node->Name);
            std::copy_n(node->Name.c_str(), node->Name.length(), bone->name);
            RichMat43 mat =
                RichQuat(node->Header.Rotation.X, node->Header.Rotation.Y, node->Header.Rotation.Z, node->Header.Rotation.W).ToMat43().GetInverse();
            mat.m.o[0] = node->Header.Position.X * node->Header.Scale.X / RESCALE;
            mat.m.o[1] = node->Header.Position.Y * node->Header.Scale.Y / RESCALE;
            mat.m.o[2] = node->Header.Position.Z * node->Header.Scale.Z / RESCALE;
            if (node->Header.ParentIndex > -1) {
                ActorNode* parentNode = nodes->Nodes[node->Header.ParentIndex].get();
                std::copy_n(parentNode->Name.c_str(), parentNode->Name.length(), bone->parentName);
                bone->eData.parent = &bones[node->Header.ParentIndex];
                RichMat43 parentMat = RichMat43(bone->eData.parent->mat);
                bone->mat = (mat * parentMat).m;
            } else {
                bone->mat = mat.m;
            }
        }
        rapi->rpgSetExData_Bones(bones, nodes->Header.NumNodes);

        rapi->rpgBegin(RPGEO_TRIANGLE);
        rapi->rpgVertex3f(new float[3]{0, 0, 0});
        rapi->rpgEnd();
        noesisModel_t* mdl = rapi->rpgConstructModel();
        models.Append(mdl);
        rapi->rpgDestroyContext(context);
        noesisModel_t* mdlList = rapi->Noesis_ModelsFromList(models, numMdl);
        return mdlList;
    }

    bool Actor::noesis_check(BYTE* buffer, int length, [[maybe_unused]] noeRAPI_t* rapi) {
        Array<char> data_buffer = Array<char>(reinterpret_cast<char*>(buffer), length);
        return check(&data_buffer);
    }

} // namespace dragon::lumberyard

#endif
