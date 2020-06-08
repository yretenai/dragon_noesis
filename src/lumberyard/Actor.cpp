//
// Created by yretenai on 5/28/2020.
//

#include "Actor.h"

#include "Material.h"
#include "Texture.h"
#include "globals.h"
#include <memory>

using namespace dragon::lumberyard::chunk::emfx;

namespace dragon::lumberyard {
    Actor::Actor(Array<char>* buffer) {
        assert(check(buffer));
    }

    bool Actor::get_chunk_header(CHUNK_ID id, EMFX_CHUNK_HEADER& chunk) { return false; }

    void Actor::get_chunks_of_type(ACTOR_CHUNK_TYPE type, std::vector<std::shared_ptr<AbstractEMFXChunk>>* chunks) {}

    bool Actor::check(Array<char>* buffer) {
        if (buffer->size() < sizeof(ACTOR_HEADER))
            return false;
        ACTOR_HEADER header = buffer->cast<ACTOR_HEADER>(0);
        return buffer->size() >= sizeof(ACTOR_HEADER) && header.Magic == FOURCC_ACTR && header.IsBigEndian == 0;
    }

#ifdef USE_NOESIS

    noesisModel_t* Actor::noesis_load(BYTE* buffer, int length, int& num_mdl, noeRAPI_t* rapi) { return nullptr; }

    bool Actor::noesis_check(BYTE* buffer, int length, [[maybe_unused]] noeRAPI_t* rapi) {
        Array<char> data_buffer = Array<char>(reinterpret_cast<char*>(buffer), length);
        return check(&data_buffer);
    }

    int Actor::noesis_create_texture(std::filesystem::path texturePath, CArrayList<noesisTex_t*>& texList, noeRAPI_t* rapi) {
        std::filesystem::path combinedPath = *LibraryRoot / texturePath;
        combinedPath.replace_extension(".dds.1");
        if (!std::filesystem::exists(combinedPath)) {
            return -1;
        }
        int num = texList.Num();
        Texture::noesies_load_direct(combinedPath, texList, rapi);
        return num;
    }

#endif
} // namespace dragon::lumberyard
