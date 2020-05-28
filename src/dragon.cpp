//
// Created by yretenai on 5/28/2020.
//

#include "dragon.h"

FILE* DragonLog;

vector<char> read_file(filesystem::path path) {
    ifstream file(path, ios::binary | ios::ate | ios::in);
    ifstream::pos_type pos = file.tellg();
    std::vector<char> bytes(pos);
    file.seekg(0, ios::beg);
    file.read(bytes.data(), pos);
    return bytes;
}

void write_file(filesystem::path path, vector<char> buffer) {
    if(buffer.empty())
        return;
    ofstream file(path, ios::binary | ios::out | ios::trunc);
    file.write(buffer.data(), buffer.size());
}
