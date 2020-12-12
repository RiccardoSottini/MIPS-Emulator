#include "../includes/mips_emulator.h"

/**
 * Check if the String contains a Number
 *
 * @param s Text that has to be evaluated
 * @return True if the String contains a Number / False if it does not
 */
bool isNumber(std::string s) {
    if(s.size() && (s[0] == '+' || s[0] == '-'))
        s.erase(0, 1);

    for(unsigned int i = 0; i < s.length(); i++)
        if(std::isdigit(s[i]) == false)
            return false;

    return true;
}

/**
 * Check if the String contains an Hexadecimal Value
 *
 * @param hexValue Text that has to be evaluated
 * @return True if the String contains an Hexadecimal Value / False if it does not
 */
bool isHex(std::string hexValue) {
    return hexValue.size() >= 2 && hexValue[0] == '0' && hexValue[1] == 'x';
}

/**
 * Convert a Binary Value into its Decimal Value
 *
 * @param binaryValue Binary Value to be converted
 * @return Decimal Value of the Binary Value converted
 */
int toDecimal(std::string binaryValue) {
    return std::stoi(binaryValue, nullptr, 2);
}

/**
 * Convert a String that can be a Binary or Hexadecimal Value into its Decimal Value
 *
 * @param inputValue Binary or Hexadecimal Value to be converted
 * @param inputFormat Format used for the value gave as input
 * @return Decimal Value of the Binary or Hexadecimal Value converted
 */
int toDecimal(std::string inputValue, enum DataFormat inputFormat) {
    if(inputFormat == BIN_FORMAT) {
        return toDecimal(inputValue);
    } else if(inputFormat == HEX_FORMAT) {
        int decimalValue;
        std::stringstream res;

        if(isHex(inputValue)) {
            inputValue.erase(0, 2);
        }

        res << std::hex << inputValue;
        res >> decimalValue;

        return decimalValue;
    }
}

/**
 * Convert a Decimal Value into its Binary Value
 *
 * @param decimalValue Decimal Value to be converted
 * @return Binary Value of the Decimal Value converted
 */
std::string toBinary(const int decimalValue) {
    return std::bitset<32>(decimalValue).to_string();
}

/**
 * Convert a Binary Value into its Hexadecimal Value
 *
 * @param binaryValue Binary Value to be converted
 * @return Hexadecimal Value of the Binary Value converted
 */
std::string toHex(std::string binaryValue) {
    std::stringstream res;
    res << std::hex << std::uppercase << std::bitset<32>(binaryValue).to_ulong();

    return "0x" + res.str();
}

/**
 * Convert a Binary Value into its Hexadecimal Value with a Fixed Size
 *
 * @param binaryValue Binary Value to be converted
 * @param hexSize Size of the Hexadecimal Value to be returned
 * @return Hexadecimal Value of the Binary Value converted
 */
std::string toHex(std::string binaryValue, const int hexSize) {
    std::string hexValue = toHex(binaryValue);

    return formatHex(hexValue, hexSize);
}

/**
 * Convert a Decimal Value into Two's Complement
 *
 * @param decimalValue Decimal Value to be converted
 * @param binarySize Size of the Binary Value to be returned
 * @return Binary Value that is the Two's Complement of the Decimal Value gave as argument
 */
std::string toTwoComplement(const int decimalValue, const int binarySize) {
    std::string binaryConversion = toBinary(abs(decimalValue));
    std::string binaryValue = formatBinary(binaryConversion, binarySize);

    if(decimalValue >= 0) {
        return binaryValue;
    } else {
        std::string invertedBinary = invertBinary(binaryValue);
        return addBinary(invertedBinary, "1");
    }
}

/**
 * Convert a Decimal Value into Two's Complement
 *
 * @param decimalValue Decimal Value to be converted
 * @return Binary Value that is the Two's Complement of the Decimal Value gave as argument
 */
int fromTwoComplement(std::string binaryValue) {
    if(binaryValue[0] == '0') {
        return toDecimal(binaryValue);
    } else {
        std::string invertedBinary = invertBinary(binaryValue);
        std::string binaryValue = addBinary(invertedBinary, "1");
        return -toDecimal(binaryValue);
    }
}

/**
 * Transform a Binary Value and make it to have a Fixed Length specified by the Size parameter
 *
 * @param binaryValue Binary Value to be transformed in the version with the Fixed Length
 * @param binarySize Length of the Binary Value to be returned
 * @return The Binary Value transformed to have the Fixed Length specified by the Size parameter
 */
std::string formatBinary(std::string binaryValue, const int binarySize) {
    if(binarySize > (const) binaryValue.length()) {
        return std::string(binarySize - binaryValue.length(), '0') + binaryValue;
    } else if(binarySize < (const) binaryValue.length()) {
        return binaryValue.erase(0, binaryValue.length() - binarySize);
    }

    return binaryValue;
}

/**
 * Transform an Hexadecimal Value and make it to have a Fixed Length specified by the Size parameter
 *
 * @param hexValue Hexadecimal Value to be transformed in the version with the Fixed Length
 * @param hexSize Length of the Hexadecimal Value to be returned
 * @return The Hexadecimal Value transformed to have the Fixed Length specified by the Size parameter
 */
std::string formatHex(std::string hexValue, const int hexSize) {
    if(isHex(hexValue)) {
        hexValue.erase(0, 2);
    }

    if(hexSize > (const) hexValue.length()) {
        return "0x" + std::string(hexSize - hexValue.length(), '0') + hexValue;
    } else if(hexSize < (const) hexValue.length()) {
        return "0x" + hexValue.erase(0, hexValue.length() - hexSize);
    }

    return "0x" + hexValue;
}

/**
 * Invert a Binary Value
 *
 * @param binaryValue Binary Value to be inverted
 * @return Result of the reverse of the Binary Value gave as argument
 */
std::string invertBinary(std::string binaryValue) {
    std::string binaryResult;

    for(int index = 0; index < binaryValue.size(); index++) {
        int bit = binaryValue[index] - 48;

        binaryResult.push_back((!bit) + 48);
    }

    return binaryResult;
}

/**
 * ADD two Binary Values
 *
 * @param binaryA First Binary Value
 * @param binaryB Second Binary Value
 * @return Sum between the two Binary Values
 */
std::string addBinary(std::string binaryA, std::string binaryB) {
    std::string binaryResult;
    binaryA = formatBinary(binaryA, 32);
    binaryB = formatBinary(binaryB, 32);

    int carry = 0;
    for(int index = 31; index >= 0; index--) {
        int a = binaryA[index] - 48;
        int b = binaryB[index] - 48;

        int sum = ((a ^ b) ^ carry);                    // sum = a xor b xor c
        carry = ((a & b) | (a & carry)) | (b & carry);  // carry = ab + ac + bc

        binaryResult.push_back(sum + 48);
    }

    std::reverse(binaryResult.begin(), binaryResult.end());
    return binaryResult;
}

/**
 * SUB two Binary Values
 *
 * @param binaryA First Binary Value
 * @param binaryB Second Binary Value
 * @return Sum between the two Binary Values
 */
std::string subBinary(std::string binaryA, std::string binaryB) {
    binaryA = formatBinary(binaryA, 32);
    binaryB = formatBinary(binaryB, 32);

    std::string invertedB = invertBinary(binaryB);
    binaryB = addBinary(invertedB, "1");

    return addBinary(binaryA, binaryB);
}

/**
 * MULTIPLY two Binary Values
 *
 * @param binaryA First Binary Value
 * @param binaryB Second Binary Value
 * @return Multiplication between the two Binary Values
 */
std::string mulBinary(std::string binaryA, std::string binaryB) {
    std::string binaryResult = formatBinary("", 32);
    binaryA = formatBinary(binaryA, 32);
    binaryB = formatBinary(binaryB, 32);

    for(int indexB = 31; indexB >= 0; indexB--) {
        int b = binaryB[indexB] - 48;

        if(b == 1) {
            std::string binaryShifted = shiftLeftBinary(binaryA, toBinary(31 - indexB));
            binaryResult = addBinary(binaryResult, binaryShifted);
        }
    }

    return binaryResult;
}

/**
 * DIVIDE two Binary Values
 *
 * @param binaryA First Binary Value
 * @param binaryB Second Binary Value
 * @return Division between the two Binary Values
 */
std::string divBinary(std::string binaryA, std::string binaryB) {
    int decimalA = std::stoi(binaryA, nullptr, 2);
    int decimalB = std::stoi(binaryB, nullptr, 2);

    return std::bitset<32>((int)(decimalA / decimalB)).to_string();
}

/**
 * AND Logical Operation between two Binary Values
 *
 * @param binaryA First Binary Value
 * @param binaryB Second Binary Value
 * @return Result of the AND Logical Operation between the two Binary Values
 */
std::string andBinary(std::string binaryA, std::string binaryB) {
    std::string binaryResult;
    binaryA = formatBinary(binaryA, 32);
    binaryB = formatBinary(binaryB, 32);

    for(int index = 0; index < 32; index++) {
        int a = binaryA[index] - 48;
        int b = binaryB[index] - 48;

        int sum = a & b;

        binaryResult.push_back(sum + 48);
    }

    return binaryResult;
}

/**
 * NOR Logical Operation between two Binary Values
 *
 * @param binaryA First Binary Value
 * @param binaryB Second Binary Value
 * @return Result of the NOR Logical Operation between the two Binary Values
 */
std::string norBinary(std::string binaryA, std::string binaryB) {
    std::string binaryResult;
    binaryA = formatBinary(binaryA, 32);
    binaryB = formatBinary(binaryB, 32);

    for(int index = 0; index < 32; index++) {
        int a = binaryA[index] - 48;
        int b = binaryB[index] - 48;

        int sum = ~(a | b);

        binaryResult.push_back(sum + 48);
    }

    return binaryResult;
}

/**
 * OR Logical Operation between two Binary Values
 *
 * @param binaryA First Binary Value
 * @param binaryB Second Binary Value
 * @return Result of the OR Logical Operation between the two Binary Values
 */
std::string orBinary(std::string binaryA, std::string binaryB) {
    std::string binaryResult;
    binaryA = formatBinary(binaryA, 32);
    binaryB = formatBinary(binaryB, 32);

    for(int index = 0; index < 32; index++) {
        int a = binaryA[index] - 48;
        int b = binaryB[index] - 48;

        int sum = a | b;

        binaryResult.push_back(sum + 48);
    }

    return binaryResult;
}

/**
 * Shift-Left Logical Operation on a Binary Value by a number of bit specified by the Shift parameter
 *
 * @param binaryValue Binary Value in which shift the Value by a specified number of bit
 * @param binaryShift Specifies the number of bit to shift the Binary Value to the Left
 * @return Result of the Shift Left Logical Operation on the Binary Value
 */
std::string shiftLeftBinary(std::string binaryValue, std::string binaryShift) {
    int decimalValue = std::stoi(binaryValue, nullptr, 2);
    int decimalShift = std::stoi(binaryShift, nullptr, 2);

    return std::bitset<32>(decimalValue << decimalShift).to_string();
}

/**
 * Shift-Right Logical Operation on a Binary Value by a number of bit specified by the Shift parameter
 *
 * @param binaryValue Binary Value in which shift the Value by a specified number of bit
 * @param binaryShift Specifies the number of bit to shift the Binary Value to the Right
 * @return Result of the Shift Right Logical Operation on the Binary Value
 */
std::string shiftRightBinary(std::string binaryValue, std::string binaryShift) {
    int decimalValue = std::stoi(binaryValue, nullptr, 2);
    int decimalShift = std::stoi(binaryShift, nullptr, 2);

    return std::bitset<32>(decimalValue >> decimalShift).to_string();
}

/**
 * Convert Immediate value into the Signed Extension
 *
 * @param immediate Immediate Value gave as argument
 * @return Signed Extension of the Immediate Value
 */
std::string SignExtImm(std::string immediate) {
    immediate = formatBinary(immediate, 16);
    char signExt = immediate[0];

    return std::string(16, signExt) + immediate;
}

/**
 * Convert Immediate value into the Zero Extension
 *
 * @param immediate Immediate Value gave as argument
 * @return Zero Extension of the Immediate Value
 */
std::string ZeroExtImm(std::string immediate) {
    immediate = formatBinary(immediate, 16);

    return std::string(16, '0') + immediate;
}

/**
 * Calculate the Branch Address from the Immediate Value
 *
 * @param immediate Immediate Value gave as argument
 * @return Branch Address calculated from the Immediate Value
 */
std::string BranchAddr(std::string immediate) {
    immediate = formatBinary(immediate, 16);
    char signExt = immediate[0];

    return std::string(14, signExt) + immediate + std::string(2, '0');
}

/**
 * Calculate the Jump Address from the Program Counter and the Address Value
 *
 * @param PC Program Counter gave as argument
 * @param address Address Value gave as argument
 * @return Jump Address calculated from the Program Counter and the Address Value
 */
std::string JumpAddr(std::string PC, std::string address) {
    address = formatBinary(address, 26);
    std::string newPC = addBinary(PC, "100");

    return newPC.substr(0, 4) + address + std::string(2, '0');
}

/**
 * ADD Instruction -> R[rd] = R[rs] + R[rt]
 *
 * @param executionScope Execution Scope
 * @param funcParams Parameters Values (rd, rs, rt)
 */
void ADD_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string rdValue = funcParams[0];
    std::string rsValue = executionScope->getRegisterValue(funcParams[1]);
    std::string rtValue = executionScope->getRegisterValue(funcParams[2]);
    std::string resultValue = addBinary(rsValue, rtValue);

    executionScope->setRegisterValue(rdValue, resultValue);
    executionScope->incPC();
}

/**
 * ADDI Instruction -> R[rt] = R[rs] + SignExtImm
 *
 * @param executionScope Execution Scope
 * @param funcParams Parameters Values (rt, rs, imm)
 */
void ADDI_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string rtValue = funcParams[0];
    std::string rsValue = executionScope->getRegisterValue(funcParams[1]);
    std::string immValue = SignExtImm(funcParams[2]);
    std::string resultValue = addBinary(rsValue, immValue);

    executionScope->setRegisterValue(rtValue, resultValue);
    executionScope->incPC();
}

/**
 * ADDIU Instruction -> R[rt] = R[rs] + SignExtImm
 *
 * @param executionScope Execution Scope
 * @param funcParams Parameters Values (rt, rs, imm)
 */
void ADDIU_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string rtValue = funcParams[0];
    std::string rsValue = executionScope->getRegisterValue(funcParams[1]);
    std::string immValue = SignExtImm(funcParams[2]);
    std::string resultValue = addBinary(rsValue, immValue);

    executionScope->setRegisterValue(rtValue, resultValue);
    executionScope->incPC();
}

/**
 * ADDU Instruction -> R[rd] = R[rs] + R[rt]
 *
 * @param executionScope Execution Scope
 * @param funcParams Parameters Values (rd, rs, rt)
 */
void ADDU_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string rdValue = funcParams[0];
    std::string rsValue = executionScope->getRegisterValue(funcParams[1]);
    std::string rtValue = executionScope->getRegisterValue(funcParams[2]);
    std::string resultValue = addBinary(rsValue, rtValue);

    executionScope->setRegisterValue(rdValue, resultValue);
    executionScope->incPC();
}

/**
 * AND Instruction -> R[rd] = R[rs] & R[rt]
 *
 * @param executionScope Execution Scope
 * @param funcParams Parameters Values (rd, rs, rt)
 */
void AND_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string rdValue = funcParams[0];
    std::string rsValue = executionScope->getRegisterValue(funcParams[1]);
    std::string rtValue = executionScope->getRegisterValue(funcParams[2]);
    std::string resultValue = andBinary(rsValue, rtValue);

    executionScope->setRegisterValue(rdValue, resultValue);
    executionScope->incPC();
}

/**
 * ANDI Instruction -> R[rt] = R[rs] & ZeroExtImm
 *
 * @param executionScope Execution Scope
 * @param funcParams Parameters Values (rt, rs, imm)
 */
void ANDI_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string rtValue = funcParams[0];
    std::string rsValue = executionScope->getRegisterValue(funcParams[1]);
    std::string immValue = ZeroExtImm(funcParams[2]);
    std::string resultValue = andBinary(rsValue, immValue);

    executionScope->setRegisterValue(rtValue, resultValue);
    executionScope->incPC();
}

/**
 * BEQ Instruction -> if(R[rs] == R[rt]) { PC = PC + 4 + BranchAddr }
 *
 * @param executionScope Execution Scope
 * @param funcParams Parameters Values (rs, rt, imm)
 */
void BEQ_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string rsValue = executionScope->getRegisterValue(funcParams[0]);
    std::string rtValue = executionScope->getRegisterValue(funcParams[1]);
    std::string immValue = funcParams[2];

    if(rsValue.compare(rtValue) == 0) {
        executionScope->setPC(immValue, PC_RELATIVE_ADDRESSING);
    } else {
        executionScope->incPC();
    }
}

/**
 * BNE Instruction -> if(R[rs] != R[rt]) { PC = PC + 4 + BranchAddr }
 *
 * @param executionScope Execution Scope
 * @param funcParams Parameters Values (rs, rt, imm)
 */
void BNE_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string rsValue = executionScope->getRegisterValue(funcParams[0]);
    std::string rtValue = executionScope->getRegisterValue(funcParams[1]);
    std::string immValue = funcParams[2];

    if(rsValue.compare(rtValue) != 0) {
        executionScope->setPC(immValue, PC_RELATIVE_ADDRESSING);
    } else {
        executionScope->incPC();
    }
}

/**
 * J Instruction -> PC = JumpAddr
 *
 * @param executionScope Execution Scope
 * @param funcParams Parameters Values (addr)
 */
void J_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string addrValue = funcParams[0];

    executionScope->setPC(addrValue, PSEUDO_DIRECT_ADDRESSING);
}

/**
 * JAL Instruction -> R[31] = PC + 4; PC = JumpAddr
 *
 * @param executionScope Execution Scope
 * @param funcParams Parameters Values (addr)
 */
void JAL_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string addrValue = funcParams[0];

    std::string PC = executionScope->getPC();
    std::string newPC = addBinary(PC, "100");

    executionScope->setRegisterValue("11111", newPC);
    executionScope->setPC(addrValue, PSEUDO_DIRECT_ADDRESSING);
}

/**
 * JR Instruction -> PC = R[rs]
 *
 * @param executionScope Execution Scope
 * @param funcParams Parameters Values (rs)
 */
void JR_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string rsValue = executionScope->getRegisterValue(funcParams[0]);

    executionScope->setPC(rsValue, REGISTER_ADDRESSING);
}

/**
 * LBU Instruction -> R[rt] = {24’b0, M[R[rs] + SignExtImm](7:0)}
 *
 * @param executionScope Execution Scope
 * @param funcParams Parameters Values (rt, imm, rs)
 */
void LBU_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string rtValue = funcParams[0];
    std::string immValue = SignExtImm(funcParams[1]);
    std::string rsValue = executionScope->getRegisterValue(funcParams[2]);

    std::string wordValue = executionScope->getWordValue(rsValue, immValue);
    std::string resultValue = (wordValue.size()) ? wordValue.substr(24, 8) : "";

    executionScope->setRegisterValue(rtValue, resultValue);
    executionScope->incPC();
}

/**
 * LHU Instruction -> R[rt] = {16’b0, M[R[rs] + SignExtImm](15:0)}
 *
 * @param executionScope Execution Scope
 * @param funcParams Parameters Values (rt, imm, rs)
 */
void LHU_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string rtValue = funcParams[0];
    std::string immValue = SignExtImm(funcParams[1]);
    std::string rsValue = executionScope->getRegisterValue(funcParams[2]);

    std::string wordValue = executionScope->getWordValue(rsValue, immValue);
    std::string resultValue = (wordValue.size()) ? wordValue.substr(16, 16) : "";

    executionScope->setRegisterValue(rtValue, resultValue);
    executionScope->incPC();
}

/**
 * LUI Instruction -> R[rt] = {imm, 16’b0}
 *
 * @param executionScope Execution Scope
 * @param funcParams Parameters Values (rt, imm)
 */
void LUI_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string rtValue = funcParams[0];
    std::string immValue = funcParams[1];

    std::string resultValue = formatBinary(immValue, 16) + formatBinary("", 16);

    executionScope->setRegisterValue(rtValue, resultValue);
    executionScope->incPC();
}

/**
 * LW Instruction -> R[rt] = M[R[rs] + SignExtImm]
 *
 * @param executionScope Execution Scope
 * @param funcParams Parameters Values (rt, imm, rs)
 */
void LW_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string rtValue = funcParams[0];
    std::string immValue = SignExtImm(funcParams[1]);
    std::string rsValue = executionScope->getRegisterValue(funcParams[2]);

    std::string resultValue = executionScope->getWordValue(rsValue, immValue);

    executionScope->setRegisterValue(rtValue, resultValue);
    executionScope->incPC();
}

/**
 * NOR Instruction -> R[rd] = ~(R[rs] | R[rt])
 *
 * @param executionScope Execution Scope
 * @param funcParams Parameters Values (rd, rs, rt)
 */
void NOR_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string rdValue = funcParams[0];
    std::string rsValue = executionScope->getRegisterValue(funcParams[1]);
    std::string rtValue = executionScope->getRegisterValue(funcParams[2]);
    std::string resultValue = norBinary(rsValue, rtValue);

    executionScope->setRegisterValue(rdValue, resultValue);
    executionScope->incPC();
}

/**
 * OR Instruction -> R[rd] = R[rs] | R[rt]
 *
 * @param executionScope Execution Scope
 * @param funcParams Parameters Values (rd, rs, rt)
 */
void OR_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string rdValue = funcParams[0];
    std::string rsValue = executionScope->getRegisterValue(funcParams[1]);
    std::string rtValue = executionScope->getRegisterValue(funcParams[2]);
    std::string resultValue = orBinary(rsValue, rtValue);

    executionScope->setRegisterValue(rdValue, resultValue);
    executionScope->incPC();
}

/**
 * ORI Instruction -> R[rt] = R[rs] | ZeroExtImm
 *
 * @param executionScope Execution Scope
 * @param funcParams Parameters Values (rt, rs, imm)
 */
void ORI_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string rtValue = funcParams[0];
    std::string rsValue = executionScope->getRegisterValue(funcParams[1]);
    std::string immValue = ZeroExtImm(funcParams[2]);
    std::string resultValue = orBinary(rsValue, immValue);

    executionScope->setRegisterValue(rtValue, resultValue);
    executionScope->incPC();
}

/**
 * SLT Instruction -> R[rd] = (R[rs] < R[rt]) ? 1 : 0
 *
 * @param executionScope Execution Scope
 * @param funcParams Parameters Values (rd, rs, rt)
 */
void SLT_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string rdValue = funcParams[0];
    std::string rsValue = executionScope->getRegisterValue(funcParams[1]);
    std::string rtValue = executionScope->getRegisterValue(funcParams[2]);
    std::string resultValue = (toDecimal(rsValue) < toDecimal(rtValue)) ? "1" : "0";

    executionScope->setRegisterValue(rdValue, resultValue);
    executionScope->incPC();
}

/**
 * SLTI Instruction -> R[rt] = (R[rs] < SignExtImm) ? 1 : 0
 *
 * @param executionScope Execution Scope
 * @param funcParams Parameters Values (rt, rs, imm)
 */
void SLTI_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string rtValue = funcParams[0];
    std::string rsValue = executionScope->getRegisterValue(funcParams[1]);
    std::string immValue = SignExtImm(funcParams[2]);
    std::string resultValue = (toDecimal(rsValue) < toDecimal(immValue)) ? "1" : "0";

    executionScope->setRegisterValue(rtValue, resultValue);
    executionScope->incPC();
}

/**
 * SLTIU Instruction -> R[rt] = (R[rs] < SignExtImm) ? 1 : 0
 *
 * @param executionScope Execution Scope
 * @param funcParams Parameters Values (rt, rs, imm)
 */
void SLTIU_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string rtValue = funcParams[0];
    std::string rsValue = executionScope->getRegisterValue(funcParams[1]);
    std::string immValue = SignExtImm(funcParams[2]);
    std::string resultValue = (toDecimal(rsValue) < toDecimal(immValue)) ? "1" : "0";

    executionScope->setRegisterValue(rtValue, resultValue);
    executionScope->incPC();
}

/**
 * SLTU Instruction -> R[rd] = (R[rs] < R[rt]) ? 1 : 0
 *
 * @param executionScope Execution Scope
 * @param funcParams Parameters Values (rd, rs, rt)
 */
void SLTU_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string rdValue = funcParams[0];
    std::string rsValue = executionScope->getRegisterValue(funcParams[1]);
    std::string rtValue = executionScope->getRegisterValue(funcParams[2]);
    std::string resultValue = (toDecimal(rsValue) < toDecimal(rtValue)) ? "1" : "0";

    executionScope->setRegisterValue(rdValue, resultValue);
    executionScope->incPC();
}

/**
 * SLL Instruction -> R[rd] = R[rt] << shamt
 *
 * @param executionScope Execution Scope
 * @param funcParams Parameters Values (rd, rt, shamt)
 */
void SLL_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string rdValue = funcParams[0];
    std::string rtValue = executionScope->getRegisterValue(funcParams[1]);
    std::string shamtValue = funcParams[2];
    std::string resultValue = shiftLeftBinary(rtValue, shamtValue);

    executionScope->setRegisterValue(rdValue, resultValue);
    executionScope->incPC();
}

/**
 * SRL Instruction -> R[rd] = R[rt] >> shamt
 *
 * @param executionScope Execution Scope
 * @param funcParams Parameters Values (rd, rt, shamt)
 */
void SRL_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string rdValue = funcParams[0];
    std::string rtValue = executionScope->getRegisterValue(funcParams[1]);
    std::string shamtValue = funcParams[2];
    std::string resultValue = shiftRightBinary(rtValue, shamtValue);

    executionScope->setRegisterValue(rdValue, resultValue);
    executionScope->incPC();
}

/**
 * SB Instruction -> M[R[rs] + SignExtImm](7:0) = R[rt](7:0)
 *
 * @param executionScope Execution Scope
 * @param funcParams Parameters Values (rt, imm, rs)
 */
void SB_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string rtValue = executionScope->getRegisterValue(funcParams[0]);
    std::string immValue = SignExtImm(funcParams[1]);
    std::string rsValue = executionScope->getRegisterValue(funcParams[2]);

    executionScope->setWordValue(rsValue, immValue, rtValue.substr(24, 8));
    executionScope->incPC();
}

/**
 * SH Instruction -> M[R[rs] + SignExtImm](15:0) = R[rt](15:0)
 *
 * @param executionScope Execution Scope
 * @param funcParams Parameters Values (rt, imm, rs)
 */
void SH_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string rtValue = executionScope->getRegisterValue(funcParams[0]);
    std::string immValue = SignExtImm(funcParams[1]);
    std::string rsValue = executionScope->getRegisterValue(funcParams[2]);

    executionScope->setWordValue(rsValue, immValue, rtValue.substr(16, 16));
    executionScope->incPC();
}

/**
 * SW Instruction -> M[R[rs] + SignExtImm] = R[rt]
 *
 * @param executionScope Execution Scope
 * @param funcParams Parameters Values (rt, imm, rs)
 */
void SW_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string rtValue = executionScope->getRegisterValue(funcParams[0]);
    std::string immValue = SignExtImm(funcParams[1]);
    std::string rsValue = executionScope->getRegisterValue(funcParams[2]);

    executionScope->setWordValue(rsValue, immValue, rtValue);
    executionScope->incPC();
}

/**
 * SUB Instruction -> R[rd] = R[rs] - R[rt]
 *
 * @param executionScope Execution Scope
 * @param funcParams Parameters Values (rd, rs, rt)
 */
void SUB_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string rdValue = funcParams[0];
    std::string rsValue = executionScope->getRegisterValue(funcParams[1]);
    std::string rtValue = executionScope->getRegisterValue(funcParams[2]);
    std::string resultValue = subBinary(rsValue, rtValue);

    executionScope->setRegisterValue(rdValue, resultValue);
    executionScope->incPC();
}

/**
 * SUBU Instruction -> R[rd] = R[rs] - R[rt]
 *
 * @param executionScope Execution Scope
 * @param funcParams Parameters Values (rd, rs, rt)
 */
void SUBU_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string rdValue = funcParams[0];
    std::string rsValue = executionScope->getRegisterValue(funcParams[1]);
    std::string rtValue = executionScope->getRegisterValue(funcParams[2]);
    std::string resultValue = subBinary(rsValue, rtValue);

    executionScope->setRegisterValue(rdValue, resultValue);
    executionScope->incPC();
}

/**
 * MUL Instruction -> R[rd] = R[rs] * R[rt]
 *
 * @param executionScope Execution Scope
 * @param funcParams Parameters Values (rd, rs, rt)
 */
void MUL_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string rdValue = funcParams[0];
    std::string rsValue = executionScope->getRegisterValue(funcParams[1]);
    std::string rtValue = executionScope->getRegisterValue(funcParams[2]);
    std::string resultValue = mulBinary(rsValue, rtValue);

    executionScope->setRegisterValue(rdValue, resultValue);
    executionScope->incPC();
}

/**
 * DIV Instruction -> R[rd] = R[rs] / R[rt]
 *
 * @param executionScope Execution Scope
 * @param funcParams Parameters Values (rd, rs, rt)
 */
void DIV_function(ExecutionScope* executionScope, std::vector<std::string> funcParams) {
    std::string rdValue = funcParams[0];
    std::string rsValue = executionScope->getRegisterValue(funcParams[1]);
    std::string rtValue = executionScope->getRegisterValue(funcParams[2]);
    std::string resultValue = divBinary(rsValue, rtValue);

    executionScope->setRegisterValue(rdValue, resultValue);
    executionScope->incPC();
}

