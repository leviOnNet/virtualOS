#include "OS.h"

namespace OperatingSystem {
    Instructions::Instruction** programInstructions = nullptr;
    int numberOfInstructions = 0;
    int currentInstructionNumber = 0;

    void bootComputer(int ramSize, int storageSize) {
        MemoryBuffer::createBuffer(ram, ramSize);
        MemoryBuffer::createBuffer(storage, storageSize);

        for (int i = 0; i < ramSize; ++i) {
            MemoryBuffer::store(ram, i, 0);
        }
        for (int i = 0; i < storageSize; ++i) {
            MemoryBuffer::store(storage, i, 0);
        }

        currentInstructionNumber = 0;
    }

    void shutDownComputer() {
        MemoryBuffer::destroyBuffer(ram);
        MemoryBuffer::destroyBuffer(storage);

        for (int i = 0; i < numberOfInstructions; ++i) {
            Instructions::destroyInstruction(programInstructions[i]);
        }

        delete[] programInstructions;
        programInstructions = nullptr;
    }

    void loadHardDrive(std::string hardDriveDiskName) {
        if (storage == nullptr || storage->buffer == nullptr) {
            return;
        }

         // Load data from file into the storage buffer
        readFromFile(hardDriveDiskName, storage->buffer, storage->bufferSize);
    }

    int determineNumberOfInstructions(std::string programFile) {
        std::ifstream file(programFile);
        int count = 0;
        std::string line;

        while (std::getline(file, line)) {
            if (!line.empty()) {
                ++count;
            }
        }

        return count;
    }

    void loadInstructions(std::string programFile) {
        numberOfInstructions = determineNumberOfInstructions(programFile);
        programInstructions = new Instructions::Instruction*[numberOfInstructions];

        std::ifstream file(programFile);
        std::string line;
        int index = 0;

        while (std::getline(file, line)) {
            if (!line.empty()) {
                programInstructions[index++] = Instructions::createInstruction(line);
            }
        }
    }

    void executeProgram() {
    bool halt = false;

    while (currentInstructionNumber < numberOfInstructions && !halt) {
        if (currentInstructionNumber < 0 || currentInstructionNumber >= numberOfInstructions) {
            std::cerr << "Error: Instruction pointer out of bounds: " << currentInstructionNumber << std::endl;
            break;
        }

        int before = currentInstructionNumber;

        // Optionally log current instruction info for debugging
        std::cout << "Executing instruction " << currentInstructionNumber
                  << " opcode: " << programInstructions[currentInstructionNumber]->opcode
                  << " operands: " << programInstructions[currentInstructionNumber]->operands[0] << ", "
                  << programInstructions[currentInstructionNumber]->operands[1] << ", "
                  << programInstructions[currentInstructionNumber]->operands[2] << std::endl;

        Instructions::executeInstruction(programInstructions[currentInstructionNumber], currentInstructionNumber);

        // Only increment if the instruction didn't change the pointer itself
        if (currentInstructionNumber == before) {
            currentInstructionNumber++;
        }
    }
}



    void printProgram() {
        for (int i = 0; i < numberOfInstructions; ++i) {
            Instructions::debugPrintout(programInstructions[i]);
        }
    }

    void translateProgram(std::string highLevelFile, std::string lowLevelFile) {
        std::ifstream inFile(highLevelFile);
        std::ofstream outFile(lowLevelFile);
        std::string line;

        while (std::getline(inFile, line)) {
            std::stringstream ss(line);
            std::string token;
            std::string binaryLine;

            while (std::getline(ss, token, ',')) {
                int value = std::stoi(token);
                std::string binary;

                for (int i = 3; i >= 0; --i) {
                    binary += ((value >> i) & 1) ? '1' : '0';
                }

                binaryLine += binary;
            }

            outFile << binaryLine << std::endl;
        }
    }
}
