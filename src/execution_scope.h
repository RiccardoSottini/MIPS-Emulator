#ifndef EXECUTION_SCOPE_H_INCLUDED
#define EXECUTION_SCOPE_H_INCLUDED

#include "instruction.h"

class ExecutionScope {
    public:
        ExecutionScope(std::vector<std::string> instructions);

        void executeScope();
        void printRegisters();

        void setRegisterValue(std::string regPosition, std::string value);
        void setInstruction(std::string address, std::string instruction);
        void setLabelAddress(std::string label, std::string address);

        std::string getRegisterValue(std::string regPosition);
        Instruction* getInstruction(std::string address);
        std::string getLabelAddress(std::string label);

        void incPC();
        void setPC(std::string newPC);
        void jumpLabel(std::string label);

        bool isFinished();

    private:
        std::string PC;
        std::map<std::string, std::string> memoryRegisters;
        std::map<std::string, Instruction*> memoryInstructions;
        std::map<std::string, std::string> memoryLabels;
};

#endif
