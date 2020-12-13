project(mips_emulator)
cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 14)

add_executable(mips_console src/main.cpp)
add_library(mips_emulator STATIC    src/execution_interface.cpp
                                    src/execution_scope.cpp
                                    src/functions.cpp
                                    src/instruction.cpp
                                    src/memory_structure.cpp)

include_directories(includes)

target_link_libraries(mips_console LINK_PUBLIC mips_emulator)
