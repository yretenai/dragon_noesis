//
// Created by yretenai on 5/28/2020.
//

#include "Model.h"

#include <memory>

using namespace dragon::lumberyard::chunk::model;

#define CAST_MODEL_CHUNK(chunk)                                                \
    (std::shared_ptr<AbstractModelChunk>(                                      \
        reinterpret_cast<AbstractModelChunk*>(chunk)))
#define CAST_ABSTRACT_CHUNK(target, chunk)                                     \
    (std::reinterpret_pointer_cast<target>(chunk).get())

namespace dragon::lumberyard {
    Model::Model(Array<char>* buffer) {
        assert(buffer->size() >= sizeof(CRCH_HEADER));
        Header = buffer->cast<CRCH_HEADER>(0);
        if (Header.ChunkCount == 0)
            return;
        assert(buffer->size() >=
               Header.ChunkTablePointer +
                   sizeof(CRCH_CHUNK_HEADER) * Header.ChunkCount);
        ChunkTable = buffer->cast<CRCH_CHUNK_HEADER>(Header.ChunkTablePointer,
                                                     Header.ChunkCount);
        Chunks = std::map<CHUNK_ID, std::shared_ptr<AbstractModelChunk>>();
        for (CRCH_CHUNK_HEADER chunk_header : ChunkTable) {
            assert(buffer->size() >= chunk_header.Pointer + chunk_header.Size);
            Array<char> chunk_buffer =
                buffer->slice(chunk_header.Pointer, chunk_header.Size);
            switch (chunk_header.Type) {
            case CRCH_CHUNK_HEADER::TYPE::Mesh:
                Chunks[chunk_header.Id] =
                    CAST_MODEL_CHUNK(new Mesh(&chunk_buffer, chunk_header));
                break;
            case CRCH_CHUNK_HEADER::TYPE::Node:
                Chunks[chunk_header.Id] =
                    CAST_MODEL_CHUNK(new Node(&chunk_buffer, chunk_header));
                break;
            case CRCH_CHUNK_HEADER::TYPE::MaterialName:
                Chunks[chunk_header.Id] = CAST_MODEL_CHUNK(
                    new MaterialName(&chunk_buffer, chunk_header));
                break;
            case CRCH_CHUNK_HEADER::TYPE::Flags:
                Chunks[chunk_header.Id] = CAST_MODEL_CHUNK(
                    new ExportFlags(&chunk_buffer, chunk_header));
                break;
            case CRCH_CHUNK_HEADER::TYPE::DataStream:
                Chunks[chunk_header.Id] = CAST_MODEL_CHUNK(
                    new DataStream(&chunk_buffer, chunk_header));
                break;
            case CRCH_CHUNK_HEADER::TYPE::Submesh:
                Chunks[chunk_header.Id] =
                    CAST_MODEL_CHUNK(new Submesh(&chunk_buffer, chunk_header));
                break;
            }
        }
    }

    bool Model::check(Array<char>* buffer) {
        uint32_t* pointer = reinterpret_cast<uint32_t*>(buffer->data());
        return buffer->size() >= sizeof(CRCH_HEADER) &&
               pointer[0] == FOURCC_CRCH && pointer[1] == 0x746;
    }

    bool Model::get_chunk_header(uint32_t id, CRCH_CHUNK_HEADER& chunk) {
        for (uint32_t i = 0; i < Chunks.size(); i++) {
            if (ChunkTable[i].Id == id) {
                chunk = ChunkTable[i];
                return true;
            }
        }
        return false;
    }

    void Model::get_chunks_of_type(
        CRCH_CHUNK_HEADER::TYPE type,
        std::vector<std::shared_ptr<AbstractModelChunk>>* chunks) {
        for (uint32_t i = 0; i < Chunks.size(); i++) {
            if (ChunkTable[i].Type == type) {
                chunks->push_back(Chunks[ChunkTable[i].Id]);
            }
        }
    }

#ifdef USE_NOESIS

    noesisModel_t* Model::noesis_load(BYTE* buffer, int length, int& num_mdl,
                                      noeRAPI_t* rapi) {
        Array<char> data_buffer =
            Array<char>(reinterpret_cast<char*>(buffer), length);
        Model model(&data_buffer);
        std::vector<std::shared_ptr<AbstractModelChunk>> chunks;
        model.get_chunks_of_type(CRCH_CHUNK_HEADER::TYPE::Node, &chunks);
        void* context = rapi->rpgCreateContext();
        CArrayList<noesisModel_t*> models = CArrayList<noesisModel_t*>();
        for (std::shared_ptr<AbstractModelChunk> abstractChunk : chunks) {
            Node* node = CAST_ABSTRACT_CHUNK(Node, abstractChunk);
            if (node->Header.ObjectId < 1) {
                continue;
            }
            rapi->rpgClearBufferBinds();
            Mesh* mesh =
                CAST_ABSTRACT_CHUNK(Mesh, model.Chunks[node->Header.ObjectId]);

            if (mesh->Header.StreamChunkId[(
                    int)DATA_STREAM_HEADER::TYPE::Position][0] != 0) {
                DataStream* stream = CAST_ABSTRACT_CHUNK(
                    DataStream,
                    model.Chunks[mesh->Header.StreamChunkId[(
                        int)DATA_STREAM_HEADER::TYPE::Position][0]]);
                rapi->rpgBindPositionBufferSafe(
                    stream->Buffer.data(), RPGEODATA_FLOAT, stream->Header.Size,
                    stream->Buffer.size());
            }

            int uvLayer = 0;
            for (CHUNK_ID id :
                 mesh->Header
                     .StreamChunkId[(int)DATA_STREAM_HEADER::TYPE::UV]) {
                if (id < 1) {
                    break;
                }
                DataStream* stream =
                    CAST_ABSTRACT_CHUNK(DataStream, model.Chunks[id]);
                if (uvLayer == 0) {
                    rapi->rpgBindUV1BufferSafe(
                        stream->Buffer.data(), RPGEODATA_FLOAT,
                        stream->Header.Size, stream->Buffer.size());
                } else if (uvLayer == 1) {
                    rapi->rpgBindUV2BufferSafe(
                        stream->Buffer.data(), RPGEODATA_FLOAT,
                        stream->Header.Size, stream->Buffer.size());
                } else {
                    rapi->rpgBindUVXBufferSafe(
                        stream->Buffer.data(), RPGEODATA_FLOAT,
                        stream->Header.Size, uvLayer, stream->Header.Count,
                        stream->Buffer.size());
                }
                uvLayer++;
            }

            // TODO: Color.

            if (mesh->Header
                    .StreamChunkId[(int)DATA_STREAM_HEADER::TYPE::Normal][0] !=
                0) {
                DataStream* stream = CAST_ABSTRACT_CHUNK(
                    DataStream, model.Chunks[mesh->Header.StreamChunkId[(
                                    int)DATA_STREAM_HEADER::TYPE::Normal][0]]);
                rapi->rpgBindNormalBufferSafe(
                    stream->Buffer.data(), RPGEODATA_FLOAT, stream->Header.Size,
                    stream->Buffer.size());
            }

            if (mesh->Header
                    .StreamChunkId[(int)DATA_STREAM_HEADER::TYPE::Tangent][0] !=
                0) {
                DataStream* stream = CAST_ABSTRACT_CHUNK(
                    DataStream, model.Chunks[mesh->Header.StreamChunkId[(
                                    int)DATA_STREAM_HEADER::TYPE::Tangent][0]]);
                rapi->rpgBindTangentBufferSafe(
                    stream->Buffer.data(), RPGEODATA_FLOAT, stream->Header.Size,
                    stream->Buffer.size());
            }

            MaterialName* materials = nullptr;
            if (node->Header.MaterialId != 0) {
                materials = CAST_ABSTRACT_CHUNK(
                    MaterialName, model.Chunks[node->Header.MaterialId]);
            }

            DataStream* indiceBuffer = CAST_ABSTRACT_CHUNK(
                DataStream, model.Chunks[mesh->Header.StreamChunkId[(
                                int)DATA_STREAM_HEADER::TYPE::Index][0]]);
            Submesh* submeshes = CAST_ABSTRACT_CHUNK(
                Submesh, model.Chunks[mesh->Header.SubmeshChunkId]);
            for (SUBMESH_DATA submesh : submeshes->Submeshes) {
                rapi->rpgSetName(const_cast<char*>(node->Name.c_str()));
                if (materials != nullptr) {
                    rapi->rpgSetMaterial(const_cast<char*>(
                        materials->Materials[submesh.MaterialId].c_str()));
                }
                rapi->rpgCommitTriangles(
                    indiceBuffer->Buffer.data() +
                        submesh.FirstIndexId * indiceBuffer->Header.Size,
                    indiceBuffer->Header.Size == 4 ? RPGEODATA_UINT
                                                   : RPGEODATA_USHORT,
                    submesh.IndexCount, RPGEO_TRIANGLE, true);
            }
            models.Append(rapi->rpgConstructModel());
        }
        rapi->rpgDestroyContext(context);
        noesisModel_t* mdlList = rapi->Noesis_ModelsFromList(models, num_mdl);
        return mdlList;
    }

    bool Model::noesis_check(BYTE* buffer, int length,
                             [[maybe_unused]] noeRAPI_t* rapi) {
        Array<char> data_buffer =
            Array<char>(reinterpret_cast<char*>(buffer), length);
        return Model::check(&data_buffer);
    }

#endif // USE_NOESIS
} // namespace dragon::lumberyard
