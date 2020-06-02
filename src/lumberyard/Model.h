//
// Created by yretenai on 5/28/2020.
//

#pragma once

#ifndef FMT_DRAGON_MODEL_H
#define FMT_DRAGON_MODEL_H

#include "Lumberyard.h"
#include "ModelStructs.h"
#include "chunks/model/AbstractModelChunk.h"

class LUMBERYARD_EXPORT Model {
public:
    Model(vector<char> buffer);

    ~Model();

    CrChHeader Header;
    vector<CrChChunkHeader> ChunkTable;
    map<uint32_t, AbstractModelChunk*> Chunks;

    CrChChunkHeader* get_chunk_header(uint32_t id);

    static bool check(vector<char> buffer);

#ifdef USE_NOESIS

    static noesisModel_t* noesis_load(BYTE* buffer, int length, int &num_mdl, noeRAPI_t* rapi);

    static bool noesis_check(BYTE* buffer, int length, [[maybe_unused]] noeRAPI_t* rapi);

#endif // USE_NOESIS
};

#endif //FMT_DRAGON_MODEL_H
