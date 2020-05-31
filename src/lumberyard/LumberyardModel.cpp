//
// Created by yretenai on 5/28/2020.
//

#include "../dragon.h"
#include "LumberyardModel.h"

LumberyardModel::LumberyardModel(vector<char> buffer) {
    super_assert_dragon_log<overflow_error>(buffer.size() >= sizeof(CrChHeader), "Buffer overflow when parsing model",
                                            "Assertion failed -> length > sizeof CrChHeader\n");
    char* buffer_ptr = buffer.data();
    Header = reinterpret_cast<CrChHeader*>(buffer_ptr)[0];
    if(Header.ChunkCount == 0)
        return;
    super_assert_dragon_log<overflow_error>(
            buffer.size() >= Header.ChunkTablePointer + sizeof(CrChChunkHeader) * Header.ChunkCount,
            "Buffer overflow when parsing model chunk table",
            "Assertion failed -> length > chunks * sizeof CrChChunkHeader\n");
    ChunkTable = vector<CrChChunkHeader>(reinterpret_cast<CrChChunkHeader*>(buffer_ptr) + Header.ChunkTablePointer,
                                         reinterpret_cast<CrChChunkHeader*>(buffer_ptr) + Header.ChunkTablePointer +
                                         sizeof(CrChChunkHeader) * Header.ChunkTablePointer);

    for(CrChChunkHeader chunk_header : ChunkTable) {
        super_assert_dragon_log<overflow_error>(buffer.size() >= chunk_header.Pointer + chunk_header.Size,
                                                "Buffer overflow when parsing model chunk",
                                                "Assertion failed -> length > chunk pointer + chunk size\n");
        vector<char> chunk_buffer(buffer_ptr + chunk_header.Pointer,
                                  buffer_ptr + chunk_header.Pointer + chunk_header.Size);
        switch(chunk_header.Type) {
            case LumberyardModelChunkMesh:
                write_dragon_log("Found MODEL_CHUNK_MESH\n");
                break;
            case LumberyardModelChunkMaterialList:
                write_dragon_log("Found MODEL_CHUNK_MATERIAL_LIST\n");
                break;
            case LumberyardModelChunkNode:
                write_dragon_log("Found MODEL_CHUNK_NODE\n");
                break;
            case LumberyardModelChunkMaterial:
                write_dragon_log("Found MODEL_CHUNK_MATERIAL\n");
                break;
            case LumberyardModelChunkMeta:
                write_dragon_log("Found MODEL_CHUNK_META\n");
                break;
            case LumberyardModelChunkMaterialName:
                write_dragon_log("Found MODEL_CHUNK_MATERIAL_NAME\n");
                break;
            case LumberyardModelChunkFlags:
                write_dragon_log("Found MODEL_CHUNK_FLAGS\n");
                break;
            case LumberyardModelChunkData:
                write_dragon_log("Found MODEL_CHUNK_DATA\n");
                break;
            case LumberyardModelChunkGeometry:
                write_dragon_log("Found MODEL_CHUNK_GEOMETRY\n");
                break;
            default:
                write_dragon_log("%s (%d)\n", "Unhandled model chunk!", chunk_header.Type);
                break;
        }
        flush_dragon_log();
    }
}

bool LumberyardModel::check(vector<char> buffer) {
    uint32_t* pointer = reinterpret_cast<uint32_t*>(buffer.data());
    return buffer.size() >= sizeof(CrChHeader) && pointer[0] == FOURCC_CRCH && pointer[1] == 0x746;
}

#ifdef USE_NOESIS

noesisModel_t* LumberyardModel::noesis_load(BYTE* buffer, int length, int &num_mdl, noeRAPI_t* rapi) {
    vector<char> data_buffer(buffer, buffer + length);
    LumberyardModel model(data_buffer);
    return nullptr;
}

bool LumberyardModel::noesis_check(BYTE* buffer, int length, [[maybe_unused]] noeRAPI_t* rapi) {
    vector<char> data(buffer, buffer + length);
    return LumberyardModel::check(data);
}

#endif // USE_NOESIS
