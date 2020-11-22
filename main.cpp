#include <iostream>
#include <vector>

#include "src/def.h"

int main() {
    /* Example */
    std::vector<std::string> instructions = {
        "add $s0, $zero, $zero",
        "addi $s1, $zero, 1",
        "addi $t0, $zero, 10",
        "Loop:",
        "beq $s0, $t0, Done",
        "add $s1, $s1, $s1",
        "addi $s0, $s0, 1",
        "j Loop",
        "Done:"
    };

    ExecutionScope *executionScope = new ExecutionScope(instructions);
    executionScope->executeScope();
    executionScope->printRegisters();
}
