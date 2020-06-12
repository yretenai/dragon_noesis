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
    char* checkActorFile(void* valIn, noeUserValType_e valInType) {
        if (valInType != NOEUSERVAL_FILEPATH)
            return const_cast<char*>("Unknown.");
        std::filesystem::path path(reinterpret_cast<wchar_t*>(valIn));
        if (!std::filesystem::exists(path))
            return const_cast<char*>("Does not exist");
        Array<char> data = read_file(path);
        if (!Actor::check(&data)) {
            return const_cast<char*>("Not an EMFX Actor file");
        }
        return NULL;
    }

    noesisModel_t* Actor::noesis_load(BYTE* buffer, int length, int& numMdl, noeRAPI_t* rapi) {
        wchar_t* path = new wchar_t[MAX_NOESIS_PATH];
        if (rapi->Noesis_IsExporting()) {
            g_nfn->NPAPI_GetSelectedFile(path);
        } else {
            g_nfn->NPAPI_GetOpenPreviewFile(path);
        }
        std::filesystem::path modelPath;
        if (wcslen(path) < 2) {
            delete[] path;
            return nullptr;
        } else {
            modelPath = std::filesystem::path(path);
            delete[] path;
        }
        std::string animName;
        bool isAnimation = modelPath.extension() == ".motion";

        Array<char> dataBuffer = Array<char>(reinterpret_cast<char*>(buffer), length);
        Actor actor;
        if (isAnimation) {
            noeUserPromptParam_t prompt = {const_cast<char*>("Select EMFX Actor file"),
                                           const_cast<char*>("Select Actor file to apply this animation to."), nullptr, NOEUSERVAL_FILEPATH,
                                           checkActorFile};
            bool promptResult = g_nfn->NPAPI_UserPrompt(&prompt);
            if (!promptResult || prompt.valBuf[0] == 0) {
                return nullptr;
            }
            wchar_t* promptPath = reinterpret_cast<wchar_t*>(prompt.valBuf);
            if (!std::filesystem::exists(promptPath)) {
                return nullptr;
            }
            animName = modelPath.filename().replace_extension("").string();
            modelPath = std::filesystem::path(promptPath);
            Array<char> data = read_file(modelPath);
            actor = Actor(&data);
        } else {
            actor = Actor(&dataBuffer);
        }

        void* context = rapi->rpgCreateContext();
        CArrayList<noesisModel_t*> models = CArrayList<noesisModel_t*>();

        ActorInfo* info = CAST_ABSTRACT_CHUNK(ActorInfo, actor.get_chunk(ACTOR_CHUNK_TYPE::Info));
        rapi->rpgSetName(rapi->Noesis_PooledString(const_cast<char*>(info->Name.c_str())));

        // calculate bones
        ActorNodes* nodes = CAST_ABSTRACT_CHUNK(ActorNodes, actor.get_chunk(ACTOR_CHUNK_TYPE::Nodes));
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
        std::vector<void*> buffers;
        if (isAnimation) {
            Animation anim(&dataBuffer);
            noeKeyFramedAnim_t* keyFramedAnim = static_cast<noeKeyFramedAnim_t*>(rapi->Noesis_UnpooledAlloc(sizeof(noeKeyFramedAnim_t)));
            buffers.push_back(keyFramedAnim);
            memset(keyFramedAnim, 0, sizeof(noeKeyFramedAnim_t));
            keyFramedAnim->name = rapi->Noesis_PooledString(const_cast<char*>(animName.c_str()));
            keyFramedAnim->numBones = nodes->Header.NumNodes;
            keyFramedAnim->framesPerSecond = 30.0f;
            keyFramedAnim->flags = KFANIMFLAG_SEPARATETS | KFANIMFLAG_USEBONETIMES | KFANIMFLAG_PLUSONE;
            MotionInfo* animInfo = CAST_ABSTRACT_CHUNK(MotionInfo, anim.get_chunk(MOTION_CHUNK_TYPE::Info));
            MotionSubMotions* subMotions = CAST_ABSTRACT_CHUNK(MotionSubMotions, anim.get_chunk(MOTION_CHUNK_TYPE::SubMotions));
            std::vector<float> floats;
            noeKeyFramedBone_t* keyFramedBones =
                static_cast<noeKeyFramedBone_t*>(rapi->Noesis_UnpooledAlloc(sizeof(noeKeyFramedBone_t) * subMotions->Motions.size()));
            memset(keyFramedBones, 0, sizeof(noeKeyFramedBone_t) * subMotions->Motions.size());
            buffers.push_back(keyFramedBones);
            uint32_t actualIndex = 0;
            uint32_t floatIndex = 0;
            for (uint32_t i = 0; i < subMotions->Motions.size(); i++) {
                MotionSubMotion* motion = subMotions->Motions[i].get();
                if (boneMap.find(motion->Name) == boneMap.end()) {
                    continue;
                }
                keyFramedBones[actualIndex].boneIndex = boneMap[motion->Name];
                keyFramedBones[actualIndex].translationType = NOEKF_TRANSLATION_VECTOR_3;
                keyFramedBones[actualIndex].rotationType = NOEKF_ROTATION_QUATERNION_4;
                keyFramedBones[actualIndex].scaleType = NOEKF_SCALE_VECTOR_3;
                keyFramedBones[actualIndex].translationInterpolation = NOEKF_INTERPOLATE_LINEAR;
                keyFramedBones[actualIndex].rotationInterpolation = NOEKF_INTERPOLATE_LINEAR;
                keyFramedBones[actualIndex].scaleInterpolation = NOEKF_INTERPOLATE_LINEAR;
                keyFramedBones[actualIndex].numTranslationKeys = motion->Positions.size();
                keyFramedBones[actualIndex].numRotationKeys = motion->Rotations.size();
                keyFramedBones[actualIndex].numScaleKeys = motion->Scales.size();
                keyFramedBones[actualIndex].minTime = 0.0f;
                RichVec3 bindPos(motion->Header.BindPosition.X, motion->Header.BindPosition.Y, motion->Header.BindPosition.Z);
                VECTOR4_SINGLE bindRotation = Animation::uncompress_quaternion(motion->Header.BindRotation);
                RichMat43 bindRot = RichQuat(bindRotation.X, bindRotation.Y, bindRotation.Z, bindRotation.W).GetTranspose().ToMat43();
                RichVec3 bindScale(motion->Header.BindScale.X, motion->Header.BindScale.Y, motion->Header.BindScale.Z);
                if (keyFramedBones[actualIndex].numTranslationKeys > 0) {
                    noeKeyFrameData_t* posKeyframes = static_cast<noeKeyFrameData_t*>(
                        rapi->Noesis_UnpooledAlloc(sizeof(noeKeyFrameData_t) * keyFramedBones[actualIndex].numTranslationKeys));
                    memset(posKeyframes, 0, sizeof(noeKeyFrameData_t) * keyFramedBones[actualIndex].numTranslationKeys);
                    buffers.push_back(posKeyframes);
                    for (uint32_t j = 0; j < keyFramedBones[actualIndex].numTranslationKeys; j++) {
                        MOTION_VECTOR3_KEY key = motion->Positions[j];
                        posKeyframes[j].dataIndex = floatIndex;
                        posKeyframes[j].time = key.Time;
                        if (posKeyframes[j].time > keyFramedBones[actualIndex].maxTime) {
                            keyFramedBones[actualIndex].maxTime = posKeyframes[j].time;
                        }
                        floats.resize(floatIndex + 3);
                        RichVec3 pos(key.Value.X, key.Value.Y, key.Value.Z);
                        if (animInfo->Header.IsAdditive == 1) {
                            pos += bindPos;
                        }
                        floats[floatIndex] = pos.v[0];
                        floats[floatIndex + 1] = pos.v[1];
                        floats[floatIndex + 2] = pos.v[2];
                        floatIndex += 3;
                    }
                    keyFramedBones[actualIndex].translationKeys = posKeyframes;
                }
                if (keyFramedBones[actualIndex].numRotationKeys > 0) {
                    noeKeyFrameData_t* rotKeyframes = static_cast<noeKeyFrameData_t*>(
                        rapi->Noesis_UnpooledAlloc(sizeof(noeKeyFrameData_t) * keyFramedBones[actualIndex].numRotationKeys));
                    memset(rotKeyframes, 0, sizeof(noeKeyFrameData_t) * keyFramedBones[actualIndex].numRotationKeys);
                    buffers.push_back(rotKeyframes);
                    for (uint32_t j = 0; j < keyFramedBones[actualIndex].numRotationKeys; j++) {
                        MOTION_VECTOR4_KEY key = motion->Rotations[j];
                        rotKeyframes[j].dataIndex = floatIndex;
                        rotKeyframes[j].time = key.Time;
                        if (rotKeyframes[j].time > keyFramedBones[actualIndex].maxTime) {
                            keyFramedBones[actualIndex].maxTime = rotKeyframes[j].time;
                        }
                        floats.resize(floatIndex + 4);
                        VECTOR4_SINGLE rotation = Animation::uncompress_quaternion(key.Value);
                        RichMat43 rotMat = RichQuat(rotation.X, rotation.Y, rotation.Z, rotation.W).GetTranspose().ToMat43();
                        if (animInfo->Header.IsAdditive == 1) {
                            rotMat *= bindRot;
                        }
                        RichQuat rot = rotMat.ToQuat();
                        floats[floatIndex] = rot[0];
                        floats[floatIndex + 1] = rot[1];
                        floats[floatIndex + 2] = rot[2];
                        floats[floatIndex + 3] = rot[3];
                        floatIndex += 4;
                    }
                    keyFramedBones[actualIndex].rotationKeys = rotKeyframes;
                }
                if (keyFramedBones[actualIndex].numScaleKeys > 0) {
                    noeKeyFrameData_t* scaleKeyframes = static_cast<noeKeyFrameData_t*>(
                        rapi->Noesis_UnpooledAlloc(sizeof(noeKeyFrameData_t) * keyFramedBones[actualIndex].numScaleKeys));
                    memset(scaleKeyframes, 0, sizeof(noeKeyFrameData_t) * keyFramedBones[actualIndex].numScaleKeys);
                    buffers.push_back(scaleKeyframes);
                    for (uint32_t j = 0; j < keyFramedBones[actualIndex].numScaleKeys; j++) {
                        MOTION_VECTOR3_KEY key = motion->Scales[j];
                        scaleKeyframes[j].dataIndex = floatIndex;
                        scaleKeyframes[j].time = key.Time;
                        if (scaleKeyframes[j].time > keyFramedBones[actualIndex].maxTime) {
                            keyFramedBones[actualIndex].maxTime = scaleKeyframes[j].time;
                        }
                        floats.resize(floatIndex + 3);
                        RichVec3 scale(key.Value.X, key.Value.Y, key.Value.Z);
                        if (animInfo->Header.IsAdditive == 1) {
                            scale += bindScale;
                        }
                        floats[floatIndex] = scale.v[0];
                        floats[floatIndex + 1] = scale.v[1];
                        floats[floatIndex + 2] = scale.v[2];
                        floatIndex += 3;
                    }
                    keyFramedBones[actualIndex].scaleKeys = scaleKeyframes;
                }
                actualIndex++;
            }
            keyFramedAnim->numKfBones = actualIndex;
            keyFramedAnim->numDataFloats = floats.size();
            float* floatBuffer = static_cast<float*>(rapi->Noesis_UnpooledAlloc(sizeof(float) * keyFramedAnim->numDataFloats));
            buffers.push_back(floatBuffer);
            std::copy_n(floats.begin(), floats.size(), floatBuffer);
            keyFramedAnim->data = floatBuffer;
            keyFramedAnim->kfBones = keyFramedBones;
            rapi->rpgSetExData_Anims(rapi->Noesis_AnimFromBonesAndKeyFramedAnim(bones, nodes->Header.NumNodes, keyFramedAnim, true));
        }
        for (void* noesis_buffer : buffers) {
            rapi->Noesis_UnpooledFree(noesis_buffer);
        }
        buffers.clear();

        if (LibraryRoot != nullptr) {
            noesisMatData_t* matData = nullptr;
            std::filesystem::path materialPath(modelPath);
            materialPath.replace_extension(".mtl");
            if (std::filesystem::exists(materialPath)) {
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
                matData = rapi->Noesis_GetMatDataFromLists(matList, texList);
            }
            rapi->rpgSetExData_Materials(matData);
        }

        std::vector<std::shared_ptr<AbstractEMFXChunk>> meshChunks;
        std::vector<std::shared_ptr<AbstractEMFXChunk>> skinChunks;
        actor.get_chunks_of_type(ACTOR_CHUNK_TYPE::Mesh, &meshChunks);
        actor.get_chunks_of_type(ACTOR_CHUNK_TYPE::SkinningInfo, &skinChunks);
        std::map<uint32_t, ActorSkinningInfo*> skins;
        for (std::shared_ptr<AbstractEMFXChunk> skinPtr : skinChunks) {
            ActorSkinningInfo* skin = CAST_ABSTRACT_CHUNK(ActorSkinningInfo, skinPtr);
            if (skin->Header.LOD != 0)
                continue;
            skins[skin->Header.NodeIndex] = skin;
        }
        for (std::shared_ptr<AbstractEMFXChunk> meshPtr : meshChunks) {
            ActorMesh* mesh = CAST_ABSTRACT_CHUNK(ActorMesh, meshPtr);
            if (mesh->Header.LOD != 0)
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
                case ACTOR_VBO_V1_HEADER::TYPE::VERTEXID:
                    vertexId = vbo;
                    break;
                default:
                    continue;
                }
            }

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
            rapi->rpgBindBoneWeightBufferSafe(weightBuffer, RPGEODATA_FLOAT, sizeof(float) * maxInfluences, maxInfluences,
                                              sizeof(float) * mesh->Header.TotalVerts * maxInfluences);
            rapi->rpgBindBoneIndexBufferSafe(boneBuffer, RPGEODATA_INT, sizeof(int32_t) * maxInfluences, maxInfluences,
                                             sizeof(int32_t) * mesh->Header.TotalVerts * maxInfluences);
            indiceOffset = 0;
            for (std::shared_ptr<ActorSubmesh> submeshPtr : mesh->Submeshes) {
                ActorSubmesh* submesh = submeshPtr.get();
                rapi->rpgSetMaterialIndex(submeshPtr->Header.MaterialId);
                rapi->rpgCommitTriangles(indiceBuffer + indiceOffset, RPGEODATA_UINT, submesh->Header.NumIndices, RPGEO_TRIANGLE, true);
                indiceOffset += submesh->Header.NumIndices;
            }
        }
        noesisModel_t* mdl = rapi->rpgConstructModel();
        models.Append(mdl);
        for (void* noesis_buffer : buffers) {
            rapi->Noesis_UnpooledFree(noesis_buffer);
        }
        buffers.clear();
        rapi->rpgDestroyContext(context);
        noesisModel_t* mdlList = rapi->Noesis_ModelsFromList(models, numMdl);
        return mdlList;
    }

    bool Actor::noesis_check(BYTE* buffer, int length, [[maybe_unused]] noeRAPI_t* rapi) {
        Array<char> data_buffer = Array<char>(reinterpret_cast<char*>(buffer), length);
        return check(&data_buffer) || Animation::check(&data_buffer);
    }

} // namespace dragon::lumberyard

#endif
