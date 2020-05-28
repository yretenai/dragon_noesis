//
// Created by yretenai on 5/29/2020.
//

#include "../../dragon.h"
#include "../../lumberyard/Lumberyard.h"

using namespace std;

int main(int argc, char** argv) {
    open_dragon_log_stdout();
    if(argc < 2) {
        write_dragon_log("Usage: test.exe path\n");
        super_assert_dragon_log<std::invalid_argument>(argc > 2, "Invalid program args",
                                                       "Assertion failed -> argc > 2\n");
    }
    char* filepath = argv[1];
    std::vector<char> bytes = read_file(filepath);
    LumberyardModel model(bytes);
    return 0;
}
