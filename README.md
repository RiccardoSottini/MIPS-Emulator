# MIPS Emulator
Simple MIPS Instruction Set Emulator.\
A CLI (Command Line Interface) is provided to use the MIPS Emulator.

**Instruction Class:**
Parse an Instruction and translate it in its Binary Format.\
The Instruction class also holds informations used by the Execution Scope to execute the Instruction's operations.

**MemoryStructure Class:**
Informations about the Instruction Format.\
Provides the list of Binary Fields required by the Instruction and provides their order.\
It also provides the pointer to the function defined to execute the Instruction's operations.

**ExecutionScope Class:**
Provides the state of the Execution - Register Values, Memory Locations and Program Counter.\
The class also manages the Execution of the Instructions, handling the data required and by calling the Instruction's functions.\
Implements functions to print the Execution State.

**ExecutionInterface Class:**
Implements the Command Line Interface to use the MIPS Emulator through the ExecutionScope class.
