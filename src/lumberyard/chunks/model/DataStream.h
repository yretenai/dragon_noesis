//
// Created by yretenai on 6/4/2020.
//

#pragma once

#ifndef FMT_DRAGON_DATASTREAM_H
#define FMT_DRAGON_DATASTREAM_H

#include "AbstractModelChunk.h"

namespace dragon::lumberyard::chunk::model {
    class LUMBERYARD_EXPORT DataStream : public AbstractModelChunk {
      public:
        DataStream(Array<char>* buffer, CRCH_CHUNK_HEADER chunk_header);

        DATA_STREAM_HEADER Header;
        Array<char> Buffer;
    };
} // namespace dragon::lumberyard::chunk::model

#endif // FMT_DRAGON_DATASTREAM_H
