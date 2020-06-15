//
// Created by yretenai on 5/28/2020.
//

#ifdef USE_NOESIS

#include "Actor.h"
#include "Animation.h"
#include "Material.h"
#include "Model.h"
#include "globals.h"

#define CAST_ABSTRACT_CHUNK(target, chunk) (std::reinterpret_pointer_cast<target>(chunk).get())

using namespace dragon::lumberyard::chunk::emfx;

namespace dragon::lumberyard {
    noesisModel_t* Actor::noesis_load(BYTE* buffer, int length, int& numMdl, noeRAPI_t* rapi) {
        std::filesystem::path modelPath(rapi->Noesis_GetInputNameW());
        std::string animName;
        bool isAnimation = modelPath.extension() == ".motion";

        Array<char> dataBuffer = Array<char>(reinterpret_cast<char*>(buffer), length);
        Actor actor;
        std::vector<void*> buffers;
        if (isAnimation) {
            char actorPath[MAX_NOESIS_PATH];
            std::fill_n(actorPath, MAX_NOESIS_PATH, 0);
            int modelSize = 0;
            BYTE* modelData = nullptr;
            if (AutoDetect) {
                std::vector<std::filesystem::path> files;
                for (std::filesystem::directory_entry entry : std::filesystem::directory_iterator(modelPath.parent_path())) {
                    if (entry.is_regular_file() && entry.path().extension() == ".actor") {
                        files.push_back(entry.path());
                    }
                }
                if (files.size() != 1) {
                    if (files.size() > 0) {
                        files.clear();
                    }
                    std::filesystem::path characterDir = modelPath.parent_path().parent_path() / "character";
                    if (std::filesystem::is_directory(characterDir)) {
                        for (std::filesystem::directory_entry entry : std::filesystem::directory_iterator(characterDir)) {
                            if (entry.is_regular_file() && entry.path().extension() == ".actor") {
                                files.push_back(entry.path());
                            }
                        }
                    }
                }
                if (files.size() == 1) {
                    modelData = rapi->Noesis_ReadFileW(files[0].c_str(), &modelSize);
                    std::string tmp = files[0].string();
                    std::copy_n(tmp.c_str(), tmp.size(), actorPath);
                }
            }
            if (modelData == nullptr) {
                modelData = rapi->Noesis_LoadPairedFile(rapi->Noesis_PooledString(const_cast<char*>("Select EMFX Actor file")),
                                                        rapi->Noesis_PooledString(const_cast<char*>(".actor")), modelSize, actorPath);
            }
            if (modelData == nullptr) {
                return nullptr;
            }
            animName = modelPath.filename().replace_extension("").string();
            modelPath = std::filesystem::path(actorPath);
            buffers.push_back(modelData);
            Array<char> data(reinterpret_cast<char*>(modelData), modelSize);
            actor = Actor(&data);
        } else {
            actor = Actor(&dataBuffer);
        }

        void* context = rapi->rpgCreateContext();
        // CArrayList<noesisModel_t*> models = CArrayList<noesisModel_t*>();

        ActorInfo* info = CAST_ABSTRACT_CHUNK(ActorInfo, actor.get_chunk(ACTOR_CHUNK_TYPE::Info));
        ActorNodes* nodes = CAST_ABSTRACT_CHUNK(ActorNodes, actor.get_chunk(ACTOR_CHUNK_TYPE::Nodes));
        if (info == nullptr || nodes == nullptr) {
            for (void* noesis_buffer : buffers) {
                rapi->Noesis_UnpooledFree(noesis_buffer);
            }
            return nullptr;
        }
        rapi->rpgSetName(rapi->Noesis_PooledString(const_cast<char*>(info->Name.c_str())));

        // calculate bones
        modelBone_t* bones = rapi->Noesis_AllocBones(nodes->Header.NumNodes);
        std::map<std::string, uint32_t> boneMap;
        for (int i = 0; i < nodes->Header.NumNodes; i++) {
            ActorNode* node = nodes->Nodes[i].get();
            modelBone_t* bone = &bones[i];
            assert(node->Name.length() < MAX_BONE_NAME_LEN);
            assert(boneMap.find(node->Name) == boneMap.end());
            boneMap[node->Name] = i;
            std::copy_n(node->Name.c_str(), node->Name.length(), bone->name);
            RichMat43 mat =
                RichQuat(node->Header.Rotation.X, node->Header.Rotation.Y, node->Header.Rotation.Z, node->Header.Rotation.W).ToMat43().GetInverse();
            mat.m.o[0] = node->Header.Position.X * node->Header.Scale.X;
            mat.m.o[1] = node->Header.Position.Y * node->Header.Scale.Y;
            mat.m.o[2] = node->Header.Position.Z * node->Header.Scale.Z;
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

        if (LibraryRoot != nullptr) {
            std::filesystem::path materialPath;
            if (AutoDetect) {
                materialPath = std::filesystem::path(modelPath);
                materialPath.replace_extension(".mtl");
            }

            if (materialPath.empty() || !std::filesystem::is_regular_file(materialPath)) {
                char materialPathNoe[MAX_NOESIS_PATH];
                int unusedMaterialSize = 0;
                BYTE* unusedMaterialData =
                    rapi->Noesis_LoadPairedFile(rapi->Noesis_PooledString(const_cast<char*>("Select Lumberyard Material file")),
                                                rapi->Noesis_PooledString(const_cast<char*>(".mtl")), unusedMaterialSize, materialPathNoe);
                materialPath = std::filesystem::path(materialPathNoe);
                if (unusedMaterialData != nullptr) {
                    rapi->Noesis_UnpooledFree(unusedMaterialData);
                }
            }

            if (!materialPath.empty()) {
                Material material = Material::from_path(materialPath);
                CArrayList<noesisTex_t*> texList;
                CArrayList<noesisMaterial_t*> matList;
                for (Material subMaterial : material.SubMaterials) {
                    noesisMaterial_t* mat = rapi->Noesis_GetMaterialList(1, false);
                    mat->name = rapi->Noesis_PooledString(const_cast<char*>(subMaterial.Name.c_str()));
                    std::copy_n(subMaterial.DiffuseColor, 4, mat->diffuse);
                    std::copy_n(subMaterial.SpecularColor, 4, mat->specular);
                    if (subMaterial.Textures.find("Diffuse") != subMaterial.Textures.end()) {
                        mat->texIdx = Model::noesis_create_texture(subMaterial.Textures["Diffuse"], texList, false, rapi);
                    }
                    if (subMaterial.Textures.find("Bumpmap") != subMaterial.Textures.end()) {
                        mat->specularTexIdx = Model::noesis_create_texture(subMaterial.Textures["Bumpmap"], texList, true, rapi);
                    }
                    matList.Push(mat);
                }
                noesisMatData_t* matData = rapi->Noesis_GetMatDataFromLists(matList, texList);
                rapi->rpgSetExData_Materials(matData);
            }
        }

        std::vector<std::shared_ptr<AbstractEMFXChunk>> meshChunks;
        std::vector<std::shared_ptr<AbstractEMFXChunk>> skinChunks;
        actor.get_chunks_of_type(ACTOR_CHUNK_TYPE::Mesh, &meshChunks);
        actor.get_chunks_of_type(ACTOR_CHUNK_TYPE::SkinningInfo, &skinChunks);
        std::map<uint32_t, ActorSkinningInfo*> skins;
        for (std::shared_ptr<AbstractEMFXChunk> skinPtr : skinChunks) {
            ActorSkinningInfo* skin = CAST_ABSTRACT_CHUNK(ActorSkinningInfo, skinPtr);
            if (skin == nullptr || skin->Header.LOD != 0)
                continue;
            skins[skin->Header.NodeIndex] = skin;
        }
        for (std::shared_ptr<AbstractEMFXChunk> meshPtr : meshChunks) {
            ActorMesh* mesh = CAST_ABSTRACT_CHUNK(ActorMesh, meshPtr);
            if (mesh == nullptr || mesh->Header.LOD != 0)
                continue;
            if (mesh->Header.IsTriangleMesh != 1) {
                LOG("Why does Lumberyard have mixed triangle meshes? We will never know.");
                continue;
            }
            ActorSkinningInfo* skin = skins[mesh->Header.NodeIndex];
            rapi->rpgClearBufferBinds();
            int uvLayer = 0;
            ActorVertexBuffer* vertexId;
            for (std::shared_ptr<ActorVertexBuffer> vboPtr : mesh->VBOs) {
                ActorVertexBuffer* vbo = vboPtr.get();
                switch (vbo->Header.LayerType) {
                case ACTOR_VBO_V1_HEADER::TYPE::POSITIONS:
                case ACTOR_VBO_V1_HEADER::TYPE::NORMALS:
                case ACTOR_VBO_V1_HEADER::TYPE::TANGENTS: {
                    uint8_t* streamBuffer;
                    if (vbo->Header.AttribSizeInBytes == 16) {
                        Array<VECTOR3_SINGLE>* vec3 = Actor::unwrap_simd_array(vbo->Buffer);
                        streamBuffer = (uint8_t*)vec3->to_noesis(rapi);
                        delete vec3;
                    } else {
                        streamBuffer = vbo->Buffer.to_noesis(rapi);
                    }
                    buffers.push_back(streamBuffer);
                    switch (vbo->Header.LayerType) {
                    case ACTOR_VBO_V1_HEADER::TYPE::POSITIONS:
                        rapi->rpgBindPositionBufferSafe(streamBuffer, RPGEODATA_FLOAT, 12, mesh->Header.TotalVerts * 12);
                        break;
                    case ACTOR_VBO_V1_HEADER::TYPE::NORMALS:
                        rapi->rpgBindNormalBufferSafe(streamBuffer, RPGEODATA_FLOAT, 12, mesh->Header.TotalVerts * 12);
                        break;
                    case ACTOR_VBO_V1_HEADER::TYPE::TANGENTS:
                        rapi->rpgBindTangentBufferSafe(streamBuffer, RPGEODATA_FLOAT, 12, mesh->Header.TotalVerts * 12);
                        break;
                    default:
                        throw out_of_bounds_exception();
                    }
                } break;
                case ACTOR_VBO_V1_HEADER::TYPE::UV: {
                    uint8_t* streamBuffer = vbo->Buffer.to_noesis(rapi);
                    buffers.push_back(streamBuffer);
                    if (uvLayer == 0) {
                        rapi->rpgBindUV1BufferSafe(streamBuffer, RPGEODATA_FLOAT, vbo->Header.AttribSizeInBytes, vbo->Buffer.size());
                    } else if (uvLayer == 1) {
                        rapi->rpgBindUV2BufferSafe(streamBuffer, RPGEODATA_FLOAT, vbo->Header.AttribSizeInBytes, vbo->Buffer.size());
                    } else {
                        rapi->rpgBindUVXBufferSafe(streamBuffer, RPGEODATA_FLOAT, vbo->Header.AttribSizeInBytes, uvLayer, mesh->Header.TotalVerts,
                                                   vbo->Buffer.size());
                    }
                    uvLayer++;
                } break;
                case ACTOR_VBO_V1_HEADER::TYPE::COLORS32: {
                    uint8_t* streamBuffer = vbo->Buffer.to_noesis(rapi);
                    buffers.push_back(streamBuffer);
                    rapi->rpgBindColorBufferSafe(streamBuffer, RPGEODATA_BYTE, 4, 4, mesh->Header.TotalVerts * 4);
                    break;
                }
                case ACTOR_VBO_V1_HEADER::TYPE::COLORS128: {
                    uint8_t* streamBuffer = vbo->Buffer.to_noesis(rapi);
                    buffers.push_back(streamBuffer);
                    rapi->rpgBindColorBufferSafe(streamBuffer, RPGEODATA_FLOAT, 16, 4, mesh->Header.TotalVerts * 16);
                    break;
                }
                case ACTOR_VBO_V1_HEADER::TYPE::VERTEXID:
                    vertexId = vbo;
                    break;
                default:
                    continue;
                }
            }

            if (skin != nullptr) {
                uint32_t maxInfluences = 0;
                for (ACTOR_SKINNING_INFO_v1_ENTRY tableEntry : skin->Table) {
                    if (tableEntry.NumElements > maxInfluences)
                        maxInfluences = tableEntry.NumElements;
                }
                float* weightBuffer = static_cast<float*>(rapi->Noesis_UnpooledAlloc(sizeof(float) * mesh->Header.TotalVerts * maxInfluences));
                buffers.push_back(weightBuffer);
                int32_t* boneBuffer = static_cast<int32_t*>(rapi->Noesis_UnpooledAlloc(sizeof(int32_t) * mesh->Header.TotalVerts * maxInfluences));
                buffers.push_back(boneBuffer);
                for (uint32_t i = 0; i < mesh->Header.TotalVerts; i++) {
                    int boneOffset = i * maxInfluences;
                    uint32_t vid = vertexId->Buffer.cast<uint32_t>(i * sizeof(uint32_t));
                    ACTOR_SKINNING_INFO_v1_ENTRY tableEntry = skin->Table[vid];
                    for (uint32_t j = 0; j < maxInfluences; j++) {
                        if (j < tableEntry.NumElements) {
                            weightBuffer[boneOffset + j] = skin->Influences[tableEntry.StartIndex + j].Weight;
                            boneBuffer[boneOffset + j] = skin->Influences[tableEntry.StartIndex + j].NodeIndex;
                        } else {
                            weightBuffer[boneOffset + j] = 0;
                            boneBuffer[boneOffset + j] = -1;
                        }
                    }
                }

                rapi->rpgBindBoneWeightBufferSafe(weightBuffer, RPGEODATA_FLOAT, sizeof(float) * maxInfluences, maxInfluences,
                                                  sizeof(float) * mesh->Header.TotalVerts * maxInfluences);
                rapi->rpgBindBoneIndexBufferSafe(boneBuffer, RPGEODATA_INT, sizeof(int32_t) * maxInfluences, maxInfluences,
                                                 sizeof(int32_t) * mesh->Header.TotalVerts * maxInfluences);
            }

            uint32_t* indiceBuffer = static_cast<uint32_t*>(rapi->Noesis_UnpooledAlloc(sizeof(uint32_t) * mesh->Header.TotalIndices));
            buffers.push_back(indiceBuffer);
            // rebuild vertex buffers.
            uint32_t indiceOffset = 0;
            uint32_t vertexOffset = 0;
            for (std::shared_ptr<ActorSubmesh> submeshPtr : mesh->Submeshes) {
                ActorSubmesh* submesh = submeshPtr.get();
                for (uint32_t i = 0; i < submesh->Header.NumIndices; i++) {
                    indiceBuffer[i + indiceOffset] = submesh->Indices[i] + vertexOffset;
                }
                indiceOffset += submesh->Header.NumIndices;
                vertexOffset += submesh->Header.NumVertices;
            }

            indiceOffset = 0;
            for (std::shared_ptr<ActorSubmesh> submeshPtr : mesh->Submeshes) {
                ActorSubmesh* submesh = submeshPtr.get();
                rapi->rpgSetMaterialIndex(submeshPtr->Header.MaterialId);
                rapi->rpgCommitTriangles(indiceBuffer + indiceOffset, RPGEODATA_UINT, submesh->Header.NumIndices, RPGEO_TRIANGLE, true);
                indiceOffset += submesh->Header.NumIndices;
            }
        }
        noesisModel_t* mdl = rapi->rpgConstructModel();
        CNoeCustomDataList custom;
        custom.CreateCustomData("IsLumberyardActor", "dragon::lumberyard::ActorPresence", rapi, true);
        rapi->Noesis_SetModelCustomData(mdl, custom);
        // models.Append(mdl);
        for (void* noesis_buffer : buffers) {
            rapi->Noesis_UnpooledFree(noesis_buffer);
        }
        buffers.clear();
        // noesisModel_t* mdlList = rapi->Noesis_ModelsFromList(models, numMdl);
        if (isAnimation) {
            Animation anim(&dataBuffer);
            Animation::noesis_inject_anim(mdl, animName, anim, rapi);
        }
        rapi->rpgDestroyContext(context);
        numMdl = 1;
        return mdl;
    }

    bool Actor::noesis_check(BYTE* buffer, int length, [[maybe_unused]] noeRAPI_t* rapi) {
        Array<char> data_buffer = Array<char>(reinterpret_cast<char*>(buffer), length);
        return check(&data_buffer) || Animation::check(&data_buffer);
    }
} // namespace dragon::lumberyard

#endif
