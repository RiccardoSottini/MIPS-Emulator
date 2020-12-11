#include <iostream>
#include <vector>

#include "src/mips_emulator.h"

int main() {
    ExecutionInterface* interface = new ExecutionInterface();
    interface->executeInterface();

    return 0;
}
