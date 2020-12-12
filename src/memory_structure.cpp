#include "../includes/mips_emulator.h"

/**
 * MemoryStructure Constructor - Initializes the Memory Structure used to store an Instruction with its parameters order
 *
 * @param format Instruction Format (R-Format / I-Format / J-Format)
 * @param instructionPurpose Purpose of the Instruction
 * @param opcode Opcode of the Instruction
 * @param funct Funct Code of the Instruction
 * @param parametersOrder Order in which the Parameters are placed into the Instruction
 * @param instructionFunction Pointer to the Instruction's Function to be executed
 */
MemoryStructure::MemoryStructure(Format format, InstructionPurpose instructionPurpose, std::string opcode, std::string funct, std::vector<std::string> parametersOrder, std::function<void(ExecutionScope*, std::vector<std::string>)> instructionFunction) {
    this->format = format;
    this->opcode = opcode;
    this->funct = funct;
    this->parametersOrder = parametersOrder;
    this->instructionPurpose = instructionPurpose;
    this->instructionFunction = instructionFunction;
}

/**
 * Get the Format of the Instruction
 *
 * @return Format of the Instruction (R-Format / I-Format / J-Format)
 */
enum Format MemoryStructure::getFormat() const {
    return this->format;
}

/**
 * Get the Purpose of the Instruction
 *
 * @return Purpose of the Instruction (Maths / Logic / Address / Move)
 */
enum InstructionPurpose MemoryStructure::getInstructionPurpose() const {
    return this->instructionPurpose;
};

/**
 * Get the Opcode of the Instruction
 *
 * @return Opcode of the Instruction
 */
std::string MemoryStructure::getOpcode() const {
    return this->opcode;
}

/**
 * Get the Funct Code of the Instruction
 *
 * @return Funct Code of the Instruction
 */
std::string MemoryStructure::getFunct() const {
    return this->funct;
}

/**
 * Get the Parameters Order of the Instruction
 *
 * @return Parameters Order of the Instruction (Order in which the Parameters are placed into the Instruction)
 */
std::vector<std::string> MemoryStructure::getParametersOrder() const {
    return this->parametersOrder;
}

/**
 * Execute the Function of the Instruction
 *
 * @param executionScope Execution Scope
 * @param funcParams Parameters Values
 */
void MemoryStructure::executeFunction(ExecutionScope* executionScope, std::vector<std::string> funcParams) const {
    this->instructionFunction(executionScope, funcParams);
}
