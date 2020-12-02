#include <iostream>
#include <vector>

#include "src/def.h"

int main() {
    /* Instruction Parse Example */
    std::string binary = "00100001001010010001001110001000";
    Instruction* secondInstruction = new Instruction(binary, BINARY_VALUE);
    secondInstruction->calculateInstruction();
    std::cout << "INSTRUCTION BINARY PARSER: " << secondInstruction->getInstruction() << std::endl << std::endl;

    /* Execution Scope Example */
    std::vector<std::string> instructions = {
        "addi $t1, $t1, 5000",
        "addi $t2, $t2, 0xC",
        "sb $t1, 4($t2)",
        "sb $t1, 8($t2)",

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
    executionScope->loadArray({"", "", ""});
    executionScope->loadValue(15);
    executionScope->executeScope();

    std::cout << "Instructions:" << std::endl;
    executionScope->printInstructions(INSTRUCTION_VALUE);

    std::cout << std::endl << "Instructions (Binary):" << std::endl;
    executionScope->printInstructions(BINARY_VALUE);

    std::cout << std::endl << "Memory (Binary):" << std::endl;
    executionScope->printMemory();

    std::cout << std::endl << "Registers (Binary):" << std::endl;
    executionScope->printRegisters();
}
