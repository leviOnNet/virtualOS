#include "IO.h"




int strToInt(std::string str) {
    // Convert string to integer using stringstream
    std::stringstream ss(str);
    int result = 0;
    ss >> result;
    return result;
}

std::string intToBin(int value, int size) {
    std::string binary = "";

    // Convert to binary
    while (value > 0) {
    if (value % 2 == 0) {
        binary = "0" + binary;
    } else {
        binary = "1" + binary;
    }
    value = value / 2;
}

    // Adjust size
    if ((int)binary.length() > size) {
        // Drop leftmost bits
        binary = binary.substr(binary.length() - size);
    } else {
        // Pad with zeros on the left
        while ((int)binary.length() < size) {
            binary = "0" + binary;
        }
    }

    return binary;
}

void writeToFile(std::string fileName, const int* array, int size) {
    if (array == NULL || size <= 0) {
        return; // Invalid array or size
    }

    std::ofstream outFile(fileName.c_str());
    if (outFile.is_open()) {
        for (int i = 0; i < size; i++) {
            outFile << array[i] << std::endl;
        }
        outFile.close();
    }
}

void readFromFile(std::string fileName, int* array, int size) {
    if (array == NULL || size <= 0) {
        return; // Terminate if invalid
    }

    std::ifstream inFile(fileName.c_str());
    if (inFile.is_open()) {
        int i = 0;
        int value;
        while (inFile >> value && i < size) {
            array[i] = value;
            i++;
        }
        // Fill remaining values with 0
        while (i < size) {
            array[i] = 0;
            i++;
        }

        inFile.close();
    }
}

void printError(int errorCode) {
    if (errorCode == 0) {
        std::cout << "Error: Buffer Is Null" << std::endl;
    } else if (errorCode == 2) {
        std::cout << "Error: Segfault" << std::endl;
    } else if (errorCode == 3) {
        std::cout << "Error: Invalid buffer size" << std::endl;
    } else if (errorCode == 4) {
        std::cout << "Error: Unknown Instruction" << std::endl;
    }
    // Do nothing for any other value (including 1)
}

int obtainInput() {
    int input;
    std::cout << "Please enter an input:" << std::endl;
    std::cin >> input;
    return input;
}

void printOut(int value) {
    std::cout << "Printout: " << value << "$" << std::endl;
}

