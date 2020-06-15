//
// Created by yretenai on 2020-06-13.
//

#include "Actor.h"
#include "Animation.h"
#include "globals.h"

#define CAST_ABSTRACT_CHUNK(target, chunk) (std::reinterpret_pointer_cast<target>(chunk).get())
using namespace dragon::lumberyard::chunk::emfx;

namespace dragon::lumberyard {
    int Animation::noesis_load([[maybe_unused]] int handle, [[maybe_unused]] void* user_data) {
        noeRAPI_t* rapi = g_nfn->NPAPI_GetPreviewRAPI();
        if (rapi == nullptr) {
            return 0;
        }
        wchar_t path[MAX_NOESIS_PATH];
        g_nfn->NPAPI_GetSelectedFile(path);
        if (wcslen(path) < 2) {
            return 0;
        }
        std::filesystem::path animationPath(path);
        Array<char> dataBuffer = read_file(animationPath);
        std::string animName = animationPath.filename().replace_extension("").string();
        Animation anim(&dataBuffer);

        noesisModel_t* model = rapi->Noesis_GetLoadedModel(rapi->Noesis_GetSelectedPreviewModel());
        if (model == nullptr) {
            return 0;
        }

        return noesis_inject_anim(model, animName, anim, rapi);
    }

    int Animation::noesis_inject_anim(noesisModel_t* model, std::string& animName, Animation anim, noeRAPI_t* rapi) {
        MotionInfo* animInfo = CAST_ABSTRACT_CHUNK(MotionInfo, anim.get_chunk(MOTION_CHUNK_TYPE::Info));
        MotionSubMotions* subMotions = CAST_ABSTRACT_CHUNK(MotionSubMotions, anim.get_chunk(MOTION_CHUNK_TYPE::SubMotions));
        if (animInfo == nullptr || subMotions == nullptr) {
            return 0;
        }
        std::vector<void*> buffers;
        sharedModel_t* shared = rapi->rpgGetSharedModel(model, 0);
        CNoeCustomData* actorPresence = shared->mCustomData.FindCustomDataByType("dragon::lumberyard::ActorPresence");
        if (actorPresence == nullptr) {
            return 0;
        }
        noeKeyFramedAnim_t* keyFramedAnim = static_cast<noeKeyFramedAnim_t*>(rapi->Noesis_UnpooledAlloc(sizeof(noeKeyFramedAnim_t)));
        buffers.push_back(keyFramedAnim);
        memset(keyFramedAnim, 0, sizeof(noeKeyFramedAnim_t));
        keyFramedAnim->name = rapi->Noesis_PooledString(const_cast<char*>(animName.c_str()));
        keyFramedAnim->numBones = shared->numBones;
        keyFramedAnim->framesPerSecond = 30.0f;
        keyFramedAnim->flags = KFANIMFLAG_SEPARATETS | KFANIMFLAG_USEBONETIMES | KFANIMFLAG_PLUSONE;
        std::vector<float> floats;
        noeKeyFramedBone_t* keyFramedBones =
            static_cast<noeKeyFramedBone_t*>(rapi->Noesis_UnpooledAlloc(sizeof(noeKeyFramedBone_t) * subMotions->Motions.size()));
        memset(keyFramedBones, 0, sizeof(noeKeyFramedBone_t) * subMotions->Motions.size());
        buffers.push_back(keyFramedBones);
        uint32_t actualIndex = 0;
        uint32_t floatIndex = 0;
        std::map<std::string, uint32_t> boneMap;
        for (uint32_t i = 0; i < shared->numBones; i++) {
            boneMap[std::string(shared->bones[i].name)] = i;
        }
        noeKeyFrameInterpolation_e interpolation = NOEKF_INTERPOLATE_NEAREST;
        if (InterpolateAnimation) {
            interpolation = NOEKF_INTERPOLATE_LINEAR;
        }
        for (uint32_t i = 0; i < subMotions->Motions.size(); i++) {
            MotionSubMotion* motion = subMotions->Motions[i].get();
            if (boneMap.find(motion->Name) == boneMap.end()) {
                continue;
            }
            noeKeyFramedBone_t* boneKey = &keyFramedBones[actualIndex];
            boneKey->boneIndex = boneMap[motion->Name];
            boneKey->translationType = NOEKF_TRANSLATION_VECTOR_3;
            boneKey->rotationType = NOEKF_ROTATION_QUATERNION_4;
            boneKey->scaleType = NOEKF_SCALE_VECTOR_3;
            boneKey->translationInterpolation = interpolation;
            boneKey->rotationInterpolation = interpolation;
            boneKey->scaleInterpolation = interpolation;
            boneKey->numTranslationKeys = motion->Positions.size() + 1;
            boneKey->numRotationKeys = motion->Rotations.size() + 1;
            boneKey->numScaleKeys = motion->Scales.size() + 1;
            boneKey->minTime = 0.0f;
            boneKey->maxTime = 0.0f;
            uint32_t frameOffset = 1;
            bool isAdditive = animInfo->Header.IsAdditive == 1;
            RichVec3 bindPos(motion->Header.BindPosition.X, motion->Header.BindPosition.Y, motion->Header.BindPosition.Z);
            VECTOR4_SINGLE bindRotation = uncompress_quaternion(motion->Header.BindRotation);
            RichMat43 bindRot = RichQuat(bindRotation.X, bindRotation.Y, bindRotation.Z, bindRotation.W).GetTranspose().ToMat43();
            RichVec3 bindScale(motion->Header.BindScale.X, motion->Header.BindScale.Y, motion->Header.BindScale.Z);
            if (boneKey->numTranslationKeys > 0) {
                noeKeyFrameData_t* posKeyframes =
                    static_cast<noeKeyFrameData_t*>(rapi->Noesis_UnpooledAlloc(sizeof(noeKeyFrameData_t) * boneKey->numTranslationKeys));
                memset(posKeyframes, 0, sizeof(noeKeyFrameData_t) * boneKey->numTranslationKeys);
                buffers.push_back(posKeyframes);

                MOTION_VECTOR3_KEY refKey = {motion->Header.RefPosition, 0};
                insert_key(refKey, bindPos, isAdditive, false, keyFramedBones[actualIndex], posKeyframes[0], floats, floatIndex);

                for (uint32_t j = 0; j < boneKey->numTranslationKeys - frameOffset; j++) {
                    MOTION_VECTOR3_KEY key = motion->Positions[j];
                    insert_key(key, bindPos, isAdditive, false, keyFramedBones[actualIndex], posKeyframes[j + frameOffset], floats, floatIndex);
                }
                boneKey->translationKeys = posKeyframes;
            }
            if (boneKey->numRotationKeys > 0) {
                noeKeyFrameData_t* rotKeyframes =
                    static_cast<noeKeyFrameData_t*>(rapi->Noesis_UnpooledAlloc(sizeof(noeKeyFrameData_t) * boneKey->numRotationKeys));
                memset(rotKeyframes, 0, sizeof(noeKeyFrameData_t) * boneKey->numRotationKeys);
                buffers.push_back(rotKeyframes);

                MOTION_VECTOR4_KEY refKey = {motion->Header.RefRotation, 0};
                insert_key(refKey, bindRot, isAdditive, keyFramedBones[actualIndex], rotKeyframes[0], floats, floatIndex);

                for (uint32_t j = 0; j < boneKey->numRotationKeys - frameOffset; j++) {
                    MOTION_VECTOR4_KEY key = motion->Rotations[j];
                    insert_key(key, bindRot, isAdditive, keyFramedBones[actualIndex], rotKeyframes[j + frameOffset], floats, floatIndex);
                }
                boneKey->rotationKeys = rotKeyframes;
            }
            if (boneKey->numScaleKeys > 0) {
                noeKeyFrameData_t* scaleKeyframes =
                    static_cast<noeKeyFrameData_t*>(rapi->Noesis_UnpooledAlloc(sizeof(noeKeyFrameData_t) * boneKey->numScaleKeys));
                memset(scaleKeyframes, 0, sizeof(noeKeyFrameData_t) * boneKey->numScaleKeys);
                buffers.push_back(scaleKeyframes);

                MOTION_VECTOR3_KEY refKey = {motion->Header.RefScale, 0};
                insert_key(refKey, bindScale, isAdditive, true, keyFramedBones[actualIndex], scaleKeyframes[0], floats, floatIndex);

                for (uint32_t j = 0; j < boneKey->numScaleKeys - frameOffset; j++) {
                    MOTION_VECTOR3_KEY key = motion->Scales[j];
                    insert_key(key, bindScale, isAdditive, true, keyFramedBones[actualIndex], scaleKeyframes[j + frameOffset], floats, floatIndex);
                }
                boneKey->scaleKeys = scaleKeyframes;
            }
            actualIndex++;
        }
        keyFramedAnim->numKfBones = actualIndex;
        keyFramedAnim->numDataFloats = floats.size();
        float* floatBuffer = static_cast<float*>(rapi->Noesis_UnpooledAlloc(sizeof(float) * keyFramedAnim->numDataFloats));
        buffers.push_back(floatBuffer);
        std::copy_n(floats.begin(), floats.size(), floatBuffer);
        keyFramedAnim->data = floatBuffer;
        keyFramedAnim->kfBones = keyFramedBones;

        noesisAnim_t* noesisAnim = rapi->Noesis_AnimFromBonesAndKeyFramedAnim(shared->bones, shared->numBones, keyFramedAnim, true);

        if (shared->animData != nullptr && shared->animData->dataLen > 0) {
            CArrayList<noesisAnim_t*> anims;
            anims.Append(shared->animData);
            anims.Append(noesisAnim);
            noesisAnim = rapi->Noesis_AnimFromAnimsList(anims, 2);
        }
        // Why can we set multiple animations when Noesis only shows one?
        rapi->Noesis_SetModelAnims(model, noesisAnim, 1);
        for (void* noesis_buffer : buffers) {
            rapi->Noesis_UnpooledFree(noesis_buffer);
        }
        return 1;
    }

    int Animation::noesis_check([[maybe_unused]] int handle, const wchar_t* path, [[maybe_unused]] void* resv_a, [[maybe_unused]] void* resv_b) {
        if (path == nullptr)
            return false;

        if (wcslen(path) < 7)
            return false;

        std::filesystem::path wpath(path);
        noeRAPI_t* rapi = g_nfn->NPAPI_GetPreviewRAPI();
        if (!wpath.has_extension() || wpath.extension() != ".motion" || rapi == nullptr)
            return false;

        Array<char> data = read_file(wpath);
        return Animation::check(&data);
    }

    void Animation::insert_key(chunk::emfx::MOTION_VECTOR3_KEY key, RichVec3 bind, bool isAdditive, bool multiply, noeKeyFramedBone_t& bone,
                               noeKeyFrameData_t& frame, std::vector<float>& floats, uint32_t& floatIndex) {
        frame.dataIndex = floatIndex;
        frame.time = key.Time;
        if (frame.time > bone.maxTime) {
            bone.maxTime = frame.time;
        }
        floats.resize(floatIndex + 3);
        RichVec3 vec(key.Value.X, key.Value.Y, key.Value.Z);
        if (isAdditive) {
            if (multiply) {
                vec *= bind;
            } else {
                vec += bind;
            }
        }
        floats[floatIndex] = vec.v[0];
        floats[floatIndex + 1] = vec.v[1];
        floats[floatIndex + 2] = vec.v[2];
        floatIndex += 3;
    }

    void Animation::insert_key(chunk::emfx::MOTION_VECTOR4_KEY key, RichMat43 bind, bool isAdditive, noeKeyFramedBone_t& bone,
                               noeKeyFrameData_t& frame, std::vector<float>& floats, uint32_t& floatIndex) {
        frame.dataIndex = floatIndex;
        frame.time = key.Time;
        if (frame.time > bone.maxTime) {
            bone.maxTime = frame.time;
        }
        floats.resize(floatIndex + 4);
        VECTOR4_SINGLE rotation = Animation::uncompress_quaternion(key.Value);
        RichMat43 mat = RichQuat(rotation.X, rotation.Y, rotation.Z, rotation.W).GetTranspose().ToMat43();
        if (isAdditive) {
            mat *= bind;
        }
        RichQuat rot = mat.ToQuat();
        floats[floatIndex] = rot[0];
        floats[floatIndex + 1] = rot[1];
        floats[floatIndex + 2] = rot[2];
        floats[floatIndex + 3] = rot[3];
        floatIndex += 4;
        floatIndex += 3;
    }
} // namespace dragon::lumberyard