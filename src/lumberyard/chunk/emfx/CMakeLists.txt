project(fmt_lumberyard)

target_sources(fmt_lumberyard PRIVATE
               EMFXChunks.h AbstractEMFXChunk.h SharedStructures.h
               ActorInfo.cpp ActorInfo.h
               ActorNode.cpp ActorNode.h
               ActorNodes.cpp ActorNodes.h)
