#include "execution_scope.h"

ExecutionScope::ExecutionScope(std::vector<std::string> instructions) {
    this->PC = formatBinary("", 32);
    this->memPosition = formatBinary("", 32);

    for(unsigned int insIndex = 0; insIndex < instructions.size(); insIndex++) {
        std::string insAddress = std::bitset<32>(insIndex * 4).to_string();
        this->setInstruction(insAddress, instructions[insIndex]);
    }

    for(unsigned int regIndex = 0; regIndex < 32; regIndex++) {
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

void ExecutionScope::printRegisters() {
    for(int regIndex = 0; regIndex < 32; regIndex++) {
        std::string regPosition = std::bitset<5>(regIndex).to_string();
        std::cout << "$" << std::setw(2) << std::left << regIndex << " - " << regPosition << ": " << this->getRegisterValue(regPosition) << std::endl;
    }
}

void ExecutionScope::printMemory() {
    std::string memAddress = formatBinary("", 32);
    int endPosition = toDecimal(this->memPosition);

    while(toDecimal(memAddress) < endPosition) {
        std::cout << toHex(memAddress, 8) << ":  " << this->getWordValue(memAddress) << std::endl;
        memAddress = addBinary(memAddress, "100");
    }
}

void ExecutionScope::loadArray(std::vector<std::string> arrayValue) {
    for(unsigned int arrayIndex = 0; arrayIndex < arrayValue.size(); arrayIndex++) {
        std::string value = formatBinary(arrayValue[arrayIndex], 32);

        this->setWordValue(this->memPosition, value);
        this->memPosition = addBinary(this->memPosition, "100");
    }
}

void ExecutionScope::loadArray(std::vector<int> arrayValue) {
    for(unsigned int arrayIndex = 0; arrayIndex < arrayValue.size(); arrayIndex++) {
        std::string value = std::bitset<32>(arrayValue[arrayIndex]).to_string();

        this->setWordValue(this->memPosition, value);
        this->memPosition = addBinary(this->memPosition, "100");
    }
}

void ExecutionScope::setRegisterValue(std::string regPosition, std::string value) {
    regPosition = formatBinary(regPosition, 5);

    this->listRegisters[regPosition] = formatBinary(value, 32);
}

void ExecutionScope::setByteValue(std::string byteAddress, std::string byteValue) {
    byteAddress = formatBinary(byteAddress, 32);

    this->memoryLocations[byteAddress] = formatBinary(byteValue, 8);
}

void ExecutionScope::setByteValue(std::string byteAddress, std::string byteOffset, std::string byteValue) {
    std::string pointedAddress = addBinary(byteAddress, byteOffset);

    this->setByteValue(pointedAddress, byteValue);
}

void ExecutionScope::setWordValue(std::string wordAddress, std::string wordValue) {
    if(toDecimal(wordAddress) % 4 == 0) {
        wordAddress = formatBinary(wordAddress, 32);
        wordValue = formatBinary(wordValue, 32);

        this->setByteValue(wordAddress, wordValue.substr(0, 8));
        this->setByteValue(addBinary(wordAddress, "01"), wordValue.substr(8, 8));
        this->setByteValue(addBinary(wordAddress, "10"), wordValue.substr(16, 8));
        this->setByteValue(addBinary(wordAddress, "11"), wordValue.substr(24, 8));

        this->setMemoryBound(wordAddress);
    }
}

void ExecutionScope::setWordValue(std::string wordAddress, std::string byteOffset, std::string wordValue) {
    std::string pointedAddress = addBinary(wordAddress, byteOffset);

    if(toDecimal(pointedAddress) % 4 == 0) {
        this->setWordValue(pointedAddress, wordValue);
    }
}

void ExecutionScope::setInstruction(std::string insAddress, std::string instruction) {
    insAddress = formatBinary(insAddress, 32);

    Instruction* instructionScope = new Instruction(instruction, this);
    this->listInstructions[insAddress] = instructionScope;

    if(instructionScope->getStatementType() == LABEL) {
        this->setLabelAddress(instructionScope->getName(), insAddress);
    }
}

void ExecutionScope::setLabelAddress(std::string label, std::string insAddress) {
    insAddress = formatBinary(insAddress, 32);

    this->listLabels[label] = insAddress;
}

std::string ExecutionScope::getByteValue(std::string byteAddress) {
    byteAddress = formatBinary(byteAddress, 32);
    auto posLocation = memoryLocations.find(byteAddress);

    if(posLocation != memoryLocations.end()) {
        return posLocation->second;
    } else {
        if(toDecimal(byteAddress) < toDecimal(this->memPosition)) {
            return formatBinary("", 8);
        } else {
            return "";
        }
    }
}

std::string ExecutionScope::getByteValue(std::string byteAddress, std::string byteOffset) {
    std::string pointedAddress = addBinary(byteAddress, byteOffset);

    return this->getByteValue(pointedAddress);
}

std::string ExecutionScope::getWordValue(std::string wordAddress) {
    if(toDecimal(wordAddress) % 4 == 0) {
        std::string firstByte = this->getByteValue(wordAddress);
        std::string secondByte = this->getByteValue(addBinary(wordAddress, "01"));
        std::string thirdByte = this->getByteValue(addBinary(wordAddress, "10"));
        std::string fourthByte = this->getByteValue(addBinary(wordAddress, "11"));

        return firstByte + secondByte + thirdByte + fourthByte;
    } else {
        return "";
    }
}

std::string ExecutionScope::getWordValue(std::string wordAddress, std::string byteOffset) {
    std::string pointedAddress = addBinary(wordAddress, byteOffset);

    if(toDecimal(pointedAddress) % 4 == 0) {
        return this->getWordValue(pointedAddress);
    } else {
        return "";
    }
}

std::string ExecutionScope::getRegisterValue(std::string regPosition) {
    regPosition = formatBinary(regPosition, 5);
    auto posRegister = listRegisters.find(regPosition);

    if(posRegister != listRegisters.end()) {
        return posRegister->second;
    } else {
        return "";
    }
}

Instruction* ExecutionScope::getInstruction(std::string insAddress) {
    insAddress = formatBinary(insAddress, 32);
    auto posInstruction = listInstructions.find(insAddress);

    if(posInstruction != listInstructions.end()) {
        return posInstruction->second;
    } else {
        return nullptr;
    }
}

std::string ExecutionScope::getLabelAddress(std::string label) {
    auto posLabel = listLabels.find(label);

    if(posLabel != listLabels.end()) {
        return posLabel->second;
    } else {
        return "";
    }
}

std::string ExecutionScope::getPC() {
    return this->PC;
}

void ExecutionScope::incPC() {
    std::string newPC = addBinary(this->PC, "100");

    this->PC = formatBinary(newPC, 32);
}

void ExecutionScope::setPC(std::string newPC) {
    this->PC = formatBinary(newPC, 32);
}

void ExecutionScope::setMemoryBound(std::string memAddress) {
    if(toDecimal(this->memPosition) <= toDecimal(memAddress)) {
        int newBoundAddress = ((int) (toDecimal(memAddress) / 4)) * 4 + 4;

        this->memPosition = std::bitset<32>(newBoundAddress).to_string();
    }
}

void ExecutionScope::jumpLabel(std::string label) {
    auto posLabel = listLabels.find(label);

    if(posLabel != listLabels.end()) {
        this->PC = posLabel->second;
    }
}

bool ExecutionScope::isFinished() {
    auto posInstruction = listInstructions.find(this->PC);

    return posInstruction == listInstructions.end();
}
