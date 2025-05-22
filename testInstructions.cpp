// testInstructions.cpp
#include "Instructions.h"
#include "Memory.h"

int main() {
    using namespace Instructions;

    // Initialize RAM and Storage
    MemoryBuffer::createBuffer(ram, 32768);      // 2^15
    MemoryBuffer::createBuffer(storage, 32768);  // Or whatever size fits your tests

  

    std::string binary = "0011 0001 0010 0011"; // opcode 3 (add), 3 operands: 0001, 0010, 0011
    int pc = 0;

    Instruction* instr = createInstruction(binary);
    debugPrintout(instr);
    executeInstruction(instr, pc);

    destroyInstruction(instr);

    return 0;
}
