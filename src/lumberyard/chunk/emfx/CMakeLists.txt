project(fmt_lumberyard)

target_sources(fmt_lumberyard PRIVATE
               EMFXChunks.h AbstractEMFXChunk.h SharedStructures.h
               ActorInfo.cpp ActorInfo.h
               ActorMaterial.cpp ActorMaterial.h
               ActorMaterialInfo.cpp ActorMaterialInfo.h
               ActorMaterialLayer.cpp ActorMaterialLayer.h
               ActorNode.cpp ActorNode.h
               ActorNodeMotionSources.cpp ActorNodeMotionSources.h
               ActorNodes.cpp ActorNodes.h)
