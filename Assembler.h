//
//		Assembler class.  This is a container for all the components
//		that make up the assembler.
//
#pragma once 

#include "SymTab.h"
#include "Instruction.h"
#include "FileAccess.h"
#include "Emulator.h"


class Assembler {

public:
    Assembler( int argc, char *argv[] );

    // Pass I - establish the locations of the symbols
    void PassI( );

    // Pass II - generate a translation
    void PassII();


    // Display the symbols in the symbol table.
    void DisplaySymbolTable() { m_symtab.DisplaySymbolTable(); }
    
    // Run emulator on the translation.
    void RunProgramInEmulator();


private:

    FileAccess m_facc;	    // File Access object
    SymbolTable m_symtab;   // Symbol table object
    Instruction m_inst;	    // Instruction object
    emulator m_emul;        // Emulator object
    vector<pair<long long, int>> m_translatedInstructions; // map to store translated instructions and its location

    
    void MachineLanguageTranslation(bool& endFound, bool& haltFound, int loc);  // This function translates machine language instructions into their corresponding machine code.
    void AssemblyTranslation(bool& endFound, int loc);      //   This function translates assembly language instructions  "org", "ds", and "dc".
    void IllegalInstruction(bool& endFound, int loc);       //    This function handles illegal instructions by checking for missing or undefined opcodes and operands.
    string formatOpCode(int num);    // This function formats an opcode integer as a 2-digit string with leading zeros if necessary.
    string formatAddress(int num);   // This function formats an address integer as a 5 - digit string with leading zeros if necessary.

};

