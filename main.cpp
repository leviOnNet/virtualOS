#include "OS.h"

int main() {
    using namespace OperatingSystem;

    bootComputer(64, 64); // Initialize memory
    loadInstructions("programTask.txt"); // Load the binary program
    executeProgram(); // Run the program
    shutDownComputer(); // Clean up
    return 0;
}
