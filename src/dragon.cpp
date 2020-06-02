//
// Created by yretenai on 5/28/2020.
//

#include "dragon.h"

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

FILE* DragonLog;

void open_dragon_log() {
    if((DragonLog == nullptr || DragonLog == stdout) && !fopen_s(&DragonLog, "fmt_dragon.log", "w")) {
        DragonLog = nullptr;
    }
}

void close_dragon_log() {
    if(DragonLog != nullptr && DragonLog != stdout) {
        fclose(DragonLog);
    }
}

void flush_dragon_log() {
    if(DragonLog != nullptr) {
        fflush(DragonLog);
    }
}

void open_dragon_log_stdout() {
    if(DragonLog != nullptr) {
        flush_dragon_log();
        close_dragon_log();
    }
    DragonLog = stdout;
}

void write_dragon_log(const char* fmt, ...) {
    if(DragonLog != nullptr) {
        va_list args;
                va_start(args, fmt);
        vfprintf(DragonLog, fmt, args);
                va_end(args);
    }
}

void assert_dragon_log(bool check, const char* fmt, ...) {
    if(!check) {
        va_list args;
                va_start(args, fmt);
        vfprintf(DragonLog, fmt, args);
                va_end(args);
    }
}

void super_assert_dragon_log(bool check, const char* error, const char* fmt, ...) {
    if(!check) {
        va_list args;
                va_start(args, fmt);
        vfprintf(DragonLog, fmt, args);
                va_end(args);
        flush_dragon_log();
        close_dragon_log();
        throw exception(error);
    }
}
