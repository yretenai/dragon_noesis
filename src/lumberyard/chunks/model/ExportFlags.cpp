//
// Created by yretenai on 6/2/2020.
//

#include "ExportFlags.h"

namespace dragon::lumberyard::chunk::model {
    ExportFlags::ExportFlags(Array<char>* buffer,
                             CRCH_CHUNK_HEADER chunk_header) {
        Chunk = chunk_header;
        assert(Chunk.Version == 0x1);
        Header = buffer->cast<EXPORT_FLAGS_HEADER>(0);
    }
} // namespace dragon::lumberyard::chunk::model
