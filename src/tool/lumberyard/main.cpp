//
// Created by yretenai on 5/29/2020.
//

#include "../../dragon.h"
#include "../../lumberyard/Lumberyard.h"
#include "../../lumberyard/chunks/model/MaterialName.h"

using namespace std;

int main(int argc, char** argv) {
    open_dragon_log_stdout();
    if(argc < 2) {
        write_dragon_log("Usage: test.exe path\n");
        super_assert_dragon_log(argc > 2, "Invalid program args", "Assertion failed -> argc > 2\n");
    }
    char* filepath = argv[1];
    std::vector<char> bytes = read_file(filepath);
    Model model(bytes);
    MaterialName name = *reinterpret_cast<MaterialName*>(model.Chunks[2]);
    close_dragon_log();
    return 0;
}
