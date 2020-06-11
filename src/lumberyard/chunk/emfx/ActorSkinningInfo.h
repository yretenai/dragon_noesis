//
// Created by yretenai on 2020-06-10.
//

#ifndef FMT_DRAGON_ACTORSKINNINGINFO_H
#define FMT_DRAGON_ACTORSKINNINGINFO_H

#include "AbstractEMFXChunk.h"

namespace dragon::lumberyard::chunk::emfx {
    class LUMBERYARD_EXPORT ActorSkinningInfo : public AbstractEMFXChunk {
      public:
        ActorSkinningInfo(Array<char>* buffer, EMFX_CHUNK_HEADER header, std::vector<std::shared_ptr<chunk::emfx::AbstractEMFXChunk>> meshChunks,
                          int& ptr);

        ACTOR_SKINNING_INFO_v1_HEADER Header;

        Array<ACTOR_SKINNING_INFO_v1_INFLUENCE> Influences;
        Array<ACTOR_SKINNING_INFO_v1_ENTRY> Table;
    };
} // namespace dragon::lumberyard::chunk::emfx

#endif // FMT_DRAGON_ACTORSKINNINGINFO_H
