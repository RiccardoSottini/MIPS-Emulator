#ifndef INSTRUCTION_H_INCLUDED
#define INSTRUCTION_H_INCLUDED

#include "def.h"

class ExecutionScope;
class MemoryStructure;

class Instruction {
    public:
        Instruction(std::string value, InputType inputType);
        Instruction(std::string value, InputType inputType, ExecutionScope* executionScope);

        std::string getName();
        std::string getField(std::string field);
        std::string getParameter(const int index);
        enum StatementType getStatementType();
        std::string getBinary();
        std::string getInstruction();

        void setField(std::string fieldName, std::string fieldValue);
        void setParameter(const int index, std::string parameterValue);

        void parseInstruction(std::string instruction);
        void parseBinary(std::string binary);

        void calculateBinary();
        void calculateInstruction();
        std::string calculateField(std::string parameterName, std::string parameterValue);
        std::string calculateParameter(std::string fieldName, std::string fieldValue);

        void executeInstruction();

    private:
        std::string name;
        std::vector<std::string> parameters;

        std::map<std::string, std::string> memoryFields;
        const MemoryStructure* memoryStructure;
        enum StatementType statementType;

        ExecutionScope* executionScope = nullptr;
};

#endif // INSTRUCTION_H_INCLUDED
