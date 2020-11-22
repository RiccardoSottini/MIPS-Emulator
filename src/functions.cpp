#include "def.h"

bool isNumber(std::string s);

std::string addBinary(std::string binaryA, std::string binaryB) {
    int decimalA = std::stoi(binaryA, nullptr, 2);
    int decimalB = std::stoi(binaryB, nullptr, 2);

    return std::bitset<32>(decimalA + decimalB).to_string();
}

std::string andBinary(std::string binaryA, std::string binaryB) {
    int decimalA = std::stoi(binaryA, nullptr, 2);
    int decimalB = std::stoi(binaryB, nullptr, 2);

    return std::bitset<32>(decimalA & decimalB).to_string();
}


void ADD_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string rsValue = executionScope->getRegisterValue(funcParams[1]);
    std::string rtValue = executionScope->getRegisterValue(funcParams[2]);
    std::string resultValue = addBinary(rsValue, rtValue);

    executionScope->setRegisterValue(funcParams[0], resultValue);
    executionScope->incPC();
}

void ADDI_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string rsValue = executionScope->getRegisterValue(funcParams[1]);
    std::string immValue = funcParams[2];
    std::string resultValue = addBinary(rsValue, immValue);

    executionScope->setRegisterValue(funcParams[0], resultValue);
    executionScope->incPC();
}

void ADDIU_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string rsValue = executionScope->getRegisterValue(funcParams[1]);
    std::string immValue = funcParams[2];
    std::string resultValue = addBinary(rsValue, immValue);

    executionScope->setRegisterValue(funcParams[0], resultValue);
    executionScope->incPC();
}

void ADDU_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string rsValue = executionScope->getRegisterValue(funcParams[1]);
    std::string rtValue = executionScope->getRegisterValue(funcParams[2]);
    std::string resultValue = addBinary(rsValue, rtValue);

    executionScope->setRegisterValue(funcParams[0], resultValue);
    executionScope->incPC();
}


void AND_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string rsValue = executionScope->getRegisterValue(funcParams[1]);
    std::string rtValue = executionScope->getRegisterValue(funcParams[2]);
    std::string resultValue = andBinary(rsValue, rtValue);

    executionScope->setRegisterValue(funcParams[0], resultValue);
    executionScope->incPC();
}

void ANDI_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string rsValue = executionScope->getRegisterValue(funcParams[1]);
    std::string immValue = funcParams[2];
    std::string resultValue = andBinary(rsValue, immValue);

    executionScope->setRegisterValue(funcParams[0], resultValue);
    executionScope->incPC();
}


void BEQ_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string rsValue = executionScope->getRegisterValue(funcParams[0]);
    std::string rtValue = executionScope->getRegisterValue(funcParams[1]);
    std::string immValue = funcParams[2];

    if(rsValue.compare(rtValue) == 0) {
        executionScope->jumpLabel(immValue);
    } else {
        executionScope->incPC();
    }
}

void BNE_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string rsValue = executionScope->getRegisterValue(funcParams[0]);
    std::string rtValue = executionScope->getRegisterValue(funcParams[1]);
    std::string immValue = funcParams[2];

    if(rsValue.compare(rtValue) != 0) {
        executionScope->jumpLabel(immValue);
    } else {
        executionScope->incPC();
    }
}


void J_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    executionScope->jumpLabel(funcParams[0]);
}
