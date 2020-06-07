//
// Created by yretenai on 5/31/2020.
//

#include "Lumberyard.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCInconsistentNamingInspection"

#ifdef USE_NOESIS

const char* g_pPluginName = "lumberyard";
const char* g_pPluginDesc = "Lumberyard EMFX Asset Handler by yretenai";

using namespace dragon;
using namespace dragon::lumberyard;

std::filesystem::path* LibraryRoot;

void reset_game_root() {
    if (LibraryRoot != nullptr) {
        delete LibraryRoot;
    }
    LibraryRoot = nullptr;
}

int set_game_root([[maybe_unused]] int handle,
                  [[maybe_unused]] void* user_data) {
    reset_game_root();
    noeUserPromptParam_t prompt = {const_cast<char*>(""), nullptr, nullptr,
                                   NOEUSERVAL_FOLDERPATH};
    if (!g_nfn->NPAPI_UserPrompt(&prompt) || prompt.valBuf[0] == 0) {
        return 0;
    }

    wchar_t* path = reinterpret_cast<wchar_t*>(prompt.valBuf);
    if (!std::filesystem::exists(path)) {
        LOG("Path does not exist");
        return 0;
    }
    LibraryRoot = new std::filesystem::path(path);
    return 1;
}

bool NPAPI_InitLocal() {
    g_nfn->NPAPI_PopupDebugLog(0);
    LOG("v1.0.0 (fmt_dragon v1)");
    LOG("Adding Lumberyard CGF Model handler...");
    int handle =
        g_nfn->NPAPI_Register((char*)"Lumberyard Model", (char*)".cgf");
    g_nfn->NPAPI_SetTypeHandler_LoadModel(handle, Model::noesis_load);
    g_nfn->NPAPI_SetTypeHandler_TypeCheck(handle, Model::noesis_check);

    LOG("Adding Lumberyard DDS Texture handler...");
    handle = g_nfn->NPAPI_Register((char*)"Lumberyard Texture", (char*)".a;.1");
    g_nfn->NPAPI_SetTypeHandler_LoadRGBA(handle, Texture::noesis_load);
    g_nfn->NPAPI_SetTypeHandler_TypeCheck(handle, Texture::noesis_check);

    LOG("Adding Lumberyard DDS Tool handler...");
    handle = g_nfn->NPAPI_RegisterTool((char*)"Cook Lumberyard Texture",
                                       Texture::noesis_tool, nullptr);
    g_nfn->NPAPI_SetToolHelpText(
        handle, (char*)"Merges streamed Lumberyard textures into one DDS");
    g_nfn->NPAPI_SetToolFlags(handle, NTOOLFLAG_CONTEXTITEM);
    g_nfn->NPAPI_SetToolVisibleCallback(handle,
                                        Texture::noesis_tool_visibility);

    LOG("Adding Lumberyard GameRoot tool handler...");
    handle = g_nfn->NPAPI_RegisterTool((char*)"Set Lumberyard Game Directory",
                                       set_game_root, nullptr);
    g_nfn->NPAPI_SetToolHelpText(
        handle, (char*)"Sets the EXTRACTED game root folder, for loading other "
                       "assets like textures.");
    return true;
}

void NPAPI_ShutdownLocal() { reset_game_root(); }

#endif // USE_NOESIS

#pragma clang diagnostic pop
