//
// Created by yretenai on 5/29/2020.
//

#include "../../dragon.h"
#include "../../lumberyard/Lumberyard.h"
#include "../../lumberyard/chunks/model/MaterialName.h"

using namespace std;
using namespace dragon;

int main(int argc, char** argv) {
    open_dragon_log_stdout();
    if(argc < 2) {
        write_dragon_log("Usage: %s path\n", argv[0]);
        return 1;
    }
    char* filepath = argv[1];
    std::vector<char> bytes = read_file(filepath);
    lumberyard::Model model(bytes);
    close_dragon_log();
    return 0;
}
