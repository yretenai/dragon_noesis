//
// Created by yretenai on 6/2/2020.
//

#include "MaterialName.h"

MaterialName::MaterialName(vector<char> buffer) {
    char* ptr = buffer.data();
    Name = string(ptr, ptr + 0x80);
    ptr += 0x80;
    uint32_t count = reinterpret_cast<uint32_t*>(ptr)[0];
    ptr += 0x24;
    Materials = vector<string>(count);
    for(int i = 0; i < count; i++) {
        string material(ptr);
        Materials[i] = material;
        ptr += material.size() + 1;
    }
}

MaterialName::~MaterialName() {
}
