#include "def.h"

/**
 * Instruction Constructor - Initializes the Instruction Object by parsing the Value passed as parameter
 *
 * @param value Value passed as Parameter that can be the Instruction or the Binary Value (specified by the Input Type)
 * @param inputType Input Type that says what does Value contain (an Instruction or the Binary Value)
 */
Instruction::Instruction(std::string value, InputType inputType) {
    if(inputType == INSTRUCTION_VALUE) {
        parseInstruction(value);
    } else if(inputType == BINARY_VALUE) {
        parseBinary(value);
    }
}

/**
 * Instruction Constructor - Initializes the Instruction Object by parsing the Value passed as parameter and save the pointer to the Execution Scope
 *
 * @param value Value passed as Parameter that can be the Instruction or the Binary Value (specified by the Input Type)
 * @param inputType Input Type that says what does Value contain (an Instruction or the Binary Value)
 * @param executionScope Execution Scope pointer that holds the Execution informations of the MIPS Emulator
 */
Instruction::Instruction(std::string value, InputType inputType, ExecutionScope* executionScope) {
    if(inputType == INSTRUCTION_VALUE) {
        parseInstruction(value);
    } else if(inputType == BINARY_VALUE) {
        parseBinary(value);
    }

    this->executionScope = executionScope;
}

/**
 * Parse the Instruction and save its Parameters
 *
 * @param instruction Instruction to be parsed
 */
void Instruction::parseInstruction(std::string instruction) {
    std::string currentParameter = "";
    int parameterIndex = 0;

    for(int index = 0; index < instruction.size(); index++) {
        char currentChar = instruction[index];
        bool currentAdd = false;

        if(currentChar != ' ' && currentChar != ',' && currentChar != '(' && currentChar != ')') {
            currentParameter += currentChar;
            currentAdd = true;
        }

        if((!currentAdd || index == instruction.size() - 1) && currentParameter.size()) {
            if(parameterIndex > 0) {
                this->parameters.push_back(currentParameter);
            } else {
                this->name = currentParameter;
            }

            currentParameter = "";
            parameterIndex++;
        }
    }

    if(this->parameters.size()) {
        this->statementType = INSTRUCTION;
    } else {
        this->statementType = LABEL;
        this->name.pop_back();
    }

    auto posInstruction = instructionFormats.find(this->name);
    if(posInstruction != instructionFormats.end()) {
        memoryStructure = &posInstruction->second;
    }
}

/**
 * Parse the Binary Value of an Instruction and save its Fields
 *
 * @param binary Binary Value to be parsed
 */
void Instruction::parseBinary(std::string binary) {
    std::string nameInstruction = "";
    std::string opcode = binary.substr(0, 6);
    std::string funct = "";

    if(opcode != std::string(6, 'x')) {
        this->statementType = INSTRUCTION;

        if(toDecimal(opcode) == 0) {
            funct = binary.substr(26, 6);
        }

        auto pos = instructionPointers.find({opcode, funct});
        if(pos != instructionPointers.end()) {
            nameInstruction = pos->second;
        }

        auto posInstruction = instructionFormats.find(nameInstruction);
        if(posInstruction != instructionFormats.end()) {
            memoryStructure = &posInstruction->second;

            setField("opcode", opcode);

            if(memoryStructure->getFormat() == R_FORMAT) {
                setField("rs", binary.substr(6, 5));
                setField("rt", binary.substr(11, 5));
                setField("rd", binary.substr(16, 5));
                setField("shamt", binary.substr(21, 5));
                setField("funct", funct);
            } else if(memoryStructure->getFormat() == I_FORMAT) {
                setField("rs", binary.substr(6, 5));
                setField("rt", binary.substr(11, 5));
                setField("imm", binary.substr(16, 16));
            } else if(memoryStructure->getFormat() == J_FORMAT) {
                setField("addr", binary.substr(6, 26));
            }
        }
    } else {
        this->statementType = LABEL;
    }
}

/**
 * Calculate the Binary Value starting from the Instruction Parameters
 *
 * @return Binary Value of the Instruction
 */
std::string Instruction::calculateBinary() {
    if(memoryStructure != nullptr) {
        std::vector<std::string> parametersOrder = memoryStructure->getParametersOrder();

        setField("opcode", memoryStructure->getOpcode());

        if(memoryStructure->getFormat() == R_FORMAT) {
            setField("rs", "");
            setField("rt", "");
            setField("rd", "");
            setField("shamt", "");
            setField("funct", memoryStructure->getFunct());
        } else if(memoryStructure->getFormat() == I_FORMAT) {
            setField("rs", "");
            setField("rt", "");
            setField("imm", "");
        } else if(memoryStructure->getFormat() == J_FORMAT) {
            setField("addr", "");
        }

        for(int indexP = 0; indexP < parametersOrder.size(); indexP++) {
            std::string parameterName = parametersOrder[indexP];
            std::string parameterValue = getParameter(indexP);

            setField(parameterName, calculateField(parameterName, parameterValue));
        }
    } else {
        if(getStatementType() == INSTRUCTION) {
            std::cout << "ERROR: Instruction " + getName() + " does not exist!";
        }
    }

    return this->getBinary();
}

/**
 * Calculate the Instruction starting from the Binary Value Fields
 *
 * @return Instruction's Value (String that contains the opcode and the parameters)
 */
std::string Instruction::calculateInstruction() {
    if(getStatementType() == INSTRUCTION) {
        if(memoryStructure != nullptr) {
            std::vector<std::string> parametersOrder = memoryStructure->getParametersOrder();

            this->name = calculateParameter("opcode", getField("opcode"));

            for(int indexP = 0; indexP < parametersOrder.size(); indexP++) {
                std::string fieldName = parametersOrder[indexP];
                std::string fieldValue = getField(fieldName);

                setParameter(indexP, calculateParameter(fieldName, fieldValue));
            }
        } else {
            if(getStatementType() == INSTRUCTION) {
                std::cout << "ERROR: Instruction " + getName() + " does not exist!";
            }
        }
    }

    return this->getInstruction();
}

/**
 * Calculate a Field's Value starting from the Parameter's Name and the Parameter's Value
 *
 * @param parameterName Parameter's Name used to calculate the size of the Binary Value to be returned
 * @param parameterValue Parameter's Value to be converted into its Binary Value
 * @return Field's Binary Value calculated in base of the Parameter's Name and the Parameter's Value
 */
std::string Instruction::calculateField(std::string parameterName, std::string parameterValue) {
    int fieldSize = 0;
    std::string binaryValue = "";

    auto posSize = fieldSizes.find(parameterName);
    if(posSize != fieldSizes.end()) {
        fieldSize = posSize->second;
    }

    if(parameterValue[0] == '$') {
        auto pos = registerFormats.find(parameterValue);
        if(pos != registerFormats.end()) {
            std::string value = pos->second;
            binaryValue = std::string(fieldSize - value.length(), '0') + value;
        } else {
            std::cout << "ERROR: Register not valid!";
        }
    } else {
        std::string value = "";

        if(isNumber(parameterValue) || isHex(parameterValue)) {
            if(isHex(parameterValue)) {
                parameterValue = std::to_string(toDecimal(parameterValue, HEX_FORMAT));
            }

            if(parameterName == "shamt") {
                value = std::bitset<5>(std::stoi(parameterValue)).to_string();
            } else if(parameterName == "imm") {
                value = toTwoComplement(std::stoi(parameterValue), 16);
            } else if(parameterName == "addr") {
                value = std::bitset<26>(std::stoi(parameterValue)).to_string();
            }
        } else {
            if(this->executionScope != nullptr && this->instructionAddress != "") {
                std::string labelAddress = executionScope->getLabelAddress(parameterValue);

                if(parameterName == "imm") {
                    value = shiftRightBinary(subBinary(labelAddress, instructionAddress), "10");
                } else if(parameterName == "addr") {
                    value = shiftRightBinary(labelAddress, "10");
                }
            } else {
                value = std::string(fieldSize, parameterName[0]);
            }
        }

        binaryValue = formatBinary(value, fieldSize);
    }

    return binaryValue;
}

/**
 * Calculate a Parameter's Value starting from the Field's Name and the Field's Value
 *
 * @param fieldName Field's Name used to calculate the Parameter's Value
 * @param fieldValue Field's Value that is in Binary to be converted into the Parameter's Value
 * @return Parameter's Value calculated in base of the Field's Name and the Field's Value
 */
std::string Instruction::calculateParameter(std::string fieldName, std::string fieldValue) {
    if(fieldName == "opcode") {
        std::string funct = memoryStructure->getFunct();

        auto pos = instructionPointers.find({fieldValue, funct});
        if(pos != instructionPointers.end()) {
            return pos->second;
        }
    } else if(fieldName == "rs" || fieldName == "rt" || fieldName == "rd") {
        int posRegister = toDecimal(fieldValue);

        if(posRegister >= 0 && posRegister < 32) {
            return registerPointers[posRegister];
        }
    } else if(fieldName == "shamt") {
        return std::to_string(toDecimal(fieldValue));
    } else if(fieldName == "imm") {
        if(this->memoryStructure->getInstructionPurpose() != INSTRUCTION_ADDRESS) {
            return std::to_string(fromTwoComplement(fieldValue));
        } else if(this->executionScope != nullptr && this->instructionAddress != "") {
            std::string labelAddress = addBinary(this->instructionAddress, BranchAddr(fieldValue));

            return executionScope->getLabelName(labelAddress);
        }
    } else if(fieldName == "addr") {
        if(this->executionScope != nullptr && this->instructionAddress != "") {
            std::string labelAddress = JumpAddr(this->instructionAddress, fieldValue);

            return executionScope->getLabelName(labelAddress);
        } else {
            return toHex(fieldValue, 8);
        }
    }

    return "";
}

/**
 * Get the Name of the Instruction
 *
 * @return Name of the Instruction
 */
std::string Instruction::getName() {
    return this->name;
}

/**
 * Get the Parameter's Value in base of the Index
 *
 * @param index Index of the Parameter to get from the parameters array
 * @return Parameters's Value at the index position of the parameters array
 */
std::string Instruction::getParameter(const int index) {
    if(index < this->parameters.size()) {
        return this->parameters[index];
    }

    return "";
}

/**
 * Get the Field's Value in base of its name
 *
 * @param field Field's Name used to retrieve the Field's Value
 * @return Field's Value using the Field's Name to retrieve it
 */
std::string Instruction::getField(std::string field) {
    auto pos = memoryFields.find(field);
    if(pos != memoryFields.end()) {
        return pos->second;
    }

    return "";
}

/**
 * Get the Statement's Type (it can be an Instruction or a Label)
 *
 * @return Statement's Type (INSTRUCTION / LABEL)
 */
enum StatementType Instruction::getStatementType() {
    return this->statementType;
};

/**
 * Get the Instruction's Binary Value
 *
 * @return Binary Value of the Instruction
 */
std::string Instruction::getBinary() {
    if(this->statementType == INSTRUCTION) {
        std::string opcode = this->getField("opcode");

        if(memoryStructure->getFormat() == R_FORMAT) {
            std::string rs = this->getField("rs");
            std::string rt = this->getField("rt");
            std::string rd = this->getField("rd");
            std::string shamt = this->getField("shamt");
            std::string funct = this->getField("funct");

            return opcode + rs + rt + rd + shamt + funct;
        } else if(memoryStructure->getFormat() == I_FORMAT) {
            std::string rs = this->getField("rs");
            std::string rt = this->getField("rt");
            std::string imm = this->getField("imm");

            return opcode + rs + rt + imm;
        } else if(memoryStructure->getFormat() == J_FORMAT) {
            std::string addr = this->getField("addr");

            return opcode + addr;
        }
    } else if(this->statementType == LABEL) {
        return std::string(32, 'x');
    }
}

/**
 * Get the Instruction's Value
 *
 * @return Instruction's Value (String that contains the opcode and the parameters)
 */
std::string Instruction::getInstruction() {
    if(this->statementType == INSTRUCTION) {
        std::vector<std::string> parametersOrder = memoryStructure->getParametersOrder();
        std::string instruction = getName() + " ";

        if(memoryStructure->getFormat() == I_FORMAT && (parametersOrder.size() == 3 && parametersOrder[1] == "imm")) {
            instruction += getParameter(0) + ", " + getParameter(1) + "(" + getParameter(2) + ")";
        } else {
            for(int paramIndex = 0; paramIndex < parametersOrder.size(); paramIndex++) {
                instruction += getParameter(paramIndex);

                if(paramIndex < parametersOrder.size() - 1) instruction += ", ";
            }
        }

        return instruction;
    } else if(this->statementType == LABEL) {
        if(this->name.size()) {
            return getName() + ":";
        } else if(this->executionScope != nullptr && this->instructionAddress != "") {
            return executionScope->getLabelName(instructionAddress) + ":";
        } else {
            return "LABEL (name not assigned): ";
        }
    }
}

void Instruction::setAddress(std::string instructionAddress) {
    this->instructionAddress = formatBinary(instructionAddress, 32);
}

/**
 * Set Field's Value in base of the Field's Name
 *
 * @param fieldName Field's Name used to select the correct Field
 * @param fieldValue Field's Value that is set to the selected Field
 */
void Instruction::setField(std::string fieldName, std::string fieldValue) {
    auto posSize = fieldSizes.find(fieldName);
    if(posSize != fieldSizes.end()) {
        int fieldSize = posSize->second;

        this->memoryFields[fieldName] = formatBinary(fieldValue, fieldSize);
    }
}

/**
 * Set Parameter's Value in base of the Parameter's Index
 *
 * @param index Parameter's Index used to select the correct Parameter in the parameters array
 * @param parameterValue Parameter's Value that is set to the selected Parameter
 */
void Instruction::setParameter(const int index, std::string parameterValue) {
    if(index < this->parameters.size()) {
        this->parameters[index] = parameterValue;
    } else {
        this->parameters.push_back(parameterValue);
    }
}

/**
 * Execute the Instruction running the Instruction's pointed function and modify the status of the Execution Scope
 */
void Instruction::executeInstruction() {
    if(this->executionScope != nullptr) {
        if(this->statementType == INSTRUCTION) {
            std::vector<std::string> funcParams;
            std::vector<std::string> parametersOrder = memoryStructure->getParametersOrder();

            for(int paramIndex = 0; paramIndex < parametersOrder.size(); paramIndex++) {
                funcParams.push_back(this->getField(parametersOrder[paramIndex]));
            }

            memoryStructure->executeFunction(this->executionScope, funcParams);
        } else if(this->statementType == LABEL) {
            this->executionScope->incPC();
        }
    } else {
        std::cout << "ERROR: Cannot execute the instruction, no Execution Scope defined.";
    }
}
