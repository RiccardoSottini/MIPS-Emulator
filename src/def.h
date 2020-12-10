#ifndef DEF_H_INCLUDED
#define DEF_H_INCLUDED

#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <bitset>
#include <functional>
#include <algorithm>

/**
 * Data Format
 */
enum DataFormat {
    DEC_FORMAT,     ///< Decimal Format
    BIN_FORMAT,     ///< Binary Format
    HEX_FORMAT      ///< Hexadecimal Format
};

/**
 * Type of Input used to construct the Instruction
 */
enum InputType {
    INSTRUCTION_VALUE,  ///< Instruction Value
    BINARY_VALUE        ///< Binary Value
};

/**
 * Instruction's Format
 */
enum Format {
    R_FORMAT,   ///< Instruction R-Format (opcode - rs - rt - rd - shamt - funct)
    I_FORMAT,   ///< Instruction I-Format (opcode - rs - rt - immediate)
    J_FORMAT    ///< Instruction J-Format (opcode - address)
};

/**
 * Statement's Type
 */
enum StatementType {
    INSTRUCTION,    ///< Definition of an Instruction
    LABEL           ///< Definition of a Label
};

/**
 * Addressing Type
 */
enum AddressingType {
    IMMEDIATE_ADDRESSING,       ///< Immediate Addressing
    REGISTER_ADDRESSING,        ///< Register Addressing
    BASE_ADDRESSING,            ///< Base Addressing
    PC_RELATIVE_ADDRESSING,     ///< PC-Relative Addressing
    PSEUDO_DIRECT_ADDRESSING    ///< Pseudo-Direct Addressing
};

/**
 * Self-Explanatory Instruction Purpose
 */
enum InstructionPurpose {
    INSTRUCTION_MATHS,          ///< Instruction that does Maths operations
    INSTRUCTION_LOGIC,          ///< Instruction that does Logic operations
    INSTRUCTION_ADDRESS,        ///< Instruction that does Addressing operations
    INSTRUCTION_MOVE            ///< Instruction that moves values around Registers and Memory Locations
};

#include "memory_structure.h"
#include "instruction.h"
#include "execution_scope.h"
#include "execution_interface.h"

bool isNumber(std::string s);
bool isHex(std::string hexValue);

int toDecimal(std::string binaryValue);
int toDecimal(std::string inputValue, enum DataFormat inputFormat);
std::string toBinary(const int decimalValue);
std::string toHex(std::string binaryValue);
std::string toHex(std::string binaryValue, const int hexSize);

std::string toTwoComplement(const int decimalValue, const int binarySize);
int fromTwoComplement(std::string binaryValue);

std::string formatBinary(std::string binaryValue, const int binarySize);
std::string formatHex(std::string hexValue, const int hexSize);

std::string invertBinary(std::string binaryValue);
std::string addBinary(std::string binaryA, std::string binaryB);
std::string andBinary(std::string binaryA, std::string binaryB);
std::string norBinary(std::string binaryA, std::string binaryB);
std::string orBinary(std::string binaryA, std::string binaryB);
std::string shiftLeftBinary(std::string binaryValue, std::string binaryShift);
std::string shiftRightBinary(std::string binaryValue, std::string binaryShift);
std::string subBinary(std::string binaryA, std::string binaryB);

std::string SignExtImm(std::string immediate);
std::string ZeroExtImm(std::string immediate);
std::string BranchAddr(std::string immediate);
std::string JumpAddr(std::string PC, std::string address);

void ADD_function(ExecutionScope* executionScope, std::vector<std::string> funcParams);
void ADDI_function(ExecutionScope* executionScope, std::vector<std::string> funcParams);
void ADDIU_function(ExecutionScope* executionScope, std::vector<std::string> funcParams);
void ADDU_function(ExecutionScope* executionScope, std::vector<std::string> funcParams);

void AND_function(ExecutionScope* executionScope, std::vector<std::string> funcParams);
void ANDI_function(ExecutionScope* executionScope, std::vector<std::string> funcParams);

void BEQ_function(ExecutionScope* executionScope, std::vector<std::string> funcParams);
void BNE_function(ExecutionScope* executionScope, std::vector<std::string> funcParams);

void J_function(ExecutionScope* executionScope, std::vector<std::string> funcParams);
void JAL_function(ExecutionScope* executionScope, std::vector<std::string> funcParams);
void JR_function(ExecutionScope* executionScope, std::vector<std::string> funcParams);

void LBU_function(ExecutionScope* executionScope, std::vector<std::string> funcParams);
void LHU_function(ExecutionScope* executionScope, std::vector<std::string> funcParams);
void LUI_function(ExecutionScope* executionScope, std::vector<std::string> funcParams);
void LW_function(ExecutionScope* executionScope, std::vector<std::string> funcParams);

void NOR_function(ExecutionScope* executionScope, std::vector<std::string> funcParams);
void OR_function(ExecutionScope* executionScope, std::vector<std::string> funcParams);
void ORI_function(ExecutionScope* executionScope, std::vector<std::string> funcParams);

void SLT_function(ExecutionScope* executionScope, std::vector<std::string> funcParams);
void SLTI_function(ExecutionScope* executionScope, std::vector<std::string> funcParams);
void SLTIU_function(ExecutionScope* executionScope, std::vector<std::string> funcParams);
void SLTU_function(ExecutionScope* executionScope, std::vector<std::string> funcParams);

void SLL_function(ExecutionScope* executionScope, std::vector<std::string> funcParams);
void SRL_function(ExecutionScope* executionScope, std::vector<std::string> funcParams);

void SB_function(ExecutionScope* executionScope, std::vector<std::string> funcParams);
void SH_function(ExecutionScope* executionScope, std::vector<std::string> funcParams);
void SW_function(ExecutionScope* executionScope, std::vector<std::string> funcParams);

void SUB_function(ExecutionScope* executionScope, std::vector<std::string> funcParams);
void SUBU_function(ExecutionScope* executionScope, std::vector<std::string> funcParams);

void MUL_function(ExecutionScope* executionScope, std::vector<std::string> funcParams);
void DIV_function(ExecutionScope* executionScope, std::vector<std::string> funcParams);

/**
 * Match the Instruction's Name with the Memory Structure (that contains structural informations about the Instruction)
 *
 * @hideinitializer
 */
const std::map<std::string, MemoryStructure> instructionFormats = {
    { "add",    MemoryStructure(R_FORMAT, INSTRUCTION_MATHS,    "000000", "100000",   {"rd", "rs", "rt"},     ADD_function)   },
    { "addi",   MemoryStructure(I_FORMAT, INSTRUCTION_MATHS,    "001000", "",         {"rt", "rs", "imm"},    ADDI_function)  },
    { "addiu",  MemoryStructure(I_FORMAT, INSTRUCTION_MATHS,    "001001", "",         {"rt", "rs", "imm"},    ADDIU_function) },
    { "addu",   MemoryStructure(R_FORMAT, INSTRUCTION_MATHS,    "000000", "100001",   {"rd", "rs", "rt"},     ADDU_function)  },
    { "and",    MemoryStructure(R_FORMAT, INSTRUCTION_LOGIC,    "000000", "100100",   {"rd", "rs", "rt"},     AND_function)   },
    { "andi",   MemoryStructure(I_FORMAT, INSTRUCTION_LOGIC,    "001100", "",         {"rt", "rs", "imm"},    ANDI_function)  },
    { "beq",    MemoryStructure(I_FORMAT, INSTRUCTION_ADDRESS,  "000100", "",         {"rs", "rt", "imm"},    BEQ_function)   },
    { "bne",    MemoryStructure(I_FORMAT, INSTRUCTION_ADDRESS,  "000101", "",         {"rs", "rt", "imm"},    BNE_function)   },
    { "j",      MemoryStructure(J_FORMAT, INSTRUCTION_ADDRESS,  "000010", "",         {"addr"},               J_function)     },
    { "jal",    MemoryStructure(J_FORMAT, INSTRUCTION_ADDRESS,  "000011", "",         {"addr"},               JAL_function)   },
    { "jr",     MemoryStructure(R_FORMAT, INSTRUCTION_ADDRESS,  "000000", "001000",   {"rs"},                 JR_function)    },
    { "lbu",    MemoryStructure(I_FORMAT, INSTRUCTION_MOVE,     "100100", "",         {"rt", "imm", "rs"},    LBU_function)   },
    { "lhu",    MemoryStructure(I_FORMAT, INSTRUCTION_MOVE,     "100101", "",         {"rt", "imm", "rs"},    LHU_function)   },
    { "lui",    MemoryStructure(I_FORMAT, INSTRUCTION_MOVE,     "001111", "",         {"rt", "imm"},          LUI_function)   },
    { "lw",     MemoryStructure(I_FORMAT, INSTRUCTION_MOVE,     "100011", "",         {"rt", "imm", "rs"},    LW_function)    },
    { "nor",    MemoryStructure(R_FORMAT, INSTRUCTION_LOGIC,    "000000", "100111",   {"rd", "rs", "rt"},     NOR_function)   },
    { "or",     MemoryStructure(R_FORMAT, INSTRUCTION_LOGIC,    "000000", "100101",   {"rd", "rs", "rt"},     OR_function)    },
    { "ori",    MemoryStructure(I_FORMAT, INSTRUCTION_LOGIC,    "001101", "",         {"rt", "rs", "imm"},    ORI_function)   },
    { "slt",    MemoryStructure(R_FORMAT, INSTRUCTION_LOGIC,    "000000", "101010",   {"rd", "rs", "rt"},     SLT_function)   },
    { "slti",   MemoryStructure(I_FORMAT, INSTRUCTION_LOGIC,    "001010", "",         {"rt", "rs", "imm"},    SLTI_function)  },
    { "sltiu",  MemoryStructure(I_FORMAT, INSTRUCTION_LOGIC,    "001011", "",         {"rt", "rs", "imm"},    SLTIU_function) },
    { "sltu",   MemoryStructure(R_FORMAT, INSTRUCTION_LOGIC,    "000000", "101011",   {"rd", "rs", "rt"},     SLTU_function)  },
    { "sll",    MemoryStructure(R_FORMAT, INSTRUCTION_LOGIC,    "000000", "000000",   {"rd", "rt", "shamt"},  SLL_function)   },
    { "srl",    MemoryStructure(R_FORMAT, INSTRUCTION_LOGIC,    "000000", "000010",   {"rd", "rt", "shamt"},  SRL_function)   },
    { "sb",     MemoryStructure(I_FORMAT, INSTRUCTION_MOVE,     "101000", "",         {"rt", "imm", "rs"},    SB_function)    },
    { "sh",     MemoryStructure(I_FORMAT, INSTRUCTION_MOVE,     "101001", "",         {"rt", "imm", "rs"},    SH_function)    },
    { "sw",     MemoryStructure(I_FORMAT, INSTRUCTION_MOVE,     "101011", "",         {"rt", "imm", "rs"},    SW_function)    },
    { "sub",    MemoryStructure(R_FORMAT, INSTRUCTION_MATHS,    "000000", "100010",   {"rd", "rs", "rt"},     SUB_function)   },
    { "subu",   MemoryStructure(R_FORMAT, INSTRUCTION_MATHS,    "000000", "100011",   {"rd", "rs", "rt"},     SUBU_function)  },
    { "mul",    MemoryStructure(R_FORMAT, INSTRUCTION_MATHS,    "000000", "010010",   {"rd", "rs", "rt"},     MUL_function)   },
    { "div",    MemoryStructure(R_FORMAT, INSTRUCTION_MATHS,    "000000", "011010",   {"rd", "rs", "rt"},     DIV_function)   }
};

/**
 * Match the Opcode / Funct with the Name of the Instruction
 *
 * @hideinitializer
 */
const std::map<std::pair<std::string, std::string>, std::string> instructionPointers {
    { { "000000", "100000" },   "add"   },
    { { "001000", ""       },   "addi"  },
    { { "001001", ""       },   "addiu" },
    { { "000000", "100001" },   "addu"  },
    { { "000000", "100100" },   "and"   },
    { { "001100", ""       },   "andi"  },
    { { "000100", ""       },   "beq"   },
    { { "000101", ""       },   "bne"   },
    { { "000010", ""       },   "j"     },
    { { "000011", ""       },   "jal"   },
    { { "000000", "001000" },   "jr"    },
    { { "100100", ""       },   "lbu"   },
    { { "100101", ""       },   "lhu"   },
    { { "001111", ""       },   "lui"   },
    { { "100011", ""       },   "lw"    },
    { { "000000", "100111" },   "nor"   },
    { { "000000", "100101" },   "or"    },
    { { "001101", ""       },   "ori"   },
    { { "000000", "101010" },   "slt"   },
    { { "001010", ""       },   "slti"  },
    { { "001011", ""       },   "sltiu" },
    { { "000000", "101011" },   "sltu"  },
    { { "000000", "000000" },   "sll"   },
    { { "000000", "000010" },   "srl"   },
    { { "101000", ""       },   "sb"    },
    { { "101001", ""       },   "sh"    },
    { { "101011", ""       },   "sw"    },
    { { "000000", "100010" },   "sub"   },
    { { "000000", "100011" },   "subu"  },
    { { "000000", "010010" },   "mul"   },
    { { "000000", "011010" },   "div"   }
};

/**
 * Size of the Instruction's Fields
 *
 * @hideinitializer
 */
const std::map<std::string, int> fieldSizes = {
    { "opcode", 6 },
    { "rs",     5 },
    { "rt",     5 },
    { "rd",     5 },
    { "shamt",  5 },
    { "funct",  6 },
    { "imm",    16 },
    { "addr",   26 },
};

/**
 * Register's binary value
 *
 * @hideinitializer
 */
const std::map<std::string, std::string> registerFormats = {
    { "$zero",  "00000" },
    { "$at",    "00001" },
    { "$v0",    "00010" },
    { "$v1",    "00011" },
    { "$a0",    "00100" },
    { "$a1",    "00101" },
    { "$a2",    "00110" },
    { "$a3",    "00111" },
    { "$t0",    "01000" },
    { "$t1",    "01001" },
    { "$t2",    "01010" },
    { "$t3",    "01011" },
    { "$t4",    "01100" },
    { "$t5",    "01101" },
    { "$t6",    "01110" },
    { "$t7",    "01111" },
    { "$s0",    "10000" },
    { "$s1",    "10001" },
    { "$s2",    "10010" },
    { "$s3",    "10011" },
    { "$s4",    "10100" },
    { "$s5",    "10101" },
    { "$s6",    "10110" },
    { "$s7",    "10111" },
    { "$t8",    "11000" },
    { "$t9",    "11001" },
    { "$k0",    "11010" },
    { "$k1",    "11011" },
    { "$gp",    "11100" },
    { "$sp",    "11101" },
    { "$fp",    "11110" },
    { "$ra",    "11111" },
};

/**
 * Register's text value
 *
 * @hideinitializer
 */
const std::vector<std::string> registerPointers {
    "$zero", "$at",
    "$v0", "$v1",
    "$a0", "$a1", "$a2", "$a3",
    "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7",
    "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7",
    "$t8", "$t9",
    "$k0", "$k1",
    "$gp", "$sp", "$fp", "$ra"
};

/**
 * Initial value of the Program Counter
 */
const std::string startPC = formatBinary("10000000000000000000000", 32);


/**
 * Initial value of the Global Area Pointer Register
 */
const std::string startGP = formatBinary("10000000000001000000000000000", 32);

/**
 * Initial value of the Stack Pointer Register
 */
const std::string startSP = formatBinary("1111111111111111111111111111100", 32);

#endif // DEF_H_INCLUDED
