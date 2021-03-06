#include "../includes/execution_scope.h"

/**
 * ExecutionScope Constructor - Initializes the data structures used by the Execution Scope
 *
 * @param instructions List of Instructions to load into the Execution Scope
 */
ExecutionScope::ExecutionScope(std::vector<std::string> instructions) {
    this->setPC(startPC);
    std::vector<Instruction*> instructionsParsed;

    for(unsigned int insIndex = 0; insIndex < instructions.size(); insIndex++) {
        Instruction* instructionScope = new Instruction(instructions[insIndex], INSTRUCTION_VALUE, this);
        instructionsParsed.push_back(instructionScope);

        if(instructionScope->getStatementType() == LABEL) {
            std::string insAddress = toBinary(toDecimal(this->PC) + (insIndex * 4));
            this->setLabelAddress(instructionScope->getName(), insAddress);
        }
    }

    for(unsigned int insIndex = 0; insIndex < instructionsParsed.size(); insIndex++) {
        std::string insAddress = toBinary(toDecimal(this->PC) + (insIndex * 4));

        instructionsParsed[insIndex]->setAddress(insAddress);
        std::string insValue = instructionsParsed[insIndex]->calculateBinary();

        this->setWordValue(insAddress, insValue);

        delete instructionsParsed[insIndex];
    }

    for(unsigned int regIndex = 0; regIndex < 32; regIndex++) {
        std::string regPosition = std::bitset<5>(regIndex).to_string();
        this->setRegisterValue(regPosition, "");
    }

    this->setRegisterValue("11100", startGP);
    this->setRegisterValue("11101", startSP);
}

/**
 * Emulate the MIPS Execution Scope
 */
void ExecutionScope::executeScope() {
    this->setPC(startPC);

    while(!this->isFinished()) {
        std::string insValue = this->getWordValue(this->PC);

        Instruction* instructionScope = new Instruction(insValue, BINARY_VALUE, this);
        instructionScope->executeInstruction();

        delete instructionScope;
    }
}

/**
 * Print the Instructions loaded into the Execution Scope
 *
 * @param inputType Format used to print out the Instruction
 */
void ExecutionScope::printInstructions(enum InputType inputType) {
    std::string insAddress = startPC;
    std::string insValue = "";

    while((insValue = this->getWordValue(insAddress)) != "") {
        if(inputType == INSTRUCTION_VALUE) {
            Instruction* instructionScope = new Instruction(insValue, BINARY_VALUE, this);
            instructionScope->setAddress(insAddress);

            std::cout << toHex(insAddress, 8) << ":    " << instructionScope->calculateInstruction() << std::endl;
        } else if(inputType == BINARY_VALUE) {
            std::cout << toHex(insAddress, 8) << ":    " << insValue << std::endl;
        }

        insAddress = addBinary(insAddress, "100");
    }
}

/**
 * Print the Registers values of the Execution Scope
 */
void ExecutionScope::printRegisters() {
    for(int regIndex = 0; regIndex < 32; regIndex++) {
        std::string regPosition = std::bitset<5>(regIndex).to_string();
        std::cout << std::setw(5) << std::left << registerPointers[regIndex] << " (" << regPosition << "): " << this->getRegisterValue(regPosition) << std::endl;
    }
}

/**
 * Print the Dynamic Memory Locations values loaded into the Execution Scope
 */
void ExecutionScope::printMemory() {
    std::string memAddress = formatBinary("10000000000001000000000000000", 32);
    std::string gpValue = this->getRegisterValue("11100");

    while(toDecimal(memAddress) < toDecimal(gpValue)) {
        std::cout << toHex(memAddress, 8) << ":    " << this->getWordValue(memAddress) << std::endl;
        memAddress = addBinary(memAddress, "100");
    }
}

/**
 * Load Binary Values into the Dynamic Memory
 *
 * @param binaryValue Binary Values to be loaded into the Dynamic Memory
 */
void ExecutionScope::loadValue(std::string binaryValue) {
    std::string gpValue = this->getRegisterValue("11100");
    this->setWordValue(gpValue, binaryValue);

    this->setRegisterValue("11100", addBinary(gpValue, "100"));
}

/**
 * Load an Integer Value that is then converted into its Binary Format into the Dynamic Memory
 *
 * @param decimalValue Integer that is converted into Binary Values and loaded into the Dynamic Memory
 */
void ExecutionScope::loadValue(const int decimalValue) {
    std::string gpValue = this->getRegisterValue("11100");
    this->setWordValue(gpValue, toBinary(decimalValue));

    this->setRegisterValue("11100", addBinary(gpValue, "100"));
}

/**
 * Load an array of Binary Values into the Dynamic Memory
 *
 * @param arrayValue Array of Binary Values
 */
void ExecutionScope::loadArray(std::vector<std::string> arrayValue) {
    std::string gpValue = this->getRegisterValue("11100");

    for(unsigned int arrayIndex = 0; arrayIndex < arrayValue.size(); arrayIndex++) {
        std::string value = formatBinary(arrayValue[arrayIndex], 32);
        this->setWordValue(gpValue, value);

        gpValue = addBinary(gpValue, "100");
    }

    this->setRegisterValue("11100", gpValue);
}

/**
 * Load an array of Integers converted into Binary Values into the Dynamic Memory
 *
 * @param arrayValue Array of Integer then converted into Binary Values
 */
void ExecutionScope::loadArray(std::vector<int> arrayValue) {
    std::string gpValue = this->getRegisterValue("11100");

    for(unsigned int arrayIndex = 0; arrayIndex < arrayValue.size(); arrayIndex++) {
        std::string value = std::bitset<32>(arrayValue[arrayIndex]).to_string();
        this->setWordValue(gpValue, value);

        gpValue = addBinary(gpValue, "100");
    }

    this->setRegisterValue("11100", gpValue);
}

/**
 * Set the Value to the specified Register
 *
 * @param regPosition Register selected to set the Value
 * @param value Value set to the specified Register
 */
void ExecutionScope::setRegisterValue(std::string regPosition, std::string value) {
    regPosition = formatBinary(regPosition, 5);

    this->listRegisters[regPosition] = formatBinary(value, 32);
}

/**
 * Set the Value to a certain Byte Memory Location specified by the Address
 *
 * @param byteAddress Address used to select a certain Memory Location
 * @param byteValue Byte Value that is set at the Address specified
 */
void ExecutionScope::setByteValue(std::string byteAddress, std::string byteValue) {
    byteAddress = formatBinary(byteAddress, 32);

    this->memoryLocations[byteAddress] = formatBinary(byteValue, 8);
}

/**
 * Set the Value to a certain Byte Memory Location calculated using the Address and the Offset
 *
 * @param byteAddress, byteOffset Parameters used to calculate the Address used to set the Byte
 * @param byteValue Byte Value that is set at the Address calculated
 */
void ExecutionScope::setByteValue(std::string byteAddress, std::string byteOffset, std::string byteValue) {
    std::string pointedAddress = addBinary(byteAddress, byteOffset);

    this->setByteValue(pointedAddress, byteValue);
}

/**
 * Set the Value to a certain Word Memory Location specified by the Address
 *
 * @param wordAddress Address used to select a certain Memory Location
 * @param wordValue Word Value that is set at the Address specified
 */
void ExecutionScope::setWordValue(std::string wordAddress, std::string wordValue) {
    if(toDecimal(wordAddress) % 4 == 0) {
        wordAddress = formatBinary(wordAddress, 32);
        wordValue = formatBinary(wordValue, 32);

        this->setByteValue(wordAddress, wordValue.substr(0, 8));
        this->setByteValue(addBinary(wordAddress, "01"), wordValue.substr(8, 8));
        this->setByteValue(addBinary(wordAddress, "10"), wordValue.substr(16, 8));
        this->setByteValue(addBinary(wordAddress, "11"), wordValue.substr(24, 8));
    }
}

/**
 * Set the Value to a certain Word Memory Location calculated using the Address and the Offset
 *
 * @param wordAddress, byteOffset Parameters used to calculate the Address used to set the Word
 * @param wordValue Word Value that is set at the Address calculated
 */
void ExecutionScope::setWordValue(std::string wordAddress, std::string byteOffset, std::string wordValue) {
    std::string pointedAddress = addBinary(wordAddress, byteOffset);

    if(toDecimal(pointedAddress) % 4 == 0) {
        this->setWordValue(pointedAddress, wordValue);
    }
}

/**
 * Match the Label Name with its Location Address
 *
 * @param label Label Name
 * @param insAddress Address to be matched with the Label Name
 */
void ExecutionScope::setLabelAddress(std::string label, std::string insAddress) {
    insAddress = formatBinary(insAddress, 32);

    this->listLabels[label] = insAddress;
    this->pointerLabels[insAddress] = label;
}

/**
 * Set the new Program Counter
 *
 * @param newPC The new Program Counter
 */
void ExecutionScope::setPC(std::string newPC) {
    this->PC = formatBinary(newPC, 32);
}

/**
 * Set the new Program Counter in base of the Addressing Type
 *
 * @param addressingValue The value used to calculate the new Program Counter
 * @param addressingType The Type of Addressing used
 */
void ExecutionScope::setPC(std::string addressingValue, enum AddressingType addressingType) {
    if(addressingType == PC_RELATIVE_ADDRESSING) {
        /* PC = PC + 4 + BranchAddr */
        std::string newPC = addBinary(this->PC, "100");
        this->PC = addBinary(newPC, BranchAddr(addressingValue));
    } else if(addressingType == PSEUDO_DIRECT_ADDRESSING) {
        /* PC = JumpAddr */
        this->PC = JumpAddr(this->PC, addressingValue);
    } else if(addressingType == REGISTER_ADDRESSING) {
        this->PC = formatBinary(addressingValue, 32);
    }
}

/**
 * Get the Byte allocated in a certain Memory Location
 *
 * @param byteAddress Address used to retrieve the Byte
 * @return the Byte allocated at the Address gave as parameter
 */
std::string ExecutionScope::getByteValue(std::string byteAddress) {
    byteAddress = formatBinary(byteAddress, 32);

    if(this->isAllocated(byteAddress)) {
        auto posLocation = memoryLocations.find(byteAddress);

        if(posLocation != memoryLocations.end()) {
            return formatBinary(posLocation->second, 8);
        }
    } else {
        std::cout << "Byte not allocated!" << std::endl;
    }

    return "";
}

/**
 * Get the Byte allocated in a certain Memory Location calculated using the Address and the Offset
 *
 * @param byteAddress, byteOffset Parameters used to calculate the Address used to retrieve the Byte
 * @return the Byte allocated at the Address calculated
 */
std::string ExecutionScope::getByteValue(std::string byteAddress, std::string byteOffset) {
    std::string pointedAddress = addBinary(byteAddress, byteOffset);

    return this->getByteValue(pointedAddress);
}

/**
 * Get the Word allocated in a certain Memory Location
 *
 * @param wordAddress Address used to retrieve the Word
 * @return the Word allocated at the Address gave as parameter
 */
std::string ExecutionScope::getWordValue(std::string wordAddress) {
    if(toDecimal(wordAddress) % 4 == 0) {
        if(this->isAllocated(wordAddress)) {
            std::string firstByte = this->getByteValue(wordAddress);
            std::string secondByte = this->getByteValue(addBinary(wordAddress, "01"));
            std::string thirdByte = this->getByteValue(addBinary(wordAddress, "10"));
            std::string fourthByte = this->getByteValue(addBinary(wordAddress, "11"));

            return firstByte + secondByte + thirdByte + fourthByte;
        } else {
            std::cout << "Word not allocated!" << std::endl;
        }
    }

    return "";
}

/**
 * Get the Word allocated in a certain Memory Location calculated using the Address and the Offset
 *
 * @param wordAddress, byteOffset Parameters used to calculate the Address used to retrieve the Word
 * @return the Word allocated at the Address calculated
 */
std::string ExecutionScope::getWordValue(std::string wordAddress, std::string byteOffset) {
    std::string pointedAddress = addBinary(wordAddress, byteOffset);

    if(toDecimal(pointedAddress) % 4 == 0) {
        return this->getWordValue(pointedAddress);
    } else {
        return "";
    }
}

/**
 * Get the Value assigned to a Register
 *
 * @param regPosition Register selected to get its Value
 * @return The Value assigned to the specified Register
 */
std::string ExecutionScope::getRegisterValue(std::string regPosition) {
    regPosition = formatBinary(regPosition, 5);
    auto posRegister = listRegisters.find(regPosition);

    if(posRegister != listRegisters.end()) {
        return posRegister->second;
    } else {
        return "";
    }
}

/**
 * Get the Address matched with the specified Label Name
 *
 * @param label Label Name
 * @return The Address matched with the specified Label Name
 */
std::string ExecutionScope::getLabelAddress(std::string label) {
    auto posLabel = listLabels.find(label);

    if(posLabel != listLabels.end()) {
        return posLabel->second;
    } else {
        return "";
    }
}

/**
 * Get the Name of the Label
 *
 * @param address Address of the Label Location
 * @return Name of the Label found at the Address
 */
std::string ExecutionScope::getLabelName(std::string address) {
    auto posLabel = pointerLabels.find(formatBinary(address, 32));

    if(posLabel != pointerLabels.end()) {
        return posLabel->second;
    } else {
        return "";
    }
}

/**
 * Get the Program Counter's Value
 *
 * @return current Program Counter's Value
 */
std::string ExecutionScope::getPC() {
    return this->PC;
}

/**
 * Increment the Program Counter to point to the next Instruction
 */
void ExecutionScope::incPC() {
    std::string newPC = addBinary(this->PC, "100");

    this->PC = formatBinary(newPC, 32);
}

/**
 * Retrieves if there are Instructions left to be executed
 *
 * @return True if there are Instructions left to be executed / False if all the Instructions were already executed
 */
bool ExecutionScope::isFinished() {
    if(toDecimal(this->PC) < toDecimal("10000000000000000000000000000")) {
        auto posInstruction = memoryLocations.find(this->PC);

        return posInstruction == memoryLocations.end();
    }

    return true;
}

/**
 * Retrieves whether the Memory at the Address is allocated
 *
 * @param address Address Value where to check
 * @return True if the Memory Location is allocated, otherwise return False
 */
bool ExecutionScope::isAllocated(std::string address) {
    int addressDecimal = toDecimal(address);

    //between SP and startSP | between startPC and GP
    return ((addressDecimal >= toDecimal(getRegisterValue("11101")) &&
             addressDecimal <  toDecimal(startSP)) ||
            (addressDecimal >= toDecimal(startPC) &&
             addressDecimal <= toDecimal(getRegisterValue("11100"))));
}
