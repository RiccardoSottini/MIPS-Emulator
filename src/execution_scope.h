#ifndef EXECUTION_SCOPE_H_INCLUDED
#define EXECUTION_SCOPE_H_INCLUDED

#include "instruction.h"

class ExecutionScope {
    public:
        ExecutionScope(std::vector<std::string> instructions);

        void executeScope();

        void printInstructions(enum InputType inputType);
        void printRegisters();
        void printMemory();

        void loadValue(std::string binaryValue);
        void loadValue(const int decimalValue);
        void loadArray(std::vector<std::string> arrayValue);
        void loadArray(std::vector<int> arrayValue);

        void setByteValue(std::string byteAddress, std::string byteValue);
        void setByteValue(std::string byteAddress, std::string byteOffset, std::string byteValue);
        void setWordValue(std::string wordAddress, std::string wordValue);
        void setWordValue(std::string wordAddress, std::string byteOffset, std::string wordValue);
        void setRegisterValue(std::string regPosition, std::string value);
        void setLabelAddress(std::string label, std::string insAddress);
        void setPC(std::string newPC);
        void setPC(std::string addressingValue, enum AddressingType addressingType);

        std::string getByteValue(std::string byteAddress);
        std::string getByteValue(std::string byteAddress, std::string byteOffset);
        std::string getWordValue(std::string wordAddress);
        std::string getWordValue(std::string wordAddress, std::string byteOffset);
        std::string getRegisterValue(std::string regPosition);
        std::string getLabelAddress(std::string label);
        std::string getLabelName(std::string address);
        std::string getPC();

        void incPC();
        bool isFinished();

    private:
        std::string PC;
        //std::string memPosition;

        std::map<std::string, std::string> memoryLocations;     //32-bit String (address)   -> 8-bit  String  (byte)
        std::map<std::string, std::string> listRegisters;       //5-bit  String (register)  -> 32-bit String (word)
        std::map<std::string, std::string> listLabels;          //Label  String             -> 32-bit String (word)
        std::map<std::string, std::string> pointerLabels;       //32-bit String (word)      -> Label  String

        /*std::map<std::string, std::string> listRegisters;       //5-bit String  -> 32-bit String
        std::map<std::string, std::string> memoryLocations;     //32-bit String -> 8-bit String
        std::map<std::string, Instruction*> listInstructions;   //32-bit String -> Instruction Pointer
        std::map<std::string, std::string> listLabels;*/          //Label Name    -> 32-bit String
};

#endif
