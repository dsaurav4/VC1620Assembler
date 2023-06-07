//
//		Implementation of the symbol table class.  This is the format I want for commenting functions.
//
#include "stdafx.h"
#include "SymTab.h"
#include <iomanip>

/**/
/*
SymbolTable::AddSymbol() SymbolTable::AddSymbol()

NAME

    SymbolTable::AddSymbol - adds a symbol to the symbol table.

SYNOPSIS

    void SymbolTable::AddSymbol( const string &a_symbol, int a_loc );
        a_symbol   --> the symbol to be added to the table.
        a_loc      --> the location of the symbol.

DESCRIPTION

    This function adds a symbol to the symbol table. If the symbol already
    exists in the table, it will be marked as multiply defined.

RETURNS

    Void.

AUTHOR

    Saurav Dahal

DATE

    5:15pm 5/5/2023
*/
/**/
void SymbolTable::AddSymbol( const string &a_symbol, int a_loc )
{
    // If the symbol is already in the symbol table, record it as multiply defined.
    map<string, int>::iterator st = m_symbolTable.find( a_symbol );
    if( st != m_symbolTable.end() ) {

        st->second = multiplyDefinedSymbol;
        return;
    }
    // Record a the  location in the symbol table.
    m_symbolTable[a_symbol] = a_loc;
}
/*void SymbolTable::AddSymbol( const string &a_symbol, int a_loc );*/




/**/
/*
SymbolTable::LookupSymbol() SymbolTable::LookupSymbol()

NAME

    SymbolTable::LookupSymbol - look up a symbol in the symbol table.

SYNOPSIS

    bool SymbolTable::LookupSymbol(const string& a_symbol, int& a_loc);
        a_symbol   --> the symbol to be searched for in the table.
        a_loc      --> the location of the symbol, if found.

DESCRIPTION

    This function searches for a symbol in the symbol table. If the symbol is found,
    the location of the symbol will be updated and the function returns true.
    If the symbol is not found, the function returns false.

RETURNS

    Returns true if the symbol is found, false otherwise.

AUTHOR

    Saurav Dahal

DATE

    5:15pm 5/5/2023

*/
/**/
bool SymbolTable::LookupSymbol(const string& a_symbol, int& a_loc)
{
    // Search for the symbol in the symbol table.
    map<string, int>::iterator st = m_symbolTable.find(a_symbol);

    // If the symbol is found, update the location and return true.
    if (st != m_symbolTable.end())
    {
        a_loc = st->second;
        return true;
    }

    // If the symbol is not found, return false.
    return false;
}
/*bool SymbolTable::LookupSymbol(const string& a_symbol, int& a_loc);*/



/**/
/*
SymbolTable::DisplaySymbolTable() SymbolTable::DisplaySymbolTable()

NAME

    SymbolTable::DisplaySymbolTable - display the symbol table.

SYNOPSIS

    void SymbolTable::DisplaySymbolTable();

DESCRIPTION

    This function displays the symbol table, showing the symbol index, name, and location.

RETURNS

    Void.

AUTHOR

    Saurav Dahal

DATE

    5:15pm 5/5/2023

*/
/**/
void SymbolTable::DisplaySymbolTable()
{
    cout << "Symbol Table:" << endl << endl;
    cout << setw(10) << left << "Symbol#" << setw(10) << left << "Symbol" << setw(10) << left << "Location" << endl;

    // Iterate through the symbol table and display the symbol index, name, and location.
    int symbolIndex = 0;
    for (const auto& entry : m_symbolTable)
    {
        cout << setw(10) << left << symbolIndex++ << setw(10) << left << entry.first << left << setw(10) << entry.second << endl;
    }

    cout << endl;
    cout << "-------------------------------------------------------------------" << endl;
    cout << endl;
}
/*void SymbolTable::DisplaySymbolTable();*/



/**/
/*
SymbolTable::Clear() SymbolTable::Clear()

NAME

    SymbolTable::Clear - clears the symbol table.

SYNOPSIS

    void SymbolTable::Clear();

DESCRIPTION

    This function clears the symbol table, removing all symbols and their associated information.

RETURNS

    Void.

AUTHOR

    Saurav Dahal

DATE

    5:15pm 5/5/2023

*/
/**/
void SymbolTable::Clear()
{
    // Clear the symbol table.
    m_symbolTable.clear();
}
/*void SymbolTable::Clear()*/
