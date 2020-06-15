//
// Created by yretenai on 2020-06-15.
//

#include "WarOfDragon.h"

std::ofstream* LogStream;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCInconsistentNamingInspection"

#ifdef USE_NOESIS

const char* g_pPluginName = "warofdragon";
const char* g_pPluginDesc = "War of Dragon Asset Handler by yretenai";

using namespace dragon;

bool NPAPI_InitLocal() {
#ifndef _DEBUG
    if (std::filesystem::is_regular_file("fmt_warofdragon.log")) {
#endif
        LogStream = new std::ofstream("fmt_warofdragon.log");
#ifndef _DEBUG
    } else {
        g_nfn->NPAPI_PopupDebugLog(0);
    }
#endif
    LOG("v" << FMT_DRAGON_VERSION << " (fmt_dragon v" << FMT_DRAGON_VERSION << ")");
    return true;
}

void NPAPI_ShutdownLocal() {
    if (LogStream != nullptr) {
        delete LogStream;
        LogStream = nullptr;
    }
}

#endif
