//
// Created by yretenai on 5/28/2020.
//

#pragma once

#ifndef DRAGON_LUMBERYARD_ANIMATION_H
#define DRAGON_LUMBERYARD_ANIMATION_H

#include "standard_dragon/dragon.h"
#include "chunk/emfx/EMFXChunks.h"
#include "export.h"

#define FOURCC_MOT (MAKEFOURCC('M', 'O', 'T', ' '))
#define FOURCC_MOTW (MAKEFOURCC('M', 'O', 'T', 'W'))
namespace dragon::lumberyard {
    class LUMBERYARD_EXPORT Animation {
      public:
        Animation();
        Animation(Array<char>* buffer);

        chunk::emfx::MOTION_HEADER Header;
        std::vector<std::shared_ptr<chunk::emfx::AbstractEMFXChunk>> Chunks;

        void get_chunks_of_type(chunk::emfx::MOTION_CHUNK_TYPE type, std::vector<std::shared_ptr<chunk::emfx::AbstractEMFXChunk>>* chunks);
        std::shared_ptr<chunk::emfx::AbstractEMFXChunk> get_chunk(chunk::emfx::MOTION_CHUNK_TYPE type);

        static bool check(Array<char>* buffer);

        static VECTOR4_SINGLE uncompress_quaternion(VECTOR4_SHORT compressed);

#ifdef USE_NOESIS

        static int noesis_load([[maybe_unused]] int handle, [[maybe_unused]] void* user_data);

        static int noesis_check([[maybe_unused]] int handle, const wchar_t* path, [[maybe_unused]] void* resv_a, [[maybe_unused]] void* resv_b);

        static void insert_key(chunk::emfx::MOTION_VECTOR3_KEY key, RichVec3 bind, bool isAdditive, bool multiply, noeKeyFramedBone_t& bone,
                               noeKeyFrameData_t& frame, std::vector<float>& floats, uint32_t& floatIndex);

        static void insert_key(chunk::emfx::MOTION_VECTOR4_KEY key, RichMat43 bind, bool isAdditive, noeKeyFramedBone_t& bone,
                               noeKeyFrameData_t& frame, std::vector<float>& floats, uint32_t& floatIndex);

        static int noesis_inject_anim(noesisModel_t* model, std::string& animName, Animation anim, noeRAPI_t* rapi);

#endif // USE_NOESIS
    };
} // namespace dragon::lumberyard

#endif // DRAGON_LUMBERYARD_ANIMATION_H
