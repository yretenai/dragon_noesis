//
// Created by yretenai on 6/2/2020.
//

#include "ExportFlags.h"

namespace dragon::lumberyard::chunk::model {
    ExportFlags::ExportFlags(std::vector<char> buffer,
                             CRCH_CHUNK_HEADER chunk_header) {
        Chunk = chunk_header;
        super_assert_dragon_log(Chunk.Version == 0x1, "version == 0x1");
        char* ptr = buffer.data();
        Header = vector_cast<EXPORT_FLAGS_HEADER>(ptr);
    }
} // namespace dragon::lumberyard::chunk::model
