#ifndef DEF_H_INCLUDED
#define DEF_H_INCLUDED

#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <bitset>

enum Format {
    R_FORMAT,
    I_FORMAT,
    J_FORMAT
};

class MemoryStructure {
    public:
        MemoryStructure(Format format, std::string opcode, std::string funct, std::vector<std::string> fieldsOrder);

        std::string getFunct() const;
        std::string getOpcode() const;
        enum Format getFormat() const;
        std::vector<std::string> getParametersOrder() const;

    private:
        enum Format format;
        std::string opcode, funct;
        std::vector<std::string> parametersOrder;
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

const std::map<std::string, MemoryStructure> instructionFormats = {
    { "add",    MemoryStructure(R_FORMAT, "000000", "100000",   {"rd", "rs", "rt"})    },
    { "addi",   MemoryStructure(I_FORMAT, "001000", "",         {"rt", "rs", "imm"})   },
    { "addiu",  MemoryStructure(I_FORMAT, "001001", "",         {"rt", "rs", "imm"})   },
    { "addu",   MemoryStructure(R_FORMAT, "000000", "100001",   {"rd", "rs", "rt"})    },
    { "and",    MemoryStructure(R_FORMAT, "000000", "100100",   {"rd", "rs", "rt"})    },
    { "andi",   MemoryStructure(I_FORMAT, "001100", "",         {"rt", "rs", "imm"})   },

    { "beq",    MemoryStructure(I_FORMAT, "000100", "",         {"rs", "rt", "imm"})   },
    { "bne",    MemoryStructure(I_FORMAT, "000101", "",         {"rs", "rt", "imm"})   },

    { "j",      MemoryStructure(J_FORMAT, "000010", "",         {"addr"})              },
    { "jal",    MemoryStructure(J_FORMAT, "000011", "",         {"addr"})              },
    { "jr",     MemoryStructure(R_FORMAT, "000000", "001000",   {"rs"})                },

    { "lbu",    MemoryStructure(I_FORMAT, "100100", "",         {"rt", "imm", "rs"})   },
    { "lhu",    MemoryStructure(I_FORMAT, "100101", "",         {"rt", "imm", "rs"})   },
    { "ll",     MemoryStructure(I_FORMAT, "110000", "",         {"rt", "imm", "rs"})   },
    { "lui",    MemoryStructure(I_FORMAT, "001111", "",         {"rt", "imm"})         },
    { "lw",     MemoryStructure(I_FORMAT, "100011", "",         {"rt", "imm", "rs"})   },

    { "nor",    MemoryStructure(R_FORMAT, "000000", "100111",   {"rd", "rs", "rt"})    },
    { "or",     MemoryStructure(R_FORMAT, "000000", "100101",   {"rd", "rs", "rt"})    },
    { "ori",    MemoryStructure(I_FORMAT, "001101", "",         {"rt", "rs", "imm"})   },

    { "slt",    MemoryStructure(R_FORMAT, "000000", "101010",   {"rd", "rs", "rt"})    },
    { "slti",   MemoryStructure(I_FORMAT, "001010", "",         {"rt", "rs", "imm"})   },
    { "sltiu",  MemoryStructure(I_FORMAT, "001011", "",         {"rt", "rs", "imm"})   },
    { "sltu",   MemoryStructure(R_FORMAT, "000000", "101011",   {"rd", "rs", "rt"})    },

    { "sll",    MemoryStructure(R_FORMAT, "000000", "000000",   {"rd", "rt", "shamt"}) },
    { "srl",    MemoryStructure(R_FORMAT, "000000", "000010",   {"rd", "rt", "shamt"}) },

    { "sb",     MemoryStructure(I_FORMAT, "101000", "",         {"rt", "imm", "rs"})   },
    { "sc",     MemoryStructure(I_FORMAT, "111000", "",         {"rt", "imm", "rs"})   },
    { "sh",     MemoryStructure(I_FORMAT, "101001", "",         {"rt", "imm", "rs"})   },
    { "sw",     MemoryStructure(I_FORMAT, "101011", "",         {"rt", "imm", "rs"})   },

    { "sub",    MemoryStructure(R_FORMAT, "000000", "100010",   {"rd", "rs", "rt"})    },
    { "subu",   MemoryStructure(R_FORMAT, "000000", "100011",   {"rd", "rs", "rt"})    }
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

class Instruction {
    public:
        Instruction(std::string instruction);

        std::string getName();
        std::string getParameter(const int index);
        std::string getField(std::string field);
        std::string getBinary();

        void printMemory();
        void printField(std::string);
        void printFields(std::vector<std::string> fields);

    private:
        void calculateMemory();
        void parseInstruction(std::string instruction);
        std::string parseParameter(std::string parameterName, std::string parameterValue);

        std::string name;
        std::vector<std::string> parameters;

        std::map<std::string, std::string> memoryFields;
        const MemoryStructure *memoryStructure;
};

#endif
