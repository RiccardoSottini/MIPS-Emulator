#include "def.h"

bool isNumber(std::string s) {
    for(unsigned int i = 0; i < s.length(); i++)
        if(std::isdigit(s[i]) == false)
            return false;

    return true;
}


int toDecimal(std::string binaryValue) {
    return std::stoi(binaryValue, nullptr, 2);
}

std::string toHex(std::string binaryValue) {
    std::stringstream res;
    res << std::hex << std::uppercase << std::bitset<32>(binaryValue).to_ulong();

    return "0x" + res.str();
}

std::string toHex(std::string binaryValue, const int hexSize) {
    std::string hexValue = toHex(binaryValue);

    return formatHex(hexValue, hexSize);
}


std::string formatBinary(std::string binaryValue, const int binarySize) {
    if(binarySize > (const) binaryValue.length()) {
        return std::string(binarySize - binaryValue.length(), '0') + binaryValue;
    } else if(binarySize < (const) binaryValue.length()) {
        return binaryValue.erase(0, binaryValue.length() - binarySize);
    }

    return binaryValue;
}

std::string formatHex(std::string hexValue, const int hexSize) {
    if(hexValue.size() >= 2 && hexValue[0] == '0' && hexValue[1] == 'x') {
        hexValue.erase(0, 2);
    }

    if(hexSize > (const) hexValue.length()) {
        return "0x" + std::string(hexSize - hexValue.length(), '0') + hexValue;
    } else if(hexSize < (const) hexValue.length()) {
        return "0x" + hexValue.erase(0, hexValue.length() - hexSize);
    }

    return "0x" + hexValue;
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

std::string norBinary(std::string binaryA, std::string binaryB) {
    int decimalA = std::stoi(binaryA, nullptr, 2);
    int decimalB = std::stoi(binaryB, nullptr, 2);

    return std::bitset<32>(~(decimalA | decimalB)).to_string();
}

std::string orBinary(std::string binaryA, std::string binaryB) {
    int decimalA = std::stoi(binaryA, nullptr, 2);
    int decimalB = std::stoi(binaryB, nullptr, 2);

    return std::bitset<32>(decimalA | decimalB).to_string();
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
    std::string rdValue = funcParams[0];
    std::string rsValue = executionScope->getRegisterValue(funcParams[1]);
    std::string rtValue = executionScope->getRegisterValue(funcParams[2]);
    std::string resultValue = addBinary(rsValue, rtValue);

    executionScope->setRegisterValue(rdValue, resultValue);
    executionScope->incPC();
}

void ADDI_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string rtValue = funcParams[0];
    std::string rsValue = executionScope->getRegisterValue(funcParams[1]);
    std::string immValue = funcParams[2];
    std::string resultValue = addBinary(rsValue, immValue);

    executionScope->setRegisterValue(rtValue, resultValue);
    executionScope->incPC();
}

void ADDIU_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string rtValue = funcParams[0];
    std::string rsValue = executionScope->getRegisterValue(funcParams[1]);
    std::string immValue = funcParams[2];
    std::string resultValue = addBinary(rsValue, immValue);

    executionScope->setRegisterValue(rtValue, resultValue);
    executionScope->incPC();
}

void ADDU_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string rdValue = funcParams[0];
    std::string rsValue = executionScope->getRegisterValue(funcParams[1]);
    std::string rtValue = executionScope->getRegisterValue(funcParams[2]);
    std::string resultValue = addBinary(rsValue, rtValue);

    executionScope->setRegisterValue(rdValue, resultValue);
    executionScope->incPC();
}


void AND_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string rdValue = funcParams[0];
    std::string rsValue = executionScope->getRegisterValue(funcParams[1]);
    std::string rtValue = executionScope->getRegisterValue(funcParams[2]);
    std::string resultValue = andBinary(rsValue, rtValue);

    executionScope->setRegisterValue(rdValue, resultValue);
    executionScope->incPC();
}

void ANDI_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string rtValue = funcParams[0];
    std::string rsValue = executionScope->getRegisterValue(funcParams[1]);
    std::string immValue = funcParams[2];
    std::string resultValue = andBinary(rsValue, immValue);

    executionScope->setRegisterValue(rtValue, resultValue);
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
    std::string addrValue = funcParams[0];

    executionScope->setPC(addrValue);
}

void JAL_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string addrValue = funcParams[0];

    std::string PC = executionScope->getPC();
    std::string newPC = addBinary(PC, "1000");

    executionScope->setRegisterValue("11111", newPC);
    executionScope->setPC(addrValue);
}

void JR_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string rsValue = executionScope->getRegisterValue(funcParams[0]);

    executionScope->setPC(rsValue);
}


void LBU_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string rtValue = funcParams[0];
    std::string immValue = funcParams[1];
    std::string rsValue = executionScope->getRegisterValue(funcParams[2]);

    std::string wordValue = executionScope->getWordValue(rsValue, immValue);
    std::string resultValue = (wordValue.size()) ? wordValue.substr(24, 8) : "";

    executionScope->setRegisterValue(rtValue, resultValue);
    executionScope->incPC();
}

void LHU_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string rtValue = funcParams[0];
    std::string immValue = funcParams[1];
    std::string rsValue = executionScope->getRegisterValue(funcParams[2]);

    std::string wordValue = executionScope->getWordValue(rsValue, immValue);
    std::string resultValue = (wordValue.size()) ? wordValue.substr(16, 16) : "";

    executionScope->setRegisterValue(rtValue, resultValue);
    executionScope->incPC();
}

void LUI_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string rtValue = funcParams[0];
    std::string immValue = funcParams[1];

    std::string resultValue = formatBinary(immValue, 16) + formatBinary("", 16);

    executionScope->setRegisterValue(rtValue, resultValue);
    executionScope->incPC();
}

void LW_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string rtValue = funcParams[0];
    std::string immValue = funcParams[1];
    std::string rsValue = executionScope->getRegisterValue(funcParams[2]);

    std::string resultValue = executionScope->getWordValue(rsValue, immValue);

    executionScope->setRegisterValue(rtValue, resultValue);
    executionScope->incPC();
}

void NOR_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string rdValue = funcParams[0];
    std::string rsValue = executionScope->getRegisterValue(funcParams[1]);
    std::string rtValue = executionScope->getRegisterValue(funcParams[2]);
    std::string resultValue = norBinary(rsValue, rtValue);

    executionScope->setRegisterValue(rdValue, resultValue);
    executionScope->incPC();
}

void OR_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string rdValue = funcParams[0];
    std::string rsValue = executionScope->getRegisterValue(funcParams[1]);
    std::string rtValue = executionScope->getRegisterValue(funcParams[2]);
    std::string resultValue = orBinary(rsValue, rtValue);

    executionScope->setRegisterValue(rdValue, resultValue);
    executionScope->incPC();
}

void ORI_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string rtValue = funcParams[0];
    std::string rsValue = executionScope->getRegisterValue(funcParams[1]);
    std::string immValue = funcParams[2];
    std::string resultValue = orBinary(rsValue, immValue);

    executionScope->setRegisterValue(rtValue, resultValue);
    executionScope->incPC();
}


void SLT_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string rdValue = funcParams[0];
    std::string rsValue = executionScope->getRegisterValue(funcParams[1]);
    std::string rtValue = executionScope->getRegisterValue(funcParams[2]);
    std::string resultValue = (toDecimal(rsValue) < toDecimal(rtValue)) ? "1" : "0";

    executionScope->setRegisterValue(rdValue, resultValue);
    executionScope->incPC();
}

void SLTI_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string rtValue = funcParams[0];
    std::string rsValue = executionScope->getRegisterValue(funcParams[1]);
    std::string immValue = funcParams[2];
    std::string resultValue = (toDecimal(rsValue) < toDecimal(immValue)) ? "1" : "0";

    executionScope->setRegisterValue(rtValue, resultValue);
    executionScope->incPC();
}

void SLTIU_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string rtValue = funcParams[0];
    std::string rsValue = executionScope->getRegisterValue(funcParams[1]);
    std::string immValue = funcParams[2];
    std::string resultValue = (toDecimal(rsValue) < toDecimal(immValue)) ? "1" : "0";

    executionScope->setRegisterValue(rtValue, resultValue);
    executionScope->incPC();
}

void SLTU_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string rdValue = funcParams[0];
    std::string rsValue = executionScope->getRegisterValue(funcParams[1]);
    std::string rtValue = executionScope->getRegisterValue(funcParams[2]);
    std::string resultValue = (toDecimal(rsValue) < toDecimal(rtValue)) ? "1" : "0";

    executionScope->setRegisterValue(rdValue, resultValue);
    executionScope->incPC();
}


void SLL_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string rdValue = funcParams[0];
    std::string rtValue = executionScope->getRegisterValue(funcParams[1]);
    std::string shamtValue = funcParams[2];
    std::string resultValue = shiftLeftBinary(rtValue, shamtValue);

    executionScope->setRegisterValue(rdValue, resultValue);
    executionScope->incPC();
}

void SRL_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string rdValue = funcParams[0];
    std::string rtValue = executionScope->getRegisterValue(funcParams[1]);
    std::string shamtValue = funcParams[2];
    std::string resultValue = shiftRightBinary(rtValue, shamtValue);

    executionScope->setRegisterValue(rdValue, resultValue);
    executionScope->incPC();
}


void SB_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string rtValue = executionScope->getRegisterValue(funcParams[0]);
    std::string immValue = funcParams[1];
    std::string rsValue = executionScope->getRegisterValue(funcParams[2]);

    executionScope->setWordValue(rsValue, immValue, rtValue.substr(24, 8));
    executionScope->incPC();
}

void SH_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string rtValue = executionScope->getRegisterValue(funcParams[0]);
    std::string immValue = funcParams[1];
    std::string rsValue = executionScope->getRegisterValue(funcParams[2]);

    executionScope->setWordValue(rsValue, immValue, rtValue.substr(16, 16));
    executionScope->incPC();
}

void SW_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string rtValue = executionScope->getRegisterValue(funcParams[0]);
    std::string immValue = funcParams[1];
    std::string rsValue = executionScope->getRegisterValue(funcParams[2]);

    executionScope->setWordValue(rsValue, immValue, rtValue);
    executionScope->incPC();
}


void SUB_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string rdValue = funcParams[0];
    std::string rsValue = executionScope->getRegisterValue(funcParams[1]);
    std::string rtValue = executionScope->getRegisterValue(funcParams[2]);
    std::string resultValue = subBinary(rsValue, rtValue);

    executionScope->setRegisterValue(rdValue, resultValue);
    executionScope->incPC();
}

void SUBU_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string rdValue = funcParams[0];
    std::string rsValue = executionScope->getRegisterValue(funcParams[1]);
    std::string rtValue = executionScope->getRegisterValue(funcParams[2]);
    std::string resultValue = subBinary(rsValue, rtValue);

    executionScope->setRegisterValue(rdValue, resultValue);
    executionScope->incPC();
}
