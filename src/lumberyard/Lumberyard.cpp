//
// Created by yretenai on 5/31/2020.
//

#include "Lumberyard.h"

#if defined(USE_NOESIS) && NOESIS_PLUGINAPI_VERSION < 75
#error "Please update the Noesis plugin headers to at least version 75."
#endif

std::ofstream* LogStream;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCInconsistentNamingInspection"

#ifdef USE_NOESIS

const char* g_pPluginName = "lumberyard";
const char* g_pPluginDesc = "Lumberyard EMFX Asset Handler by yretenai";

using namespace dragon;
using namespace dragon::lumberyard;

std::filesystem::path* LibraryRoot;
bool AutoDetect = true;
bool InterpolateAnimation = true;

void reset_game_root() {
    if (LibraryRoot != nullptr) {
        delete LibraryRoot;
        LibraryRoot = nullptr;
    }
}

int set_game_root([[maybe_unused]] int handle, [[maybe_unused]] void* user_data) {
    char defaultValue[MAX_NOESIS_PATH];
    memset(defaultValue, 0, MAX_NOESIS_PATH);
    if (LibraryRoot != nullptr) {
        wcstombs(defaultValue, LibraryRoot->c_str(), LibraryRoot->string().size());
    }
    noeUserPromptParam_t prompt = {const_cast<char*>("Select Game Extraction Directory"),
                                   const_cast<char*>("Select the directory game files have been extracted to automatically load textures"),
                                   defaultValue, NOEUSERVAL_FOLDERPATH};
    bool promptResult = g_nfn->NPAPI_UserPrompt(&prompt);
    reset_game_root();
    if (!promptResult || prompt.valBuf[0] == 0) {
        BYTE buffer[MAX_NOESIS_PATH];
        memset(buffer, 0, MAX_NOESIS_PATH);
        LOG("Cleared library path");
        g_nfn->NPAPI_UserSettingWrite(const_cast<wchar_t*>(L"dragon::lumberyard::LibraryPath"), buffer, MAX_NOESIS_PATH);
        return 1;
    }

    wchar_t* path = reinterpret_cast<wchar_t*>(prompt.valBuf);
    if (!std::filesystem::exists(path)) {
        LOG("Path does not exist");
        return 0;
    }
    LibraryRoot = new std::filesystem::path(path);
    BYTE buffer[MAX_NOESIS_PATH];
    std::copy_n(prompt.valBuf, MAX_NOESIS_PATH, buffer);
    LOG("Set library path to " << *LibraryRoot);
    g_nfn->NPAPI_UserSettingWrite(const_cast<wchar_t*>(L"dragon::lumberyard::LibraryPath"), buffer, MAX_NOESIS_PATH);
    return 1;
}

void load_saved_game_root() {
    BYTE buffer[MAX_NOESIS_PATH];
    if (g_nfn->NPAPI_UserSettingRead(const_cast<wchar_t*>(L"dragon::lumberyard::LibraryPath"), buffer, MAX_NOESIS_PATH)) {
        wchar_t* path = reinterpret_cast<wchar_t*>(buffer);
        if (!std::filesystem::exists(path)) {
            return;
        }
        LibraryRoot = new std::filesystem::path(path);
        LOG("Set library path to " << *LibraryRoot);
    }
}

void get_autodetect(int handle) {
    BYTE buffer[1];
    if (g_nfn->NPAPI_UserSettingRead(const_cast<wchar_t*>(L"dragon::lumberyard::AutoDetect"), buffer, 1)) {
        AutoDetect = buffer[0] == 1;
        LOG("Set auto detect to " << AutoDetect);
    }
    g_nfn->NPAPI_CheckToolMenuItem(handle, AutoDetect);
}

int set_autodetect(int handle, [[maybe_unused]] void* user_data) {
    AutoDetect = !AutoDetect;
    BYTE buffer[1] = {AutoDetect};
    g_nfn->NPAPI_UserSettingWrite(const_cast<wchar_t*>(L"dragon::lumberyard::AutoDetect"), buffer, 1);
    g_nfn->NPAPI_CheckToolMenuItem(handle, AutoDetect);
    return 1;
}

void get_interpolate(int handle) {
    BYTE buffer[1];
    if (g_nfn->NPAPI_UserSettingRead(const_cast<wchar_t*>(L"dragon::lumberyard::InterpolateAnimation"), buffer, 1)) {
        InterpolateAnimation = buffer[0] == 1;
        LOG("Set animation interpolation to " << InterpolateAnimation);
    }
    g_nfn->NPAPI_CheckToolMenuItem(handle, InterpolateAnimation);
}

int set_interpolate(int handle, [[maybe_unused]] void* user_data) {
    InterpolateAnimation = !InterpolateAnimation;
    BYTE buffer[1] = {InterpolateAnimation};
    g_nfn->NPAPI_UserSettingWrite(const_cast<wchar_t*>(L"dragon::lumberyard::InterpolateAnimation"), buffer, 1);
    g_nfn->NPAPI_CheckToolMenuItem(handle, InterpolateAnimation);
    return 1;
}

bool NPAPI_InitLocal() {
    // g_nfn->NPAPI_PopupDebugLog(0);
    if (std::filesystem::exists("fmt_lumberyard.log")) {
        LogStream = new std::ofstream("fmt_lumberyard.log");
    }
    LOG("v" << FMT_LUMBERYARD_VERSION << " (fmt_dragon v" << FMT_DRAGON_VERSION << ")");
    int handle;
    if ((g_nfn->NPAPI_GetFormatExtensionFlags(const_cast<wchar_t*>(L".cgf"), nullptr) & NFORMATFLAG_MODELREAD) == 0) {
        LOG("Adding Lumberyard CGF Model handler...");
        handle = g_nfn->NPAPI_Register((char*)"Lumberyard/CryEngine Model", (char*)".cgf");
        g_nfn->NPAPI_SetTypeHandler_LoadModel(handle, Model::noesis_load);
        g_nfn->NPAPI_SetTypeHandler_TypeCheck(handle, Model::noesis_check);
    } else {
        LOG("Lumberyard handler already present, skipping...");
    }

    LOG("Adding Lumberyard EMFX Actor handler...");
    handle = g_nfn->NPAPI_Register((char*)"Lumberyard EMFX Actor", (char*)".actor");
    g_nfn->NPAPI_SetTypeHandler_LoadModel(handle, Actor::noesis_load);
    g_nfn->NPAPI_SetTypeHandler_TypeCheck(handle, Actor::noesis_check);

    LOG("Adding Lumberyard EMFX Motion handler...");
    handle = g_nfn->NPAPI_Register((char*)"Lumberyard EMFX Motion", (char*)".motion");
    g_nfn->NPAPI_SetTypeHandler_LoadModel(handle, Actor::noesis_load);
    g_nfn->NPAPI_SetTypeHandler_TypeCheck(handle, Actor::noesis_check);

    LOG("Adding Lumberyard DDS Texture handler...");
    handle = g_nfn->NPAPI_Register((char*)"Lumberyard Texture", (char*)".a;.1");
    g_nfn->NPAPI_SetTypeHandler_LoadRGBA(handle, Texture::noesis_load);
    g_nfn->NPAPI_SetTypeHandler_TypeCheck(handle, Texture::noesis_check);

    LOG("Adding Lumberyard EMFX Motion Tool handler...");
    handle = g_nfn->NPAPI_RegisterTool((char*)"Lumberyard - Append EMFX Motion", Animation::noesis_load, nullptr);
    g_nfn->NPAPI_SetToolHelpText(handle, (char*)"Loads Lumberyard EMFX Motion to the selected model");
    g_nfn->NPAPI_SetToolFlags(handle, NTOOLFLAG_CONTEXTITEM);
    g_nfn->NPAPI_SetToolVisibleCallback(handle, Animation::noesis_check);

    LOG("Adding Lumberyard DDS Tool handler...");
    handle = g_nfn->NPAPI_RegisterTool((char*)"Lumberyard - Cook Texture", Texture::noesis_tool, nullptr);
    g_nfn->NPAPI_SetToolHelpText(handle, (char*)"Merges streamed Lumberyard textures into one DDS");
    g_nfn->NPAPI_SetToolFlags(handle, NTOOLFLAG_CONTEXTITEM);
    g_nfn->NPAPI_SetToolVisibleCallback(handle, Texture::noesis_tool_visibility);

    LOG("Adding Lumberyard GameRoot tool handler...");
    handle = g_nfn->NPAPI_RegisterTool((char*)"Lumberyard - Set Extracted Game Directory", set_game_root, nullptr);
    g_nfn->NPAPI_SetToolHelpText(handle, (char*)"Sets the EXTRACTED game root folder, for loading other "
                                                "assets like textures.");
    load_saved_game_root();

    LOG("Adding Lumberyard Auto Detect Material tool handler...");
    handle = g_nfn->NPAPI_RegisterTool((char*)"Lumberyard - Auto Detect Material", set_autodetect, nullptr);
    g_nfn->NPAPI_SetToolFlags(handle, NTOOLFLAG_USERBITS);
    g_nfn->NPAPI_SetToolHelpText(handle, (char*)"Automatically tries to find relevant MTL files");
    get_autodetect(handle);

    LOG("Adding Lumberyard Interpolate Animation tool handler...");
    handle = g_nfn->NPAPI_RegisterTool((char*)"Lumberyard - Interpolate Animation", set_interpolate, nullptr);
    g_nfn->NPAPI_SetToolFlags(handle, NTOOLFLAG_USERBITS);
    g_nfn->NPAPI_SetToolHelpText(handle, (char*)"Interpolate animation frames");
    get_interpolate(handle);
    return true;
}

void NPAPI_ShutdownLocal() {
    if (LogStream != nullptr) {
        delete LogStream;
        LogStream = nullptr;
    }
    reset_game_root();
}

#endif // USE_NOESIS

#pragma clang diagnostic pop
