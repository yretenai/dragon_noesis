//
// Created by yretenai on 5/28/2020.
//

#pragma once

#ifndef FMT_DRAGON_LUMBERYARDMODEL_H
#define FMT_DRAGON_LUMBERYARDMODEL_H

#include "Lumberyard.h"
#include "LumberyardModelStructs.h"

class DRAGON_EXPORT LumberyardModel {
public:
    LumberyardModel(vector<char> buffer);

    CrChHeader Header;
    vector<CrChChunkHeader> ChunkTable;

    static bool check(vector<char> buffer);

#ifdef USE_NOESIS

    static noesisModel_t* noesis_load(BYTE* buffer, int length, int &num_mdl, noeRAPI_t* rapi);

    static bool noesis_check(BYTE* buffer, int length, [[maybe_unused]] noeRAPI_t* rapi);

#endif // USE_NOESIS
};

#endif //FMT_DRAGON_LUMBERYARDMODEL_H
