//
// Created by yretenai on 5/28/2020.
//

#ifdef USE_NOESIS

#include "Material.h"
#include "Model.h"
#include "Texture.h"
#include "globals.h"

using namespace dragon::lumberyard::chunk::model;

#define CAST_ABSTRACT_CHUNK(target, chunk) (std::reinterpret_pointer_cast<target>(chunk).get())

namespace dragon::lumberyard {
    noesisModel_t* Model::noesis_load(BYTE* buffer, int length, int& num_mdl, noeRAPI_t* rapi) {
        Array<char> data_buffer = Array<char>(reinterpret_cast<char*>(buffer), length);
        Model model(&data_buffer);
        std::vector<std::shared_ptr<AbstractModelChunk>> chunks;
        model.get_chunks_of_type(CRCH_CHUNK_HEADER::TYPE::Node, &chunks);
        void* context = rapi->rpgCreateContext();
        CArrayList<noesisModel_t*> models = CArrayList<noesisModel_t*>();

        for (std::shared_ptr<AbstractModelChunk> abstractChunk : chunks) {
            Node* node = CAST_ABSTRACT_CHUNK(Node, abstractChunk);
            if (node == nullptr || node->Header.ObjectId < 1) {
                continue;
            }
            rapi->rpgClearBufferBinds();
            Mesh* mesh = CAST_ABSTRACT_CHUNK(Mesh, model.Chunks[node->Header.ObjectId]);
            if (mesh == nullptr || mesh->Header.StreamChunkId[(int)DATA_STREAM_HEADER::TYPE::Position][0] < 1 ||
                mesh->Header.StreamChunkId[(int)DATA_STREAM_HEADER::TYPE::Index][0] < 1) {
                LOG("Unable to load mesh " << node->Name << ", has no geometry data");
                continue;
            }

            std::vector<char*> buffers;

            DataStream* indiceStream =
                CAST_ABSTRACT_CHUNK(DataStream, model.Chunks[mesh->Header.StreamChunkId[(int)DATA_STREAM_HEADER::TYPE::Index][0]]);
            DataStream* positionStream =
                CAST_ABSTRACT_CHUNK(DataStream, model.Chunks[mesh->Header.StreamChunkId[(int)DATA_STREAM_HEADER::TYPE::Position][0]]);
            if (positionStream == nullptr || indiceStream == nullptr) {
                continue;
            }

            char* position_stream_buffer = positionStream->Buffer.to_noesis(rapi);
            rapi->rpgBindPositionBufferSafe(position_stream_buffer, RPGEODATA_FLOAT, positionStream->Header.Size, positionStream->Buffer.size());
            buffers.push_back(position_stream_buffer);

            int uvLayer = 0;
            for (CHUNK_ID id : mesh->Header.StreamChunkId[(int)DATA_STREAM_HEADER::TYPE::UV]) {
                if (id < 1) {
                    break;
                }
                DataStream* stream = CAST_ABSTRACT_CHUNK(DataStream, model.Chunks[id]);
                if (stream != nullptr) {
                    char* stream_buffer = stream->Buffer.to_noesis(rapi);
                    if (uvLayer == 0) {
                        rapi->rpgBindUV1BufferSafe(stream_buffer, RPGEODATA_FLOAT, stream->Header.Size, stream->Buffer.size());
                    } else if (uvLayer == 1) {
                        rapi->rpgBindUV2BufferSafe(stream_buffer, RPGEODATA_FLOAT, stream->Header.Size, stream->Buffer.size());
                    } else {
                        rapi->rpgBindUVXBufferSafe(stream_buffer, RPGEODATA_FLOAT, stream->Header.Size, uvLayer, stream->Header.Count,
                                                   stream->Buffer.size());
                    }
                    buffers.push_back(stream_buffer);
                    uvLayer++;
                }
            }

            {
                DataStream* stream = nullptr;
                if (mesh->Header.StreamChunkId[(int)DATA_STREAM_HEADER::TYPE::Color][0] != 0) {
                    stream = CAST_ABSTRACT_CHUNK(DataStream, model.Chunks[mesh->Header.StreamChunkId[(int)DATA_STREAM_HEADER::TYPE::Color][0]]);
                }
                if (stream == nullptr && mesh->Header.StreamChunkId[(int)DATA_STREAM_HEADER::TYPE::Color2][0] != 0) {
                    stream = CAST_ABSTRACT_CHUNK(DataStream, model.Chunks[mesh->Header.StreamChunkId[(int)DATA_STREAM_HEADER::TYPE::Color2][0]]);
                }

                if (stream != nullptr) {
                    char* stream_buffer = stream->Buffer.to_noesis(rapi);
                    rapi->rpgBindColorBufferSafe(stream_buffer, stream->Header.Size == 4 ? RPGEODATA_BYTE : RPGEODATA_FLOAT, stream->Header.Size, 4,
                                                 4);
                    buffers.push_back(stream_buffer);
                }
            }

            if (mesh->Header.StreamChunkId[(int)DATA_STREAM_HEADER::TYPE::Normal][0] != 0) {
                DataStream* stream =
                    CAST_ABSTRACT_CHUNK(DataStream, model.Chunks[mesh->Header.StreamChunkId[(int)DATA_STREAM_HEADER::TYPE::Normal][0]]);
                if (stream != nullptr) {
                    char* stream_buffer = stream->Buffer.to_noesis(rapi);
                    rapi->rpgBindNormalBufferSafe(stream_buffer, RPGEODATA_FLOAT, stream->Header.Size, stream->Buffer.size());
                    buffers.push_back(stream_buffer);
                }
            }

            if (mesh->Header.StreamChunkId[(int)DATA_STREAM_HEADER::TYPE::Tangent][0] != 0) {
                DataStream* stream =
                    CAST_ABSTRACT_CHUNK(DataStream, model.Chunks[mesh->Header.StreamChunkId[(int)DATA_STREAM_HEADER::TYPE::Tangent][0]]);
                if (stream != nullptr) {
                    char* stream_buffer = stream->Buffer.to_noesis(rapi);
                    rapi->rpgBindTangentBufferSafe(stream_buffer, RPGEODATA_FLOAT, stream->Header.Size, stream->Buffer.size());
                    buffers.push_back(stream_buffer);
                }
            }

            MaterialName* materials = nullptr;
            if (node->Header.MaterialId != 0) {
                materials = CAST_ABSTRACT_CHUNK(MaterialName, model.Chunks[node->Header.MaterialId]);
            }
            char* indice_buffer = indiceStream->Buffer.to_noesis(rapi);
            buffers.push_back(indice_buffer);
            Submesh* submeshes = CAST_ABSTRACT_CHUNK(Submesh, model.Chunks[mesh->Header.SubmeshChunkId]);
            if (submeshes == nullptr) {
                for (char* noesis_buffer : buffers) {
                    rapi->Noesis_UnpooledFree(noesis_buffer);
                }
                continue;
            }
            for (SUBMESH_DATA submesh : submeshes->Submeshes) {
                char* name = rapi->Noesis_PooledString(const_cast<char*>(node->Name.c_str()));
                rapi->rpgSetName(name);
                if (materials != nullptr) {
                    char* material_name = rapi->Noesis_PooledString(const_cast<char*>(materials->Materials[submesh.MaterialId].c_str()));
                    rapi->rpgSetMaterial(material_name);
                }
                rapi->rpgCommitTriangles(indice_buffer + submesh.FirstIndexId * indiceStream->Header.Size,
                                         indiceStream->Header.Size == 4 ? RPGEODATA_UINT : RPGEODATA_USHORT, submesh.IndexCount, RPGEO_TRIANGLE,
                                         true);
            }
            noesisMatData_t* matData = nullptr;
            if (materials != nullptr && LibraryRoot != nullptr) {
                std::filesystem::path materialPath;
                if (AutoDetect) {
                    materialPath = std::filesystem::path(rapi->Noesis_GetInputNameW());
                    materialPath.replace_filename(materials->Name + ".mtl");
                    if (!std::filesystem::is_regular_file(materialPath) && materials->Materials.size() == 1) {
                        materialPath.replace_filename(materials->Materials[0] + ".mtl");
                    }
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
                            mat->texIdx = noesis_create_texture(subMaterial.Textures["Diffuse"], texList, false, rapi);
                        }
                        if (subMaterial.Textures.find("Bumpmap") != subMaterial.Textures.end()) {
                            mat->specularTexIdx = noesis_create_texture(subMaterial.Textures["Bumpmap"], texList, true, rapi);
                        }
                        matList.Push(mat);
                    }
                    matData = rapi->Noesis_GetMatDataFromLists(matList, texList);
                }
            }

            if (matData != nullptr) {
                rapi->rpgSetExData_Materials(matData);
            }

            models.Append(rapi->rpgConstructModel());

            for (char* noesis_buffer : buffers) {
                rapi->Noesis_UnpooledFree(noesis_buffer);
            }
        }
        rapi->rpgDestroyContext(context);
        noesisModel_t* mdlList = rapi->Noesis_ModelsFromList(models, num_mdl);
        return mdlList;
    }

    bool Model::noesis_check(BYTE* buffer, int length, [[maybe_unused]] noeRAPI_t* rapi) {
        int32_t handles;
        g_nfn->NPAPI_GetFormatExtensionFlags(const_cast<wchar_t*>(L".cgf"), &handles);
        if (handles > 1) // something else added a CGF handler.
            return false;
        Array<char> data_buffer = Array<char>(reinterpret_cast<char*>(buffer), length);
        return check(&data_buffer);
    }

    int Model::noesis_create_texture(std::filesystem::path texturePath, CArrayList<noesisTex_t*>& texList, bool alpha, noeRAPI_t* rapi) {
        std::filesystem::path combinedPath = *LibraryRoot / texturePath;
        if (alpha) {
            combinedPath.replace_extension(".dds.a");
        } else {
            combinedPath.replace_extension(".dds.1");
        }
        if (!std::filesystem::is_regular_file(combinedPath)) {
            return -1;
        }
        int num = texList.Num();
        Texture::noesies_load_direct(combinedPath, texList, rapi);
        return num;
    }

} // namespace dragon::lumberyard

#endif // USE_NOESIS
