//
// Created by yretenai on 5/28/2020.
//

#include "Model.h"

using namespace dragon::lumberyard::chunk::model;

#define CAST_MODEL_CHUNK(chunk) (std::shared_ptr<AbstractModelChunk>(reinterpret_cast<AbstractModelChunk*>(chunk)))

namespace dragon::lumberyard {
    Model::Model(Array<char>* buffer) {
        assert(check(buffer));
        Header = buffer->cast<CRCH_HEADER>(0);
        if (Header.ChunkCount == 0)
            return;
        assert(buffer->size() >= Header.ChunkTablePointer + sizeof(CRCH_CHUNK_HEADER) * Header.ChunkCount);
        ChunkTable = buffer->cast<CRCH_CHUNK_HEADER>(Header.ChunkTablePointer, Header.ChunkCount);
        Chunks = std::map<CHUNK_ID, std::shared_ptr<AbstractModelChunk>>();
        for (CRCH_CHUNK_HEADER chunk_header : ChunkTable) {
            assert(buffer->size() >= chunk_header.Pointer + chunk_header.Size);
            Array<char> chunk_buffer = buffer->slice(chunk_header.Pointer, chunk_header.Size);
            switch (chunk_header.Type) {
            case CRCH_CHUNK_HEADER::TYPE::Mesh:
                LOG("Found Mesh");
                Chunks[chunk_header.Id] = CAST_MODEL_CHUNK(new Mesh(&chunk_buffer, chunk_header));
                break;
            case CRCH_CHUNK_HEADER::TYPE::Node:
                LOG("Found Node");
                Chunks[chunk_header.Id] = CAST_MODEL_CHUNK(new Node(&chunk_buffer, chunk_header));
                break;
            case CRCH_CHUNK_HEADER::TYPE::MaterialName:
                LOG("Found MaterialName");
                Chunks[chunk_header.Id] = CAST_MODEL_CHUNK(new MaterialName(&chunk_buffer, chunk_header));
                break;
            case CRCH_CHUNK_HEADER::TYPE::Flags:
                LOG("Found Flags");
                Chunks[chunk_header.Id] = CAST_MODEL_CHUNK(new ExportFlags(&chunk_buffer, chunk_header));
                break;
            case CRCH_CHUNK_HEADER::TYPE::DataStream:
                LOG("Found DataStream");
                Chunks[chunk_header.Id] = CAST_MODEL_CHUNK(new DataStream(&chunk_buffer, chunk_header));
                break;
            case CRCH_CHUNK_HEADER::TYPE::Submesh:
                LOG("Found Submesh");
                Chunks[chunk_header.Id] = CAST_MODEL_CHUNK(new Submesh(&chunk_buffer, chunk_header));
                break;
            default:
                LOG("Unhanled model chunk " << (uint32_t)chunk_header.Type);
                break;
            }
        }
    }

    bool Model::check(Array<char>* buffer) {
        if (buffer->size() < sizeof(CRCH_HEADER))
            return false;
        uint32_t* pointer = reinterpret_cast<uint32_t*>(buffer->data());
        return buffer->size() >= sizeof(CRCH_HEADER) && pointer[0] == FOURCC_CRCH && pointer[1] == 0x746;
    }

    bool Model::get_chunk_header(CHUNK_ID id, CRCH_CHUNK_HEADER& chunk) {
        for (uint32_t i = 0; i < Chunks.size(); i++) {
            if (ChunkTable[i].Id == id) {
                chunk = ChunkTable[i];
                return true;
            }
        }
        return false;
    }

    void Model::get_chunks_of_type(CRCH_CHUNK_HEADER::TYPE type, std::vector<std::shared_ptr<AbstractModelChunk>>* chunks) {
        for (uint32_t i = 0; i < Chunks.size(); i++) {
            if (ChunkTable[i].Type == type) {
                chunks->push_back(Chunks[ChunkTable[i].Id]);
            }
        }
    }
} // namespace dragon::lumberyard
