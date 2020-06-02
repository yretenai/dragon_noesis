//
// Created by yretenai on 5/28/2020.
//

#include "../dragon.h"
#include "Model.h"
#include "chunks/model/ExportFlags.h"
#include "chunks/model/MaterialName.h"

Model::Model(vector<char> buffer) {
    super_assert_dragon_log(buffer.size() >= sizeof(CrChHeader), "Buffer overflow when parsing model",
                            "Assertion failed -> length > sizeof CrChHeader\n");
    char* buffer_ptr = buffer.data();
    Header = reinterpret_cast<CrChHeader*>(buffer_ptr)[0];
    if(Header.ChunkCount == 0)
        return;
    super_assert_dragon_log(buffer.size() >= Header.ChunkTablePointer + sizeof(CrChChunkHeader) * Header.ChunkCount,
                            "Buffer overflow when parsing model chunk table",
                            "Assertion failed -> length > chunks * sizeof CrChChunkHeader\n");
    ChunkTable = vector<CrChChunkHeader>(reinterpret_cast<CrChChunkHeader*>(buffer_ptr + Header.ChunkTablePointer),
                                         reinterpret_cast<CrChChunkHeader*>(buffer_ptr + Header.ChunkTablePointer +
                                                                            sizeof(CrChChunkHeader) *
                                                                            Header.ChunkCount));
    Chunks = map<uint32_t, shared_ptr<AbstractModelChunk>>();
    for(CrChChunkHeader chunk_header : ChunkTable) {
        super_assert_dragon_log(buffer.size() >= chunk_header.Pointer + chunk_header.Size,
                                "Buffer overflow when parsing model chunk",
                                "Assertion failed -> length > chunk pointer + chunk size\n");
        vector<char> chunk_buffer(buffer_ptr + chunk_header.Pointer,
                                  buffer_ptr + chunk_header.Pointer + chunk_header.Size);
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
                Chunks[chunk_header.Id] = shared_ptr<AbstractModelChunk>(
                        dynamic_cast<AbstractModelChunk*>(new MaterialName(chunk_buffer)));
                break;
            case CRCH_CHUNK_HEADER::ModelChunkFlags:
                write_dragon_log("Found ModelChunkFlags\n");
                Chunks[chunk_header.Id] = shared_ptr<AbstractModelChunk>(
                        dynamic_cast<AbstractModelChunk*>(new ExportFlags(chunk_buffer)));
                break;
            case CRCH_CHUNK_HEADER::ModelChunkData:
                write_dragon_log("Found ModelChunkData\n");
                break;
            case CRCH_CHUNK_HEADER::ModelChunkGeometry:
                write_dragon_log("Found ModelChunkGeometry\n");
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
    return buffer.size() >= sizeof(CrChHeader) && pointer[0] == FOURCC_CRCH && pointer[1] == 0x746;
}

bool Model::get_chunk_header(uint32_t id, CrChChunkHeader& chunk) {
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
    vector<char> data_buffer(buffer, buffer + length);
    Model model(data_buffer);
    return nullptr;
}

bool Model::noesis_check(BYTE* buffer, int length, [[maybe_unused]] noeRAPI_t* rapi) {
    vector<char> data(buffer, buffer + length);
    return Model::check(data);
}

#endif // USE_NOESIS
