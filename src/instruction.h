#ifndef INSTRUCTION_H_INCLUDED
#define INSTRUCTION_H_INCLUDED

#include "def.h"

class ExecutionScope;
class MemoryStructure;

class Instruction {
    public:
        Instruction(std::string instruction);
        Instruction(std::string instruction, ExecutionScope* executionScope);

        std::string getName();
        std::string getParameter(const int index);
        std::string getField(std::string field);
        std::string getBinary();
        enum StatementType getStatementType();

        void printMemory();
        void printField(std::string);
        void printFields(std::vector<std::string> fields);

        void calculateMemory();
        std::string calculateField(std::string parameterName, std::string parameterValue);

        void executeInstruction();

    private:
        void parseInstruction(std::string instruction);

        std::string name;
        std::vector<std::string> parameters;

        std::map<std::string, std::string> memoryFields;
        const MemoryStructure* memoryStructure;
        enum StatementType statementType;

        ExecutionScope* executionScope = nullptr;
};

#endif // INSTRUCTION_H_INCLUDED
