#include "def.h"

bool isNumber(std::string s) {
    for(int i = 0; i < s.length(); i++)
        if(std::isdigit(s[i]) == false)
            return false;

    return true;
}

std::string formatBinary(std::string binaryValue, const int bitsNumber) {
    if(bitsNumber == binaryValue.length()) {
        return binaryValue;
    } else if(bitsNumber > binaryValue.length()) {
        return std::string(bitsNumber - binaryValue.length(), '0') + binaryValue;
    } else if(bitsNumber < binaryValue.length()) {
        return binaryValue.erase(0, binaryValue.length() - bitsNumber);
    }
}

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

std::string shiftLeftBinary(std::string binaryValue, std::string binaryShift) {
    int decimalValue = std::stoi(binaryValue, nullptr, 2);
    int decimalShift = std::stoi(binaryShift, nullptr, 2);

    return std::bitset<32>(decimalValue << decimalShift).to_string();
}

std::string shiftRightBinary(std::string binaryValue, std::string binaryShift) {
    int decimalValue = std::stoi(binaryValue, nullptr, 2);
    int decimalShift = std::stoi(binaryShift, nullptr, 2);

    return std::bitset<32>(decimalValue >> decimalShift).to_string();
}

std::string subBinary(std::string binaryA, std::string binaryB) {
    int decimalA = std::stoi(binaryA, nullptr, 2);
    int decimalB = std::stoi(binaryB, nullptr, 2);

    return std::bitset<32>(decimalA - decimalB).to_string();
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
        executionScope->setPC(immValue);
    } else {
        executionScope->incPC();
    }
}

void BNE_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string rsValue = executionScope->getRegisterValue(funcParams[0]);
    std::string rtValue = executionScope->getRegisterValue(funcParams[1]);
    std::string immValue = funcParams[2];

    if(rsValue.compare(rtValue) != 0) {
        executionScope->setPC(immValue);
    } else {
        executionScope->incPC();
    }
}


void J_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    executionScope->setPC(funcParams[0]);
}


void SLL_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string rtValue = executionScope->getRegisterValue(funcParams[1]);
    std::string shamtValue = funcParams[2];
    std::string resultValue = shiftLeftBinary(rtValue, shamtValue);

    executionScope->setRegisterValue(funcParams[0], resultValue);
    executionScope->incPC();
}

void SRL_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string rtValue = executionScope->getRegisterValue(funcParams[1]);
    std::string shamtValue = funcParams[2];
    std::string resultValue = shiftRightBinary(rtValue, shamtValue);

    executionScope->setRegisterValue(funcParams[0], resultValue);
    executionScope->incPC();
}


void SUB_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string rsValue = executionScope->getRegisterValue(funcParams[1]);
    std::string rtValue = executionScope->getRegisterValue(funcParams[2]);
    std::string resultValue = subBinary(rsValue, rtValue);

    executionScope->setRegisterValue(funcParams[0], resultValue);
    executionScope->incPC();
}

void SUBU_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string rsValue = executionScope->getRegisterValue(funcParams[1]);
    std::string rtValue = executionScope->getRegisterValue(funcParams[2]);
    std::string resultValue = subBinary(rsValue, rtValue);

    executionScope->setRegisterValue(funcParams[0], resultValue);
    executionScope->incPC();
}
