//
// Created by yretenai on 6/2/2020.
//

#pragma once

#ifndef FMT_LUMBERYARD_MATERIALNAME_H
#define FMT_LUMBERYARD_MATERIALNAME_H

#include "AbstractModelChunk.h"

namespace dragon::lumberyard::chunk::model {
    class LUMBERYARD_EXPORT MaterialName : public AbstractModelChunk {
      public:
        MaterialName(Array<char>* buffer, CRCH_CHUNK_HEADER chunk_header);

        std::string Name;
        Array<int32_t> Types;
        Array<std::string> Materials;
    };
} // namespace dragon::lumberyard::chunk::model

#endif // FMT_LUMBERYARD_MATERIALNAME_H
