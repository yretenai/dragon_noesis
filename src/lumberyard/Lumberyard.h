//
// Created by yretenai on 5/29/2020.
//

#pragma once

#ifndef FMT_LUMBERYARD_LUMBERYARD_H
#define FMT_LUMBERYARD_LUMBERYARD_H

#ifdef LIBRARY_NAME
#undef LIBRARY_NAME
#endif
#define LIBRARY_NAME "fmt_lumberyard"

#include "../dragon/dragon.h"

extern std::filesystem::path* LibraryRoot;

#include "Actor.h"
#include "Animation.h"
#include "Model.h"
#include "Texture.h"

#endif // FMT_LUMBERYARD_LUMBERYARD_H
