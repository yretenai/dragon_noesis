project(fmt_lumberyard)

target_sources(fmt_lumberyard PRIVATE
               ModelChunks.h AbstractModelChunk.h SharedStructures.h
               ExportFlags.cpp ExportFlags.h
               MaterialName.cpp MaterialName.h
               Submesh.cpp Submesh.h
               DataStream.cpp DataStream.h)
