//
// Created by yretenai on 5/28/2020.
//

#pragma once

#ifndef FMT_LUMBERYARD_ACTOR_H
#define FMT_LUMBERYARD_ACTOR_H

#include "../dragon/dragon.h"
#include "export.h"

namespace dragon::lumberyard {
    class LUMBERYARD_EXPORT Actor {
      public:
        Actor(std::vector<char> buffer);
    };
} // namespace dragon::lumberyard

#endif // FMT_LUMBERYARD_ACTOR_H
