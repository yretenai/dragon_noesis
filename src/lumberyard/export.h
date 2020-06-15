//
// Created by yretenai on 6/4/2020.
//

#pragma once

#ifndef DRAGON_LUMBERYARD_EXPORT_H
#define DRAGON_LUMBERYARD_EXPORT_H
#ifdef _WIN32
#include "export_win32.h"
#else
#include "export_clang.h"
#endif
#endif