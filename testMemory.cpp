#include "Memory.h"
#include <iostream>
#include <cassert>

void testCreateBuffer() {
    MemoryBuffer::MemoryBufferObject* testBuffer = NULL;
    
    // Create a buffer of size 5
    MemoryBuffer::createBuffer(testBuffer, 5);
    assert(testBuffer != NULL);  // Ensure buffer is created
    assert(testBuffer->bufferSize == 5);  // Ensure buffer size is correct
    assert(testBuffer->buffer != NULL);  // Ensure buffer array is allocated

    // Clean up
    MemoryBuffer::destroyBuffer(testBuffer);
}

void testStoreAndRead() {
    MemoryBuffer::MemoryBufferObject* testBuffer = NULL;
    MemoryBuffer::createBuffer(testBuffer, 5);
    
    // Store values in the buffer
    MemoryBuffer::store(testBuffer, 2, 42);
    
    // Read the value back
    int value = MemoryBuffer::read(testBuffer, 2);
    assert(value == 42);  // Ensure the correct value was stored and retrieved
    
    // Clean up
    MemoryBuffer::destroyBuffer(testBuffer);
}

void testDereference() {
    MemoryBuffer::MemoryBufferObject* testBuffer = NULL;
    MemoryBuffer::createBuffer(testBuffer, 5);
    
    // Store a value
    MemoryBuffer::store(testBuffer, 3, 99);
    
    // Dereference and check the value
    int* valuePtr = MemoryBuffer::dereference(testBuffer, 3);
    assert(valuePtr != NULL);  // Ensure the pointer is valid
    assert(*valuePtr == 99);   // Ensure the correct value is stored at the address
    
    // Test invalid dereference (out of bounds)
    valuePtr = MemoryBuffer::dereference(testBuffer, 10);  // Should return NULL
    assert(valuePtr == NULL);  // Ensure NULL is returned for invalid memory address
    
    // Clean up
    MemoryBuffer::destroyBuffer(testBuffer);
}

void testPrintOutContents() {
    MemoryBuffer::MemoryBufferObject* testBuffer = NULL;
    MemoryBuffer::createBuffer(testBuffer, 3);
    
    // Store some values
    MemoryBuffer::store(testBuffer, 0, 5);
    MemoryBuffer::store(testBuffer, 1, 10);
    MemoryBuffer::store(testBuffer, 2, 15);
    
    // Print contents (this should display the values in the buffer)
    MemoryBuffer::printOutContents(testBuffer);
    
    // Clean up
    MemoryBuffer::destroyBuffer(testBuffer);
}

int main() {
    // Run tests
    std::cout << "Running tests...\n";
    
    testCreateBuffer();
    std::cout << "testCreateBuffer passed\n";
    
    testStoreAndRead();
    std::cout << "testStoreAndRead passed\n";
    
    testDereference();
    std::cout << "testDereference passed\n";
    
    testPrintOutContents();
    std::cout << "testPrintOutContents passed\n";

    return 0;
}
