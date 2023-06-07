#include "stdafx.h"
#include "Emulator.h"
#include "Errors.h"


/**/
/*
emulator::insertMemory() emulator::insertMemory()

NAME

        emulator::insertMemory - inserts contents into memory at the specified location.

SYNOPSIS

        bool emulator::insertMemory(int a_location, long long a_contents);
            a_location   --> the memory location to insert the contents.
            a_contents   --> the contents to be inserted into memory.

DESCRIPTION

        This function checks if the provided memory location is within the valid range of memory
        addresses. If it is, the contents are inserted into the memory at the specified location
        and the function returns true. If the memory location is not valid, the function returns
        false.

RETURNS

        Returns true if the contents were inserted into memory, otherwise returns false.

AUTHOR

        Saurav Dahal

DATE

        5:15pm 5/5/2023

*/
/**/
bool emulator::insertMemory(int a_location, long long a_contents) {
    // Check if the memory location is within the range of valid memory addresses
    if (a_location >= 0 && a_location < MEMSZ) {
        // If valid, insert the contents into the memory at the specified location
        m_memory[a_location] = a_contents;
        return true;
    }
    else {
        // If invalid, return false
        return false;
    }
}
/*bool emulator::insertMemory(int a_location, long long a_contents);*/


/**/
/*
emulator::runProgram() emulator::runProgram()

NAME

        emulator::runProgram - runs the emulated program.

SYNOPSIS

        bool emulator::runProgram();

DESCRIPTION

        This function runs the emulated program, executing the machine language instructions
        stored in memory. It iterates through memory locations and processes the instructions,
        applying the appropriate operations based on the opcode. If the program execution is
        successful, the function returns true. If an unknown opcode is encountered, the function
        returns false.

RETURNS

        Returns true if the program execution was successful, otherwise returns false.

AUTHOR

        Saurav Dahal

DATE

        5:15pm 5/5/2023

*/
/**/
bool emulator::runProgram() {

    cout << "Results from emulating program:" << endl << endl;
    int currentLocation = 100;
    bool halted = false;

    while (!halted) {
            if (m_memory[currentLocation] == 0) continue; 
            ostringstream oss;
            oss << setw(12) << setfill('0') << m_memory[currentLocation];
            string instruction = oss.str();
            int opcode = stoi(instruction.substr(0, 2));
            int operand1 = stoi(instruction.substr(2, 5));
            int operand2 = stoi(instruction.substr(7, 5));

            switch (opcode) {
            case 1: // ADD
                m_memory[operand1] = m_memory[operand1] + m_memory[operand2];
                if (m_memory[operand1] > 999'999'999'999) {
                    Errors::RecordError("Result too large for memory");
                    Errors::DisplayErrors();
                    return false;
                }
                break;

            case 2: // SUBTRACT
                m_memory[operand1] = m_memory[operand1] - m_memory[operand2];
                if (m_memory[operand1] < -999'999'999'999) {
                    Errors::RecordError("Result too large for memory");
                    Errors::DisplayErrors();
                    return false;
                }
                break;

            case 3: // MULTIPLY
                m_memory[operand1] = m_memory[operand1] * m_memory[operand2];
                if (m_memory[operand1] > 999'999'999'999) {
                    Errors::RecordError("Result too large for memory");
                    Errors::DisplayErrors();
                    return false;
                }
                break;

            case 4: // DIVIDE
                m_memory[operand1] = m_memory[operand1] / m_memory[operand2];
                break;

            case 5: // COPY
                m_memory[operand1] = m_memory[operand2];
                break;

            case 7: // READ
                long long input;
                cout << "? ";
                cin >> input;
                if (input > 999'999'999'999)
                {
                    Errors::RecordError("Input too large for memory");
                    Errors::DisplayErrors();
                    return false;
                }
                m_memory[operand1] = input;
                break;

            case 8: // WRITE
                cout << m_memory[operand1] << endl;
                break;

            case 9: // BRANCH
                currentLocation = operand1;
                continue;

            case 10: // BRANCH MINUS
                if (m_memory[operand2] < 0) {
                    currentLocation = operand1;
                    continue;
                }
                break;

            case 11: // BRANCH ZERO
                if (m_memory[operand2] == 0) {
                    currentLocation = operand1;
                    continue;
                }
                break;

            case 12: // BRANCH POSITIVE
                if (m_memory[operand2] > 0) {
                    currentLocation = operand1;
                    continue;
                }
                break;

            case 13: // halt
                halted = true;
                break;

            default:
                return false;
            }
        

        currentLocation++;
    }
    return true;
}
/*bool emulator::runProgram();*/
