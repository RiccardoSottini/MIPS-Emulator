#include "def.h"

Instruction::Instruction(std::string instruction) {
    this->parseInstruction(instruction);
}

Instruction::Instruction(std::string instruction, ExecutionScope* executionScope) {
    this->parseInstruction(instruction);

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
}

void Instruction::calculateMemory() {
    auto pos = instructionFormats.find(this->name);
    if(pos != instructionFormats.end()) {
        memoryStructure = &pos->second;
        std::vector<std::string> parametersOrder = memoryStructure->getParametersOrder();

        memoryFields["opcode"] = memoryStructure->getOpcode();

        if(memoryStructure->getFormat() == R_FORMAT) {
            memoryFields["rs"] = "00000";
            memoryFields["rt"] = "00000";
            memoryFields["rd"] = "00000";
            memoryFields["shamt"] = "00000";
            memoryFields["funct"] = memoryStructure->getFunct();
        } else if(memoryStructure->getFormat() == I_FORMAT) {
            memoryFields["rs"] = "00000";
            memoryFields["rt"] = "00000";
            memoryFields["imm"] = "0000000000000000";
        } else if(memoryStructure->getFormat() == J_FORMAT) {
            memoryFields["addr"] = "00000000000000000000000000";
        }

        for(int indexP = 0; indexP < parametersOrder.size(); indexP++) {
            std::string parameterValue = this->getParameter(indexP);
            std::string parameterName = parametersOrder[indexP];

            memoryFields[parameterName] = this->calculateField(parameterName, parameterValue);
        }
    } else {
        if(this->statementType == INSTRUCTION) {
            std::cout << "ERROR: Instruction " + this->name + " does not exist!";
        }
    }
}

std::string Instruction::calculateField(std::string parameterName, std::string parameterValue) {
    int bitsNumber = 0;
    std::string binaryValue = "";

    auto posSize = fieldSizes.find(parameterName);
    if(posSize != fieldSizes.end()) {
        bitsNumber = posSize->second;
    }

    if(parameterValue[0] == '$') {
        auto pos = registerFormats.find(parameterValue);
        if(pos != registerFormats.end()) {
            std::string value = pos->second;
            binaryValue = std::string(bitsNumber - value.length(), '0') + value;
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
                value = std::string(bitsNumber, parameterName[0]);
            }
        }

        binaryValue = formatBinary(value, bitsNumber);
    }

    return binaryValue;
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
        return "";
    }
}

enum StatementType Instruction::getStatementType() {
    return this->statementType;
};

void Instruction::printMemory() {
    if(this->statementType == INSTRUCTION) {
        if(memoryStructure->getFormat() == R_FORMAT) {
            this->printFields({"opcode", "rs", "rt", "rd", "shamt", "funct"});
        } else if(memoryStructure->getFormat() == I_FORMAT) {
            this->printFields({"opcode", "rs", "rt", "imm"});
        } else if(memoryStructure->getFormat() == J_FORMAT) {
            this->printFields({"opcode", "addr"});
        }
    }
}

void Instruction::printField(std::string field) {
    std::string fieldValue = this->getField(field);

    if(fieldValue != "") {
        std::cout << field << ": " << fieldValue << std::endl;
    }
}

void Instruction::printFields(std::vector<std::string> fields) {
    for(int fieldIndex = 0; fieldIndex < fields.size(); fieldIndex++) {
        this->printField(fields[fieldIndex]);
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
