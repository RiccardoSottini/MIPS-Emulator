#ifndef MEMORY_STRUCTURE_H_INCLUDED
#define MEMORY_STRUCTURE_H_INCLUDED

#include "def.h"

class ExecutionScope;

class MemoryStructure {
    public:
        MemoryStructure(Format format, std::string opcode, std::string funct, std::vector<std::string> fieldsOrder, std::function<void(ExecutionScope*, std::vector<std::string>)> instructionFunction);

        std::string getFunct() const;
        std::string getOpcode() const;
        enum Format getFormat() const;
        std::vector<std::string> getParametersOrder() const;

        void executeFunction(ExecutionScope* executionScope, std::vector<std::string> funcParams) const;

    private:
        enum Format format;
        std::string opcode, funct;
        std::vector<std::string> parametersOrder;
        std::function<void(ExecutionScope*, std::vector<std::string>)> instructionFunction;
};

#endif // MEMORY_STRUCTURE_H_INCLUDED
