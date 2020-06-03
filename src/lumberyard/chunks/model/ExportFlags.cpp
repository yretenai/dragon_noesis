//
// Created by yretenai on 6/2/2020.
//

#include "ExportFlags.h"

ExportFlags::ExportFlags(vector<char> buffer) {
    char* ptr = buffer.data();
    Header = vector_cast<EXPORT_FLAGS_HEADER>(ptr);
}
