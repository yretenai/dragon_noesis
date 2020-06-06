//
// Created by yretenai on 6/4/2020.
//

#include "DataStream.h"

namespace dragon::lumberyard::chunk::model {
    DataStream::DataStream(Array<char>* buffer,
                           CRCH_CHUNK_HEADER chunk_header) {
        Chunk = chunk_header;
        super_assert_dragon_log(Chunk.Version == 0x801, "version == 0x801");
        Header = buffer->cast<DATA_STREAM_HEADER>(0);
        Buffer = buffer->slice(sizeof(DATA_STREAM_HEADER),
                               Header.Count * Header.Size);
    }
} // namespace dragon::lumberyard::chunk::model
