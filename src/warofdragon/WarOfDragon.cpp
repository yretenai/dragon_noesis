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
    if (!g_nfn->NPAPI_DebugLogIsOpen())
        g_nfn->NPAPI_PopupDebugLog(0);
#ifndef _DEBUG
    if (std::filesystem::is_regular_file("dragon_warofdragon.log")) {
#endif
        LogStream = new std::ofstream("dragon_warofdragon.log");
#ifndef _DEBUG
    }
#endif
    LOG("v" << DRAGON_VERSION << " (dragon v" << DRAGON_VERSION << ")");
    int handle;
    LOG("Adding File Table Exporter");
    handle = g_nfn->NPAPI_Register(const_cast<char*>("War of Dragon File Table"), const_cast<char*>(".tbl2"));
    g_nfn->NPAPI_SetTypeHandler_ExtractArc(handle, nullptr);
    return true;
}

void NPAPI_ShutdownLocal() {
    if (LogStream != nullptr) {
        delete LogStream;
        LogStream = nullptr;
    }
}

#endif
