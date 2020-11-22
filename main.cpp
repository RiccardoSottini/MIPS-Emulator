#include <iostream>
#include <vector>

#include "src/def.h"

int main() {
    std::vector<std::string> instructions = {
        "add $s0, $t0, $zero",
        "addi $s1, $t1, 128",
        "sub $s1, $s1, $t1"
    };

    for(int insIndex = 0; insIndex < instructions.size(); insIndex++) {
        std::string insText = instructions[insIndex];
        Instruction *instruction = new Instruction(insText);

        std::cout << "Instruction: " << insText << std::endl;
        instruction->printMemory();
        std::cout << "Binary = " << instruction->getBinary() << std::endl << std::endl;

        delete instruction;
    }
}
