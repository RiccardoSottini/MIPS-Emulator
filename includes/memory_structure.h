#ifndef MEMORY_STRUCTURE_H_INCLUDED
#define MEMORY_STRUCTURE_H_INCLUDED

#include "mips_emulator.h"

class ExecutionScope;

class MemoryStructure {
    public:
        MemoryStructure(Format format, InstructionPurpose instructionPurpose, std::string opcode, std::string funct, std::vector<std::string> fieldsOrder, std::function<void(ExecutionScope*, std::vector<std::string>)> instructionFunction);

        enum Format getFormat() const;
        enum InstructionPurpose getInstructionPurpose() const;
        std::string getOpcode() const;
        std::string getFunct() const;
        std::vector<std::string> getParametersOrder() const;

        void executeFunction(ExecutionScope* executionScope, std::vector<std::string> funcParams) const;

    private:
        enum Format format;
        enum InstructionPurpose instructionPurpose;
        std::string opcode, funct;
        std::vector<std::string> parametersOrder;
        std::function<void(ExecutionScope*, std::vector<std::string>)> instructionFunction;
};

#endif // MEMORY_STRUCTURE_H_INCLUDED
