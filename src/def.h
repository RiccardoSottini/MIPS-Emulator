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

enum Format {
    R_FORMAT,
    I_FORMAT,
    J_FORMAT
};

enum StatementType {
    INSTRUCTION,
    LABEL
};

#include "memory_structure.h"
#include "instruction.h"
#include "execution_scope.h"

bool isNumber(std::string s);

int toDecimal(std::string binaryValue);
std::string toHex(std::string binaryValue);
std::string toHex(std::string binaryValue, const int hexSize);

std::string formatBinary(std::string binaryValue, const int binarySize);
std::string formatHex(std::string hexValue, const int hexSize);

std::string addBinary(std::string binaryA, std::string binaryB);
std::string andBinary(std::string binaryA, std::string binaryB);
std::string norBinary(std::string binaryA, std::string binaryB);
std::string orBinary(std::string binaryA, std::string binaryB);
std::string shiftLeftBinary(std::string binaryValue, std::string binaryShift);
std::string shiftRightBinary(std::string binaryValue, std::string binaryShift);
std::string subBinary(std::string binaryA, std::string binaryB);

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

const std::map<std::string, MemoryStructure> instructionFormats = {
    { "add",    MemoryStructure(R_FORMAT, "000000", "100000",   {"rd", "rs", "rt"},     ADD_function)   },
    { "addi",   MemoryStructure(I_FORMAT, "001000", "",         {"rt", "rs", "imm"},    ADDI_function)  },
    { "addiu",  MemoryStructure(I_FORMAT, "001001", "",         {"rt", "rs", "imm"},    ADDIU_function) },
    { "addu",   MemoryStructure(R_FORMAT, "000000", "100001",   {"rd", "rs", "rt"},     ADDU_function)  },

    { "and",    MemoryStructure(R_FORMAT, "000000", "100100",   {"rd", "rs", "rt"},     AND_function)   },
    { "andi",   MemoryStructure(I_FORMAT, "001100", "",         {"rt", "rs", "imm"},    ANDI_function)  },

    { "beq",    MemoryStructure(I_FORMAT, "000100", "",         {"rs", "rt", "imm"},    BEQ_function)   },
    { "bne",    MemoryStructure(I_FORMAT, "000101", "",         {"rs", "rt", "imm"},    BNE_function)   },

    { "j",      MemoryStructure(J_FORMAT, "000010", "",         {"addr"},               J_function)     },
    { "jal",    MemoryStructure(J_FORMAT, "000011", "",         {"addr"},               JAL_function)   },
    { "jr",     MemoryStructure(R_FORMAT, "000000", "001000",   {"rs"},                 JR_function)    },

    { "lbu",    MemoryStructure(I_FORMAT, "100100", "",         {"rt", "imm", "rs"},    LBU_function)   },
    { "lhu",    MemoryStructure(I_FORMAT, "100101", "",         {"rt", "imm", "rs"},    LHU_function)   },
    { "lui",    MemoryStructure(I_FORMAT, "001111", "",         {"rt", "imm"},          LUI_function)   },
    { "lw",     MemoryStructure(I_FORMAT, "100011", "",         {"rt", "imm", "rs"},    LW_function)    },

    { "nor",    MemoryStructure(R_FORMAT, "000000", "100111",   {"rd", "rs", "rt"},     NOR_function)   },
    { "or",     MemoryStructure(R_FORMAT, "000000", "100101",   {"rd", "rs", "rt"},     OR_function)    },
    { "ori",    MemoryStructure(I_FORMAT, "001101", "",         {"rt", "rs", "imm"},    ORI_function)   },

    { "slt",    MemoryStructure(R_FORMAT, "000000", "101010",   {"rd", "rs", "rt"},     SLT_function)   },
    { "slti",   MemoryStructure(I_FORMAT, "001010", "",         {"rt", "rs", "imm"},    SLTI_function)  },
    { "sltiu",  MemoryStructure(I_FORMAT, "001011", "",         {"rt", "rs", "imm"},    SLTIU_function) },
    { "sltu",   MemoryStructure(R_FORMAT, "000000", "101011",   {"rd", "rs", "rt"},     SLTU_function)  },

    { "sll",    MemoryStructure(R_FORMAT, "000000", "000000",   {"rd", "rt", "shamt"},  SLL_function)   },
    { "srl",    MemoryStructure(R_FORMAT, "000000", "000010",   {"rd", "rt", "shamt"},  SRL_function)   },

    { "sb",     MemoryStructure(I_FORMAT, "101000", "",         {"rt", "imm", "rs"},    SB_function)    },
    { "sh",     MemoryStructure(I_FORMAT, "101001", "",         {"rt", "imm", "rs"},    SH_function)    },
    { "sw",     MemoryStructure(I_FORMAT, "101011", "",         {"rt", "imm", "rs"},    SW_function)    },

    { "sub",    MemoryStructure(R_FORMAT, "000000", "100010",   {"rd", "rs", "rt"},     SUB_function)   },
    { "subu",   MemoryStructure(R_FORMAT, "000000", "100011",   {"rd", "rs", "rt"},     SUBU_function)  }
};

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

#include "memory_structure.h"
#include "instruction.h"
#include "execution_scope.h"

#endif
