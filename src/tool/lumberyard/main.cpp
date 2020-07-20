//
// Created by yretenai on 5/29/2020.
//

#include "standard_dragon/dragon.h"
#include "../../lumberyard/Lumberyard.h"

std::ofstream* LogStream;

using namespace dragon;

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " path" << std::endl;
        return 1;
    }
    char* filepath = argv[1];
    Array<char> buffer = read_file(filepath);
    lumberyard::Animation anim = lumberyard::Animation(&buffer);
    return 0;
}
