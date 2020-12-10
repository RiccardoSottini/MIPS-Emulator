#ifndef EXECUTION_INTERFACE_H_INCLUDED
#define EXECUTION_INTERFACE_H_INCLUDED

#include "execution_scope.h"

class ExecutionInterface {
    public:
        ExecutionInterface();
        void executeInterface();

    private:
        void clearScreen();

        ExecutionScope* executionScope;
        std::vector<std::string> instructions;
};

#endif // EXECUTION_INTERFACE_H_INCLUDED
