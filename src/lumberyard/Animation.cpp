//
// Created by yretenai on 5/28/2020.
//

#include "Animation.h"

#include "globals.h"
#include <memory>
#include <xmmintrin.h>

#define CAST_EMFX_CHUNK(chunk) (std::shared_ptr<AbstractEMFXChunk>(reinterpret_cast<AbstractEMFXChunk*>(chunk)))

using namespace dragon::lumberyard::chunk::emfx;

namespace dragon::lumberyard {
    Animation::Animation() {}

    Animation::Animation(Array<char>* baseBuffer) {
        assert(check(baseBuffer));

        Header = baseBuffer->cast<MOTION_HEADER>(0);
        int ptr = 0;
        Array<char> buffer = baseBuffer->slice(sizeof(MOTION_HEADER), baseBuffer->size() - sizeof(MOTION_HEADER));
        while (ptr < buffer.size()) {
            EMFX_CHUNK_HEADER chunkHeader = buffer.lpcast<EMFX_CHUNK_HEADER>(&ptr);
            if (chunkHeader.Type >= (CHUNK_TYPE)MOTION_CHUNK_TYPE::END) {
                LOG("Chunk Id is out range, aborting.");
                return;
            }
            Array<char> slice = buffer.shift(ptr);
            int localPtr = 0;
            switch ((MOTION_CHUNK_TYPE)chunkHeader.Type) {
            // case MOTION_CHUNK_TYPE::MotionEventTable: break;
            // case MOTION_CHUNK_TYPE::SubMotion: break;
            case MOTION_CHUNK_TYPE::Info:
                LOG("Found Info");
                Chunks.push_back(CAST_EMFX_CHUNK(new MotionInfo(&slice, chunkHeader, localPtr)));
                break;
            case MOTION_CHUNK_TYPE::SubMotions:
                LOG("Found SubMotions");
                Chunks.push_back(CAST_EMFX_CHUNK(new MotionSubMotions(&slice, chunkHeader, localPtr)));
                break;
            default:
                LOG("Unhandled chunk type " << chunkHeader.Type);
                localPtr = chunkHeader.Size;
                break;
            }

            ptr += localPtr;
        }
    }

    void Animation::get_chunks_of_type(MOTION_CHUNK_TYPE type, std::vector<std::shared_ptr<AbstractEMFXChunk>>* chunks) {
        for (uint32_t i = 0; i < Chunks.size(); i++) {
            if (static_cast<MOTION_CHUNK_TYPE>(Chunks[i].get()->Chunk.Type) == type) {
                chunks->push_back(Chunks[i]);
            }
        }
    }

    std::shared_ptr<AbstractEMFXChunk> Animation::get_chunk(MOTION_CHUNK_TYPE type) {
        for (uint32_t i = 0; i < Chunks.size(); i++) {
            if (static_cast<MOTION_CHUNK_TYPE>(Chunks[i].get()->Chunk.Type) == type) {
                return Chunks[i];
            }
        }
        return nullptr;
    }

    bool Animation::check(Array<char>* buffer) {
        if (buffer->size() < sizeof(MOTION_HEADER))
            return false;
        MOTION_HEADER header = buffer->cast<MOTION_HEADER>(0);
        // MOTW not supported. Fuck that.
        return buffer->size() >= sizeof(MOTION_HEADER) && header.Magic == FOURCC_MOT && header.IsBigEndian == 0;
    }

    VECTOR4_SINGLE Animation::uncompress_quaternion(VECTOR4_SHORT compressed) { return VECTOR4_SINGLE(); }
} // namespace dragon::lumberyard
