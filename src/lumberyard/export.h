//
// Created by yretenai on 6/4/2020.
//

#pragma once

#ifndef FMT_LUMBERYARD_EXPORT_H
#define FMT_LUMBERYARD_EXPORT_H
#if WIN32
#include "export_win32.h"
#else
#include "export_clang.h"
#endif
#endif