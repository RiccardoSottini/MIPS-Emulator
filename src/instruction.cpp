#include "def.h"

Instruction::Instruction(std::string value, InputType inputType) {
    if(inputType == INSTRUCTION_VALUE) {
        parseInstruction(value);
    } else if(inputType == BINARY_VALUE) {
        parseBinary(value);
    }
}

Instruction::Instruction(std::string value, InputType inputType, ExecutionScope* executionScope) {
    if(inputType == INSTRUCTION_VALUE) {
        parseInstruction(value);
    } else if(inputType == BINARY_VALUE) {
        parseBinary(value);
    }

    this->executionScope = executionScope;
}

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

void Instruction::calculateBinary() {
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
}

void Instruction::calculateInstruction() {
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
}

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

        if(isNumber(parameterValue)) {
            if(parameterName == "shamt") {
                value = std::bitset<5>(std::stoi(parameterValue)).to_string();
            } else if(parameterName == "imm") {
                value = std::bitset<16>(std::stoi(parameterValue)).to_string();
            } else if(parameterName == "addr") {
                value = std::bitset<26>(std::stoi(parameterValue)).to_string();
            }
        } else {
            if(executionScope != nullptr) {
                value = executionScope->getLabelAddress(parameterValue);
            } else {
                value = std::string(fieldSize, parameterName[0]);
            }
        }

        binaryValue = formatBinary(value, fieldSize);
    }

    return binaryValue;
}

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
    } else if(fieldName == "shamt" || fieldName == "imm" || fieldName == "addr") {
        return std::to_string(toDecimal(fieldValue));
    }

    return "";
}

std::string Instruction::getName() {
    return this->name;
}

std::string Instruction::getParameter(const int index) {
    if(index < this->parameters.size()) {
        return this->parameters[index];
    }

    return "";
}

std::string Instruction::getField(std::string field) {
    auto pos = memoryFields.find(field);
    if(pos != memoryFields.end()) {
        return pos->second;
    }

    return "";
}

enum StatementType Instruction::getStatementType() {
    return this->statementType;
};

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

std::string Instruction::getInstruction() {
    if(this->statementType == INSTRUCTION) {
        std::vector<std::string> parametersOrder = memoryStructure->getParametersOrder();
        std::string instruction = getName() + " ";

        if(memoryStructure->getFormat() == I_FORMAT && (parametersOrder.size() == 3 && parametersOrder[1] == "imm")) {
            instruction = getParameter(0) + ", " + getParameter(1) + "(" + getParameter(2) + ")";
        } else {
            for(int paramIndex = 0; paramIndex < parametersOrder.size(); paramIndex++) {
                instruction += getParameter(paramIndex);

                if(paramIndex < parametersOrder.size() - 1) instruction += ", ";
            }
        }

        return instruction;
    } else if(this->statementType == LABEL) {
        if(this->name.size()) {
            return getName() + ": ";
        } else {
            return "LABEL (name not assigned): ";
        }
    }
}

void Instruction::setField(std::string fieldName, std::string fieldValue) {
    auto posSize = fieldSizes.find(fieldName);
    if(posSize != fieldSizes.end()) {
        int fieldSize = posSize->second;

        this->memoryFields[fieldName] = formatBinary(fieldValue, fieldSize);
    }
}

void Instruction::setParameter(const int index, std::string parameterValue) {
    if(index < this->parameters.size()) {
        this->parameters[index] = parameterValue;
    } else {
        this->parameters.push_back(parameterValue);
    }
}

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
