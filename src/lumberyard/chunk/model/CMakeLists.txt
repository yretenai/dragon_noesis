project(dragon_lumberyard)

target_sources(dragon_lumberyard PRIVATE
               ModelChunks.h AbstractModelChunk.h SharedStructures.h
               ExportFlags.cpp ExportFlags.h
               MaterialName.cpp MaterialName.h
               Mesh.cpp Mesh.h
               Node.cpp Node.h
               Submesh.cpp Submesh.h
               DataStream.cpp DataStream.h)
