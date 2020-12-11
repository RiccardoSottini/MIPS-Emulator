#include "execution_interface.h"

/**
 * ExecutionInterface Constructor - initializes the Class
 */
ExecutionInterface::ExecutionInterface() {
    this->executionScope = nullptr;
}

/**
 * Execute the Command Line Interface
 */
void ExecutionInterface::executeInterface() {
    std::string insAddress = startPC;
    std::string insValue = "";

    std::cout << "Type the Instructions you want to Execute (Enter q to Quit):" << std::endl << std::endl;

    do {
        std::cout << toHex(insAddress, 8) << ": ";
        std::getline(std::cin, insValue);

        if(insValue != "q" && insValue != "Q") {
            insAddress = addBinary(insAddress, "100");
            instructions.push_back(insValue);
        }
    } while(insValue != "q" && insValue != "Q");

    this->clearScreen();

    executionScope = new ExecutionScope(this->instructions);
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

/**
 * Clear the Terminal Screen
 */
void ExecutionInterface::clearScreen() {
    #ifdef __linux__
         std::system ("clear");
    #elif _WIN32
         std::system ("cls");
    #endif
}
