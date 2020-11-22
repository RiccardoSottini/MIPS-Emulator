#include "def.h"

MemoryStructure::MemoryStructure(Format format, std::string opcode, std::string funct, std::vector<std::string> parametersOrder) {
    this->format = format;
    this->opcode = opcode;
    this->funct = funct;
    this->parametersOrder = parametersOrder;
}

std::string MemoryStructure::getOpcode() const {
    return this->opcode;
}

std::string MemoryStructure::getFunct() const {
    return this->funct;
}

enum Format MemoryStructure::getFormat() const {
    return this->format;
}

std::vector<std::string> MemoryStructure::getParametersOrder() const {
    return this->parametersOrder;
}
