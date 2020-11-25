#ifndef EXECUTION_SCOPE_H_INCLUDED
#define EXECUTION_SCOPE_H_INCLUDED

#include "instruction.h"

class ExecutionScope {
    public:
        ExecutionScope(std::vector<std::string> instructions);

        void executeScope();

        void printRegisters();
        void printMemory();

        void loadArray(std::vector<std::string> arrayValue);
        void loadArray(std::vector<int> arrayValue);

        void setByteValue(std::string byteAddress, std::string byteValue);
        void setByteValue(std::string byteAddress, std::string byteOffset, std::string byteValue);
        void setWordValue(std::string wordAddress, std::string wordValue);
        void setWordValue(std::string wordAddress, std::string byteOffset, std::string wordValue);
        void setRegisterValue(std::string regPosition, std::string value);
        void setInstruction(std::string insAddress, std::string instruction);
        void setLabelAddress(std::string label, std::string insAddress);
        void setPC(std::string newPC);
        void setMemoryBound(std::string memAddress);

        std::string getByteValue(std::string byteAddress);
        std::string getByteValue(std::string byteAddress, std::string byteOffset);
        std::string getWordValue(std::string wordAddress);
        std::string getWordValue(std::string wordAddress, std::string byteOffset);
        std::string getRegisterValue(std::string regPosition);
        Instruction* getInstruction(std::string insAddress);
        std::string getLabelAddress(std::string label);
        std::string getPC();

        void incPC();
        void jumpLabel(std::string label);

        bool isFinished();

    private:
        std::string PC;
        std::string memPosition;

        std::map<std::string, std::string> listRegisters;       //5-bit String  -> 32-bit String
        std::map<std::string, std::string> memoryLocations;     //32-bit String -> 8-bit String
        std::map<std::string, Instruction*> listInstructions;   //32-bit String -> Instruction Pointer
        std::map<std::string, std::string> listLabels;          //Label Name    -> 32-bit String
};

#endif
