//
// Created by yretenai on 5/28/2020.
//

#include "../dragon.h"
#include "Model.h"
#include "chunks/model/ExportFlags.h"
#include "chunks/model/MaterialName.h"
#include "chunks/model/Submesh.h"

using namespace dragon::lumberyard::chunk::model;

#define CAST_MODEL_CHUNK(chunk) shared_ptr<AbstractModelChunk>(dynamic_cast<AbstractModelChunk*>(chunk))

namespace dragon::lumberyard {
    Model::Model(vector<char> buffer) {
        super_assert_dragon_log(buffer.size() >= sizeof(CRCH_HEADER), "Assertion failed -> length > sizeof CrChHeader\n");
        char* buffer_ptr = buffer.data();
        Header = vector_cast<CRCH_HEADER>(buffer_ptr);
        if(Header.ChunkCount == 0)
            return;
        super_assert_dragon_log(
                buffer.size() >= Header.ChunkTablePointer + sizeof(CRCH_CHUNK_HEADER) * Header.ChunkCount,
                "Assertion failed -> length > chunks * sizeof CrChChunkHeader\n");
        ChunkTable = vector_cast_slice<CRCH_CHUNK_HEADER>(buffer_ptr + Header.ChunkTablePointer, Header.ChunkCount);
        Chunks = map<uint32_t, shared_ptr<AbstractModelChunk>>();
        for(CRCH_CHUNK_HEADER chunk_header : ChunkTable) {
            super_assert_dragon_log(buffer.size() >= chunk_header.Pointer + chunk_header.Size,
                                    "Assertion failed -> length > chunk pointer + chunk size\n");
            vector<char> chunk_buffer = vector_slice(buffer_ptr + chunk_header.Pointer, chunk_header.Size);
            switch(chunk_header.Type) {
                case CRCH_CHUNK_HEADER::ModelChunkMesh:
                    write_dragon_log("Found ModelChunkMesh\n");
                    break;
                case CRCH_CHUNK_HEADER::ModelChunkMaterialList:
                    write_dragon_log("Found ModelChunkMaterialList\n");
                    break;
                case CRCH_CHUNK_HEADER::ModelChunkNode:
                    write_dragon_log("Found ModelChunkNode\n");
                    break;
                case CRCH_CHUNK_HEADER::ModelChunkMaterial:
                    write_dragon_log("Found ModelChunkMaterial\n");
                    break;
                case CRCH_CHUNK_HEADER::ModelChunkMeta:
                    write_dragon_log("Found ModelChunkMeta\n");
                    break;
                case CRCH_CHUNK_HEADER::ModelChunkMaterialName:
                    write_dragon_log("Found ModelChunkMaterialName\n");
                    Chunks[chunk_header.Id] = CAST_MODEL_CHUNK(new MaterialName(chunk_buffer, chunk_header.Version));
                    break;
                case CRCH_CHUNK_HEADER::ModelChunkFlags:
                    write_dragon_log("Found ModelChunkFlags\n");
                    Chunks[chunk_header.Id] = CAST_MODEL_CHUNK(new ExportFlags(chunk_buffer, chunk_header.Version));
                    break;
                case CRCH_CHUNK_HEADER::ModelChunkData:
                    write_dragon_log("Found ModelChunkData\n");
                    break;
                case CRCH_CHUNK_HEADER::ModelChunkSubmesh:
                    write_dragon_log("Found ModelChunkSubmesh\n");
                    Chunks[chunk_header.Id] = CAST_MODEL_CHUNK(new Submesh(chunk_buffer, chunk_header.Version));
                    break;
                default:
                    write_dragon_log("%s (%X)\n", "Unhandled model chunk!", chunk_header.Type);
                    break;
            }
            flush_dragon_log();
        }
    }

    bool Model::check(vector<char> buffer) {
        uint32_t* pointer = reinterpret_cast<uint32_t*>(buffer.data());
        return buffer.size() >= sizeof(CRCH_HEADER) && pointer[0] == FOURCC_CRCH && pointer[1] == 0x746;
    }

    bool Model::get_chunk_header(uint32_t id, CRCH_CHUNK_HEADER &chunk) {
        for(int i = 0; i < Chunks.size(); i++) {
            if(ChunkTable[i].Id == id) {
                chunk = ChunkTable[i];
                return true;
            }
        }
        return false;
    }

#ifdef USE_NOESIS

    noesisModel_t* Model::noesis_load(BYTE* buffer, int length, int &num_mdl, noeRAPI_t* rapi) {
        vector<char> data_buffer = vector_slice(reinterpret_cast<char*>(buffer), length);
        Model model(data_buffer);
        return nullptr;
    }

    bool Model::noesis_check(BYTE* buffer, int length, [[maybe_unused]] noeRAPI_t* rapi) {
        vector<char> data_buffer = vector_slice(reinterpret_cast<char*>(buffer), length);
        return Model::check(data_buffer);
    }

#endif // USE_NOESIS
}
