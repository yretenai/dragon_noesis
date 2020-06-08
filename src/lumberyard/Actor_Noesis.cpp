//
// Created by yretenai on 5/28/2020.
//

#ifdef USE_NOESIS

#include "Actor.h"

using namespace dragon::lumberyard::chunk::emfx;

namespace dragon::lumberyard {
    noesisModel_t* Actor::noesis_load(BYTE* buffer, int length, int& num_mdl, noeRAPI_t* rapi) { return nullptr; }

    bool Actor::noesis_check(BYTE* buffer, int length, [[maybe_unused]] noeRAPI_t* rapi) {
        Array<char> data_buffer = Array<char>(reinterpret_cast<char*>(buffer), length);
        return check(&data_buffer);
    }

} // namespace dragon::lumberyard

#endif
