//
// Created by yretenai on 6/2/2020.
//

#include "ExportFlags.h"

namespace dragon::lumberyard::chunk::model {
    ExportFlags::ExportFlags(vector<char> buffer, uint32_t version) {
        super_assert_dragon_log(version == 0x800, "version == 0x800");
        char* ptr = buffer.data();
        Header = vector_cast<EXPORT_FLAGS_HEADER>(ptr);
    }
}
