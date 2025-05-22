#include "Memory.h"
#include "IO.h"
// Global memory buffer variables
MemoryBuffer::MemoryBufferObject* ram = NULL;
MemoryBuffer::MemoryBufferObject* storage = NULL;

namespace MemoryBuffer {

    // Helper function to check if memory buffer object and buffer are valid
    bool isValidBuffer(MemoryBufferObject* memoryBufferObject) {
        if (memoryBufferObject == NULL || memoryBufferObject->buffer == NULL) {
            printError(0);
            return false;
        }
        return true;
    }

    // Print the contents of the memory buffer
    void printOutContents(MemoryBufferObject* memoryBufferObject) {
        // Check if buffer is valid
        if (!isValidBuffer(memoryBufferObject)) {
            return;
        }

        // Print each index and its value
        for (int i = 0; i < memoryBufferObject->bufferSize; i++) {
            std::cout << "[" << i << "]: " << memoryBufferObject->buffer[i] << std::endl;
        }
    }


    

    // Return a pointer to a specific index in the buffer
    int* dereference(MemoryBufferObject* memoryBufferObject, int memoryAddress) {
        // Check if buffer is valid
        if (!isValidBuffer(memoryBufferObject)) {
            return NULL;
        }

        // Check if address is within valid range
        if (memoryAddress < 0 || memoryAddress >= memoryBufferObject->bufferSize) {
            printError(2);
            return NULL;
        }

        // Return pointer to the memory at the specified address
        return &(memoryBufferObject->buffer[memoryAddress]);
    }

    // Store a value at a specific address in the buffer
    void store(MemoryBufferObject* memoryBufferObject, int memoryAddress, int value) {
        // Check if buffer is valid
        if (!isValidBuffer(memoryBufferObject)) {
            return;
        }

        // Check if address is within valid range
        if (memoryAddress < 0 || memoryAddress >= memoryBufferObject->bufferSize) {
            printError(2);
            return;
        }

        // Store the value at the specified address
        memoryBufferObject->buffer[memoryAddress] = value;
    }

    // Read a value from a specific address in the buffer
    int read(MemoryBufferObject* memoryBufferObject, int memoryAddress) {
        // Check if buffer is valid
        if (!isValidBuffer(memoryBufferObject)) {
            return 0;
        }

        // Check if address is within valid range
        if (memoryAddress < 0 || memoryAddress >= memoryBufferObject->bufferSize) {
            printError(2);
            return 0;
        }

        // Return the value stored at the specified address
        return memoryBufferObject->buffer[memoryAddress];
    }

    // Create a new buffer with a given size
    void createBuffer(MemoryBufferObject*& memoryBufferObject, int bufferSize) {
        // Check for invalid size
        if (bufferSize < 0) {
            printError(3);
            return;
        }

        // If buffer already exists, destroy it first
        if (memoryBufferObject != NULL) {
            destroyBuffer(memoryBufferObject);
        }

        // Allocate memory for new buffer object and initialize its contents
        memoryBufferObject = new MemoryBufferObject;
        memoryBufferObject->bufferSize = bufferSize;
        memoryBufferObject->buffer = new int[bufferSize];

        // Initialize all values to 0
        for (int i = 0; i < bufferSize; i++) {
            memoryBufferObject->buffer[i] = 0;
        }
    }

    // Destroy the buffer and free memory
    void destroyBuffer(MemoryBufferObject*& memoryBufferObject) {
        // If memory buffer is null, do nothing
        if (memoryBufferObject == NULL) {
            return;
        }

        // Deallocate memory and set pointer to NULL
        delete[] memoryBufferObject->buffer;
        delete memoryBufferObject;

        memoryBufferObject = NULL;
    }

} // namespace MemoryBuffer
