#include "execution_scope.h"

ExecutionScope::ExecutionScope(std::vector<std::string> instructions) {
    this->PC = "00000000";

    for(int insIndex = 0; insIndex < instructions.size(); insIndex++) {
        std::string address = std::bitset<8>(insIndex * 4).to_string();
        this->setInstruction(address, instructions[insIndex]);
    }

    for(int regIndex = 0; regIndex < 32; regIndex++) {
        std::string regPosition = std::bitset<5>(regIndex).to_string();
        this->setRegisterValue(regPosition, "");
    }
}

void ExecutionScope::executeScope() {
    while(!this->isFinished()) {
        Instruction* instructionScope = this->getInstruction(this->PC);
        instructionScope->executeInstruction(this);


    }
}

void ExecutionScope::setRegisterValue(std::string regPosition, std::string value) {
    if(value.length() < 32) {
        this->memoryRegisters[regPosition] = std::string(32 - value.length(), '0') + value;
    } else {
        this->memoryRegisters[regPosition] = value;
    }
}

void ExecutionScope::setInstruction(std::string address, std::string instruction) {
    Instruction* instructionScope = new Instruction(instruction);
    this->memoryInstructions[address] = instructionScope;

    if(instructionScope->getStatementType() == LABEL) {
        memoryLabels[instructionScope->getName()] = address;
    }
}

std::string ExecutionScope::getRegisterValue(std::string regPosition) {
    auto posRegister = memoryRegisters.find(regPosition);

    if(posRegister != memoryRegisters.end()) {
        return posRegister->second;
    } else {
        return "";
    }
}

Instruction* ExecutionScope::getInstruction(std::string address) {
    auto posInstruction = memoryInstructions.find(address);

    if(posInstruction != memoryInstructions.end()) {
        return posInstruction->second;
    } else {
        return nullptr;
    }
}

void ExecutionScope::incPC() {
    int decimalPC = std::stoi(this->PC, nullptr, 2);
    int newPC = decimalPC + 4;

    this->PC = std::bitset<8>(newPC).to_string();
}

void ExecutionScope::setPC(std::string newPC) {
    this->PC = newPC;
}

void ExecutionScope::jumpLabel(std::string label) {
    auto posLabel = memoryLabels.find(label);

    if(posLabel != memoryLabels.end()) {
        this->PC = posLabel->second;
    }
}

bool ExecutionScope::isFinished() {
    auto posInstruction = memoryInstructions.find(this->PC);

    return posInstruction == memoryInstructions.end();
}

void ExecutionScope::printRegisters() {
    for(int regIndex = 0; regIndex < 32; regIndex++) {
        std::string regPosition = std::bitset<5>(regIndex).to_string();
        std::cout << regPosition << ": " << this->getRegisterValue(regPosition) << std::endl;
    }
}
