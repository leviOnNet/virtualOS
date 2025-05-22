#include "OS.h"

int main() {
    using namespace OperatingSystem;

    bootComputer(64, 64); // Small RAM and storage for test

    // Translate a high-level program to low-level binary
    translateProgram("highLevelProgram.txt", "lowLevelProgram.txt");

    // Load program instructions
    loadInstructions("lowLevelProgram.txt");

    // Debug print
    printProgram();

    // Execute instructions
    executeProgram();

    // Clean up
    shutDownComputer();

    return 0;
}
