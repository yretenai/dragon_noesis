project(dragon_lumberyard)

target_sources(dragon_lumberyard PRIVATE
               EMFXChunks.h AbstractEMFXChunk.h SharedStructures.h
               ActorInfo.cpp ActorInfo.h
               ActorMaterial.cpp ActorMaterial.h
               ActorMaterialAttribute.cpp ActorMaterialAttribute.h
               ActorMaterialAttributeSet.cpp ActorMaterialAttributeSet.h
               ActorMaterialAttributeValue.cpp ActorMaterialAttributeValue.h
               ActorMaterialInfo.cpp ActorMaterialInfo.h
               ActorMaterialLayer.cpp ActorMaterialLayer.h
               ActorMesh.cpp ActorMesh.h
               ActorNode.cpp ActorNode.h
               ActorNodeMotionSources.cpp ActorNodeMotionSources.h
               ActorNodes.cpp ActorNodes.h
               ActorSkinningInfo.cpp ActorSkinningInfo.h
               ActorSubmesh.cpp ActorSubmesh.h
               ActorVertexBuffer.cpp ActorVertexBuffer.h
               MotionInfo.cpp MotionInfo.h
               MotionSubMotion.cpp MotionSubMotion.h
               MotionSubMotions.cpp MotionSubMotions.h)
