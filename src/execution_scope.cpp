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
        instructionScope->calculateMemory();
        instructionScope->executeInstruction();

        std::cout << "Instruction " << instructionScope->getName() << ": " << std::endl;
        instructionScope->printMemory();
        std::cout << instructionScope->getBinary() << std::endl << std::endl;
    }
}

void ExecutionScope::setRegisterValue(std::string regPosition, std::string value) {
    this->memoryRegisters[regPosition] = formatBinary(value, 32);
}

void ExecutionScope::setInstruction(std::string address, std::string instruction) {
    Instruction* instructionScope = new Instruction(instruction, this);
    this->memoryInstructions[address] = instructionScope;

    if(instructionScope->getStatementType() == LABEL) {
        this->setLabelAddress(instructionScope->getName(), address);
    }
}

void ExecutionScope::setLabelAddress(std::string label, std::string address) {
    this->memoryLabels[label] = address;
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

std::string ExecutionScope::getLabelAddress(std::string label) {
    auto posLabel = memoryLabels.find(label);

    if(posLabel != memoryLabels.end()) {
        return posLabel->second;
    } else {
        return "";
    }
}

void ExecutionScope::incPC() {
    int decimalPC = std::stoi(this->PC, nullptr, 2);
    int newPC = decimalPC + 4;

    this->PC = std::bitset<8>(newPC).to_string();
}

void ExecutionScope::setPC(std::string newPC) {
    this->PC = formatBinary(newPC, 8);
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
