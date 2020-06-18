//
// Created by yretenai on 5/31/2020.
//

#include "Lumberyard.h"

std::ofstream* LogStream;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCInconsistentNamingInspection"

#ifdef USE_NOESIS

#if NOESIS_PLUGINAPI_VERSION < 75
#error "Please update the Noesis plugin headers to at least version 75."
#endif

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
    if (!std::filesystem::is_directory(path)) {
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
        if (!std::filesystem::is_directory(path)) {
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
    if (!g_nfn->NPAPI_DebugLogIsOpen())
        g_nfn->NPAPI_PopupDebugLog(0);
    if (std::filesystem::is_regular_file("dragon_lumberyard.log"))
        LogStream = new std::ofstream("dragon_lumberyard.log");
    LOG("🐲");
    LOG("v" << DRAGON_LUMBERYARD_VERSION << " (dragon v" << DRAGON_VERSION << ")");
    int handle;
    // context tools
    LOG("Adding EMFX Motion Tool...");
    handle = g_nfn->NPAPI_RegisterTool(const_cast<char*>("Lumberyard - Append EMFX Motion"), Animation::noesis_load, nullptr);
    g_nfn->NPAPI_SetToolHelpText(handle, const_cast<char*>("Loads Lumberyard EMFX Motion to the selected model"));
    g_nfn->NPAPI_SetToolFlags(handle, NTOOLFLAG_CONTEXTITEM);
    g_nfn->NPAPI_SetToolVisibleCallback(handle, Animation::noesis_check);

    LOG("Adding Cook DDS Tool...");
    handle = g_nfn->NPAPI_RegisterTool(const_cast<char*>("Lumberyard - Cook Texture"), Texture::noesis_tool, nullptr);
    g_nfn->NPAPI_SetToolHelpText(handle, const_cast<char*>("Merges streamed Lumberyard textures into one DDS"));
    g_nfn->NPAPI_SetToolFlags(handle, NTOOLFLAG_CONTEXTITEM);
    g_nfn->NPAPI_SetToolVisibleCallback(handle, Texture::noesis_tool_visibility);

    // Setting checkboxes -- I wish noesis had a dedicated interface for this.
    LOG("Adding Extracted Game Directory Setting...");
    handle = g_nfn->NPAPI_RegisterTool(const_cast<char*>("Lumberyard - Set Extracted Game Directory"), set_game_root, nullptr);
    g_nfn->NPAPI_SetToolHelpText(handle, const_cast<char*>("Sets the EXTRACTED game root folder, for loading other assets like textures."));
    load_saved_game_root();

    LOG("Adding Auto Detect Material Setting...");
    handle = g_nfn->NPAPI_RegisterTool(const_cast<char*>("Lumberyard - Auto Detect Paired File"), set_autodetect, nullptr);
    g_nfn->NPAPI_SetToolHelpText(handle, const_cast<char*>("Automatically tries to find relevant ACTOR/MTL files"));
    get_autodetect(handle);

    LOG("Adding Interpolate Animation Setting...");
    handle = g_nfn->NPAPI_RegisterTool(const_cast<char*>("Lumberyard - Interpolate Animation"), set_interpolate, nullptr);
    g_nfn->NPAPI_SetToolHelpText(handle, const_cast<char*>("Interpolate animation frames"));
    get_interpolate(handle);

    // Model handlers
    LOG("Adding CGF Model Importer...");
    handle = g_nfn->NPAPI_Register(const_cast<char*>("Lumberyard/CryEngine Model"), const_cast<char*>(".cgf"));
    g_nfn->NPAPI_SetTypeHandler_LoadModel(handle, Model::noesis_load);
    g_nfn->NPAPI_SetTypeHandler_TypeCheck(handle, Model::noesis_check);

    LOG("Adding EMFX Actor Importer...");
    handle = g_nfn->NPAPI_Register(const_cast<char*>("Lumberyard EMFX Actor"), const_cast<char*>(".actor"));
    g_nfn->NPAPI_SetTypeHandler_LoadModel(handle, Actor::noesis_load);
    g_nfn->NPAPI_SetTypeHandler_TypeCheck(handle, Actor::noesis_check);

    LOG("Adding EMFX Motion Importer...");
    handle = g_nfn->NPAPI_Register(const_cast<char*>("Lumberyard EMFX Motion"), const_cast<char*>(".motion"));
    g_nfn->NPAPI_SetTypeHandler_LoadModel(handle, Actor::noesis_load);
    g_nfn->NPAPI_SetTypeHandler_TypeCheck(handle, Actor::noesis_check);

    // Texture handlers
    LOG("Adding DDS Texture Importer...");
    handle = g_nfn->NPAPI_Register(const_cast<char*>("Lumberyard Texture"), const_cast<char*>(".a;.1"));
    g_nfn->NPAPI_SetTypeHandler_LoadRGBA(handle, Texture::noesis_load);
    g_nfn->NPAPI_SetTypeHandler_TypeCheck(handle, Texture::noesis_check);
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
