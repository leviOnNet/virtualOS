#include "Instructions.h"
#include "Memory.h"
#include <algorithm> // for std::remove

namespace Instructions {

    bool halt = false;

    int binaryToInt(std::string binaryString) {
    int result = 0;
    for (size_t i = 0; i < binaryString.length(); ++i) {
        result <<= 1;
        if (binaryString[i] == '1') {
            result |= 1;
        } else if (binaryString[i] != '0') {
            std::cerr << "Error: invalid character in binary string: " << binaryString[i] << std::endl;
            return -1;  // Or handle error
        }
    }
    return result;
}

    int determineNumberOfOperands(int opcode) {
        switch (opcode) {
            case 0: return 0; // exit
            case 1: return 2; // load
            case 2: return 2; // save
            case 3: case 4: case 5: case 6: case 7: return 3; // add, minus, greater, less, equal
            case 8: return 3; // if
            case 9: return 1; // input
            case 10: return 1; // output
            case 11: return 1; // goto
            case 12: return 0; // persist
            case 13: return 0; // reload
            case 14: return 2; // const
            case 15: return 2; // move
            default: return 0;
        }
    }

    Instruction* createInstruction(std::string strInstruction) {
    strInstruction.erase(std::remove(strInstruction.begin(), strInstruction.end(), ' '), strInstruction.end());

    Instruction* instr = new Instruction();

    if (strInstruction.length() < 4) {
        std::cerr << "Error: instruction string too short to even read opcode: length = " << strInstruction.length() << std::endl;
        delete instr;
        return NULL;
    }

    instr->opcode = binaryToInt(strInstruction.substr(0, 4));
    int numOperands = determineNumberOfOperands(instr->opcode);

    int requiredLength = 4 + numOperands * 4;
    if ((int)strInstruction.length() < requiredLength) {
        std::cerr << "Error: instruction string too short for opcode " << instr->opcode
                  << ". Length: " << strInstruction.length() << ", expected: " << requiredLength << std::endl;
        delete instr;
        return NULL;
    }

    instr->operands = new int[numOperands];
    // Initialize operands to zero to avoid garbage
    for (int i = 0; i < numOperands; ++i) {
        instr->operands[i] = 0;
    }

    for (int i = 0; i < numOperands; ++i) {
        int start = 4 + (i * 4);
        std::string operandBits = strInstruction.substr(start, 4);

        // Validate operand bits
        for (size_t j = 0; j < operandBits.length(); ++j) {
            if (operandBits[j] != '0' && operandBits[j] != '1') {
                std::cerr << "Error: invalid character '" << operandBits[j] << "' in operand bits: " << operandBits << std::endl;
                delete[] instr->operands;
                delete instr;
                return NULL;
            }
        }

        instr->operands[i] = binaryToInt(operandBits);
        std::cout << "Parsed operand " << i << ": bits = " << operandBits << ", value = " << instr->operands[i] << std::endl;
    }

    return instr;
}


    void destroyInstruction(Instruction*& instruction) {
        if (instruction) {
            delete[] instruction->operands;
            delete instruction;
            instruction = NULL;
        }
    }

    void executeInstruction(Instruction* instruction, int& currentInstructionNumber) {
        switch (instruction->opcode) {
            case 0: exitOp(); break;
            case 1: loadOp(instruction->operands[0], instruction->operands[1]); break;
            case 2: saveOp(instruction->operands[0], instruction->operands[1]); break;
            case 3: addOp(instruction->operands[0], instruction->operands[1], instruction->operands[2]); break;
            case 4: minusOp(instruction->operands[0], instruction->operands[1], instruction->operands[2]); break;
            case 5: greaterOp(instruction->operands[0], instruction->operands[1], instruction->operands[2]); break;
            case 6: lessOp(instruction->operands[0], instruction->operands[1], instruction->operands[2]); break;
            case 7: equalOp(instruction->operands[0], instruction->operands[1], instruction->operands[2]); break;
            case 8: ifOp(instruction->operands[0], instruction->operands[1], instruction->operands[2], currentInstructionNumber); return;
            case 9: inputOp(instruction->operands[0]); break;
            case 10: outputOp(instruction->operands[0]); break;
            case 11: gotoOp(instruction->operands[0], currentInstructionNumber); return;
            case 12: persistOp(); break;
            case 13: reloadOp(); break;
            case 14: constOp(instruction->operands[0], instruction->operands[1]); break;
            case 15: moveOp(instruction->operands[0], instruction->operands[1]); break;
            default: std::cout << "Invalid opcode.\n"; break;
        }
        ++currentInstructionNumber;
    }

    // --- Individual Opcode Functions ---

    void exitOp() {
        halt = true;
    }

    void loadOp(int memoryLocation, int RAMLocation) {
        int value = MemoryBuffer::read(storage, memoryLocation);
        MemoryBuffer::store(ram, RAMLocation, value);
    }

    void saveOp(int RAMLocation, int memoryLocation) {
        int value = MemoryBuffer::read(ram, RAMLocation);
        MemoryBuffer::store(storage, memoryLocation, value);
    }

    void addOp(int resultLocation, int inputALocation, int inputBLocation) {
        int a = MemoryBuffer::read(ram, inputALocation);
        int b = MemoryBuffer::read(ram, inputBLocation);
        MemoryBuffer::store(ram, resultLocation, a + b);
    }

    void minusOp(int resultLocation, int inputALocation, int inputBLocation) {
        int a = MemoryBuffer::read(ram, inputALocation);
        int b = MemoryBuffer::read(ram, inputBLocation);
        MemoryBuffer::store(ram, resultLocation, a - b);
    }

    void greaterOp(int resultLocation, int inputALocation, int inputBLocation) {
        int a = MemoryBuffer::read(ram, inputALocation);
        int b = MemoryBuffer::read(ram, inputBLocation);
        MemoryBuffer::store(ram, resultLocation, a > b ? 1 : 0);
    }

    void lessOp(int resultLocation, int inputALocation, int inputBLocation) {
        int a = MemoryBuffer::read(ram, inputALocation);
        int b = MemoryBuffer::read(ram, inputBLocation);
        MemoryBuffer::store(ram, resultLocation, a < b ? 1 : 0);
    }

    void equalOp(int resultLocation, int inputALocation, int inputBLocation) {
        int a = MemoryBuffer::read(ram, inputALocation);
        int b = MemoryBuffer::read(ram, inputBLocation);
        MemoryBuffer::store(ram, resultLocation, a == b ? 1 : 0);
    }

    void ifOp(int booleanLocation, int trueInstr, int falseInstr, int& currentInstr) {
        int condition = MemoryBuffer::read(ram, booleanLocation);
        currentInstr = condition ? trueInstr : falseInstr;
    }

    void inputOp(int RAMLocation) {
        int input = obtainInput();
        MemoryBuffer::store(ram, RAMLocation, input);
    }

    void outputOp(int RAMLocation) {
        int val = MemoryBuffer::read(ram, RAMLocation);
        printOut(val);
    }

    void gotoOp(int targetInstructionNumber, int& currentInstructionNumber) {
        currentInstructionNumber = targetInstructionNumber;
    }

    void persistOp() {
        writeToFile("storage.dat", storage->buffer, storage->bufferSize);
    }

    void reloadOp() {
        readFromFile("storage.dat", storage->buffer, storage->bufferSize);
    }

    void constOp(int constValue, int RAMLocation) {
        MemoryBuffer::store(ram, RAMLocation, constValue);
    }

    void moveOp(int sourceLocation, int destinationLocation) {
        int val = MemoryBuffer::read(ram, sourceLocation);
        MemoryBuffer::store(ram, destinationLocation, val);
    }

    void debugPrintout(const Instruction* instr) {
        std::cout << "Opcode: " << instr->opcode << "\n";
        int count = determineNumberOfOperands(instr->opcode);
        for (int i = 0; i < count; ++i) {
            std::cout << "Operand[" << i << "]: " << instr->operands[i] << "\n";
        }
    }

}
