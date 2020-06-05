//
// Created by yretenai on 5/28/2020.
//

#include "dragon.h"

namespace dragon {
    FILE* DragonLog;

#define _CRT_SECURE_NO_WARNINGS

    void open_dragon_log() {
        if (DragonLog == nullptr || DragonLog == stdout) {
            DragonLog = fopen("fmt_dragon.log", "w");
        }
    }

    void close_dragon_log() {
        if (DragonLog != nullptr && DragonLog != stdout) {
            fclose(DragonLog);
        }
    }

    void flush_dragon_log() {
        if (DragonLog != nullptr) {
            fflush(DragonLog);
        }
    }

    void open_dragon_log_stdout() {
        if (DragonLog != nullptr) {
            flush_dragon_log();
            close_dragon_log();
        }
        DragonLog = stdout;
    }

    void write_dragon_log(const char* fmt, ...) {
        if (DragonLog != nullptr) {
            va_list args;
            va_start(args, fmt);
            vfprintf(DragonLog, fmt, args);
            va_end(args);
        }
    }

    void assert_dragon_log(bool check, const char* error) {
        if (!check) {
            write_dragon_log(error);
        }
    }

    void super_assert_dragon_log(bool check, const char* error) {
        if (!check) {
            write_dragon_log(error);
            flush_dragon_log();
            close_dragon_log();
#ifdef WIN32
            throw std::exception(error);
#else
            throw std::exception();
#endif
        }
    }
} // namespace dragon
