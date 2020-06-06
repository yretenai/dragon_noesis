//
// Created by yretenai on 6/5/2020.
//

#include "Node.h"

namespace dragon::lumberyard::chunk::model {
    Node::Node(Array<char>* buffer, CRCH_CHUNK_HEADER chunk_header) {
        Chunk = chunk_header;
        super_assert_dragon_log(Chunk.Version == 0x824 ||
                                    Chunk.Version == 0x823,
                                "version == 0x824 || version == 0x823");
        Name = std::string(buffer->slice(0, 64).data());
        Header = buffer->cast<NODE_HEADER>(64);
        if (Header.PropertyStringLength > 0) {
            Property = std::string(buffer
                                       ->slice(64 + sizeof(NODE_HEADER),
                                               Header.PropertyStringLength)
                                       .data());
        }
    }
} // namespace dragon::lumberyard::chunk::model