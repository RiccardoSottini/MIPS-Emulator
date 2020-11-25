#include <iostream>
#include <vector>

#include "src/def.h"

int main() {
    /* Example */
    std::vector<std::string> instructions = {
        "addi $t1, $t1, 200",
        "addi $t2, $t2, 12",
        "sb $t1, 4($t2)",

        "add $s0, $zero, $zero",
        "addi $s1, $zero, 1",
        "addi $t0, $zero, 10",
        "Loop:",
        "beq $s0, $t0, Done",
        "sll $s1, $s1, 1",
        "addi $s0, $s0, 1",
        "j Loop",
        "Done:",
        "addi $s2, $s2, 2",
        "addi $s2, $s2, 2",
    };

    ExecutionScope *executionScope = new ExecutionScope(instructions);
    executionScope->executeScope();
    executionScope->printMemory();
    executionScope->printRegisters();
}
