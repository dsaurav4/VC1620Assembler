//
//      Implementation of the Assembler class.
//
#include "stdafx.h"
#include "Assembler.h"
#include "Errors.h"
#include <iomanip>

/**/
/*
Assembler::Assembler(int argc, char *argv[])

NAME

        Assembler::Assembler - constructs an Assembler object and initializes FileAccess object.

SYNOPSIS

        Assembler::Assembler(int argc, char *argv[]);
            argc        --> number of command-line arguments.
            argv        --> array of command-line argument strings.

DESCRIPTION

        This constructor initializes an Assembler object and passes the command-line arguments to
        the FileAccess object for further processing.

RETURNS

        None.

AUTHOR

        Saurav Dahal

DATE

        5:15pm 5/5/2023

*/
/**/
Assembler::Assembler( int argc, char *argv[] )
: m_facc( argc, argv )
{
    // Nothing else to do here at this point.
}  
/*Assembler::Assembler( int argc, char *argv[] );*/



/**/
/*
Assembler::PassI()

NAME

        Assembler::PassI - performs the first pass of the assembler, establishing the location of labels.

SYNOPSIS

        void Assembler::PassI();

DESCRIPTION

        This function processes the first pass of the assembler, which establishes the location of labels
        in the source code. It reads each line of the source code, parsing the instruction and determining
        its type. If an instruction has a label, it is added to the symbol table along with its location.
        The function keeps track of the location of instructions, updating it as it processes each line.

RETURNS

        None.

AUTHOR

        Saurav Dahal

DATE

        5:15pm 5/5/2023

*/
/**/
void Assembler::PassI( ) 
{
    int loc = 0;        // Tracks the location of the instructions to be generated.
    m_symtab.Clear();
    // Successively process each line of source code.
    for( ; ; ) {

        // Read the next line from the source file.
        string line; 
        if( ! m_facc.GetNextLine( line ) ) {

            // If there are no more lines, we are missing an end statement.
            // We will let this error be reported by Pass II.
            return;
        }
        // Parse the line and get the instruction type.
        Instruction::InstructionType st =  m_inst.ParseInstruction( line );

        // If this is an end statement, there is nothing left to do in pass I.
        // Pass II will determine if the end is the last statement and report an error if it isn't.
        if( st == Instruction::ST_End ) return;

        // Labels can only be on machine language and assembler language
        // instructions.  So, skip comments.
        if( st == Instruction::ST_Comment )  
        {
        	continue;
	    }
        // If the instruction has a label, record it and its location in the
        // symbol table.
        if( m_inst.isLabel( ) ) {

            m_symtab.AddSymbol( m_inst.GetLabel( ), loc );
        }
        // Compute the location of the next instruction.
        loc = m_inst.LocationNextInstruction( loc );
    }
}
/*void Assembler::PassI( );*/


/**/
/*
Assembler::PassII()

NAME

        Assembler::PassII - performs the second pass of the assembler, generating machine code.

SYNOPSIS

        void Assembler::PassII();

DESCRIPTION

        This function processes the second pass of the assembler, generating the machine code for each
        instruction in the source code. It reads each line of the source code, parsing the instruction
        and determining its type. Depending on the type of instruction, it generates the corresponding
        machine code, records errors, and outputs the results to the console. It also keeps track of the
        location of instructions, updating it as it processes each line.

RETURNS

        None.

AUTHOR

        Saurav Dahal

DATE

        5:15pm 5/5/2023

*/
/**/
void Assembler::PassII() {
    // Prompt the user to press enter to continue.
    cout << "Press enter to continue...";
    cin.ignore();

    Errors::InitErrorReporting();

    // Output section headers.
    cout << endl;
    cout << "Translation of Program:" << endl << endl;;
    cout << left << setw(10) << "Location" << left << setw(15) << "Contents" << left << setw(15) << "Original Statement" << endl;

    // Declare and initialize some variables.
    string line;
    bool endFound = false; //Checks if an end statement was found.
    bool haltFound = false; //Checks if halt statement was found.
    bool haltErrorDisplay = false;  //Checks if missing halt error was already printed

    m_facc.rewind(); // Rewind the input file to the beginning.
    int loc = 0; // Tracks the location of the instructions to be generated.

    // Loop through each line of the input file.
    while (true)
    {
        // If there is no next line in the input file, and there was no "END" statement, record an error.
        if (!m_facc.GetNextLine(line)) {
            if (!endFound)
            {
                Errors::RecordError("No END Instruction.");
            }
            Errors::DisplayErrors();
            break;
        }

        //If we have exceeded the memory capacity, record an error.
        if (loc > m_emul.MEMSZ)
        {
            Errors::RecordError("Insufficient Memory");
            Errors::DisplayErrors();
            break;
        }

        // Parse the current instruction and determine its type.
        Instruction::InstructionType st = m_inst.ParseInstruction(line);

        // If the label starts with a digit, it is invalid. Record an error.
        // If the label size is greater than 10, it is invalid. Record an error.
        //If the label is not alphanumeric, it is invalid. Recprd an error. 
        if (::isdigit(m_inst.GetLabel()[0]) || m_inst.GetLabel().size() > 10 || !m_inst.IsLabelAlphaNumeric())
        {
            Errors::RecordError("Invalid Label Name. ---->" + m_inst.GetLabel());
        }

        // If the instruction is a machine language instruction, generate the corresponding machine code.
        if (st == Instruction::ST_MachineLanguage)
        {
            MachineLanguageTranslation(endFound, haltFound, loc);
        }

        // If the instruction is an assembler instruction, generate the corresponding machine code.
        if (st == Instruction::ST_AssemblerInstr)
        {
            AssemblyTranslation(endFound, loc);         
        }

        // If the instruction is an "END" statement. Record an error if we have already encountered one.
        if (st == Instruction::ST_End)
        {
            if (endFound)
            {
                Errors::RecordError("Multiple End Statements.");
            }
            endFound = true;
            cout << left << setw(10) << "" << left << setw(15) << "" << left << setw(15) << m_inst.GetInstruction() << endl;
            if (m_inst.IsExtraStatement())
            {
                Errors::RecordError("Extra statement elements.---->"+m_inst.GetExtraElement());
            }
            Errors::DisplayErrors();
        }

        // If the instruction is a comment, output it to the console and continue to the next line.
        if (st == Instruction::ST_Comment) {
            cout << left << setw(10) << "" << left << setw(15) << "" << left << setw(15) << m_inst.GetInstruction() << endl;
            continue;
        }

        // If the instruction is illegal, record an error.
        if (st == Instruction::ST_Illegal)
        {
            IllegalInstruction(endFound, loc);
        }

        // Update the location counter to the next instruction.
        loc = m_inst.LocationNextInstruction(loc);
    }

    // If there was no "HALT" statement, record an error and dsipaly.
    if (!haltFound) Errors::RecordError("Halt Statement Missing");
    Errors::DisplayErrors();

    // Output section divider and prompt the user to press enter to continue.
    cout << endl;
    cout << "-------------------------------------------------------------------" << endl;
    cout << endl;

    cout << "Press enter to continue...";
    cin.ignore();
}
/*void Assembler::PassII()*/



/**/
/*
Assembler::MachineLanguageTranslation(bool& endFound, bool& haltFound, int loc)

NAME

        Assembler::MachineLanguageTranslation - Translates machine language instructions.

SYNOPSIS

        void Assembler::MachineLanguageTranslation(bool& endFound, bool& haltFound, int loc);
            endFound        --> Indicates if the END statement has been encountered.
            haltFound       --> Indicates if the HALT statement has been encountered.
            loc             --> The location of the current instruction.

DESCRIPTION

        This function translates machine language instructions into their corresponding
        machine code. It processes the opcode, operands, and checks for errors.
        It outputs the location, machine code, and the original instruction.

RETURNS

        None.

AUTHOR

        Saurav Dahal

DATE

        5:15pm 5/5/2023

*/
/**/
void Assembler::MachineLanguageTranslation(bool& endFound, bool& haltFound, int loc)
{
    
    string translatedInstructions;
    int m_tempAddr = 0;
    string m_addr1, m_addr2;
    int multiplyDefined = 0;

    // Check if the opcode requires two operands.
    if ((m_inst.GetNumericOpCode() >= 1 && m_inst.GetNumericOpCode() <= 5) || (m_inst.GetNumericOpCode() >= 10 && m_inst.GetNumericOpCode() <= 12))
    {
        if (m_symtab.LookupSymbol(m_inst.GetOperand1(), m_tempAddr)) {
            m_addr1 = formatAddress(m_tempAddr);
        }
        else {
            if (m_inst.GetOperand1().empty())
            {
                Errors::RecordError("Missing operand.");
            }
            else if (m_inst.IsOperand1Numeric())
            {
                Errors::RecordError("Operand should be symbolic.---->"+m_inst.GetOperand1());
            }
            else
            {
                Errors::RecordError("Undefined Label.---->"+m_inst.GetOperand1());
            }
            m_addr1 = "?????";
        }

        if (m_symtab.LookupSymbol(m_inst.GetOperand2(), m_tempAddr)) {
            m_addr2 = formatAddress(m_tempAddr);
        }
        else {
            if (m_inst.GetOperand2().empty())
            {
                Errors::RecordError("Missing operand.");
            }
            else if (m_inst.IsOperand2Numeric())
            {
                Errors::RecordError("Operand should be symbolic.---->" + m_inst.GetOperand2());
            }
            else
            {
                Errors::RecordError("Undefined Label.---->" + m_inst.GetOperand2());
            }
            m_addr2 = "?????";
        }

        // Combine the opcode and operands to form the machine code
        translatedInstructions = formatOpCode(m_inst.GetNumericOpCode()) + m_addr1 + m_addr2;
        m_translatedInstructions.push_back(make_pair(stoll(translatedInstructions), loc));

        // Output the machine code and original instruction to the console
        cout << left << setw(10) << loc << left << setw(15) << translatedInstructions << left << setw(15) << m_inst.GetInstruction() << endl;

        // Record an error if the instruction has an extra element.
        if (m_inst.IsExtraStatement())
        {
            Errors::RecordError("Extra statement elements.---->"+m_inst.GetExtraElement());
        }
    }

    // Checks if the opcode requires only one operand.
    else if (m_inst.GetNumericOpCode() != 13)
    {
        if (m_symtab.LookupSymbol(m_inst.GetOperand1(), m_tempAddr)) {
            m_addr1 = formatAddress(m_tempAddr);
        }
        else {
            if (m_inst.GetOperand1().empty())
            {
                Errors::RecordError("Missing operand.");
            }
            else if (m_inst.IsOperand1Numeric())
            {
                Errors::RecordError("Operand should be symbolic.---->"+m_inst.GetOperand1());
            }
            else
            {
                Errors::RecordError("Undefined Label.---->"+m_inst.GetOperand1());
            }
            m_addr1 = "?????";
        }

        translatedInstructions = formatOpCode(m_inst.GetNumericOpCode()) + m_addr1 + formatAddress(0);
        m_translatedInstructions.push_back(make_pair(stoll(translatedInstructions), loc));

        cout << left << setw(10) << loc << left << setw(15) << translatedInstructions << left << setw(15) << m_inst.GetInstruction() << endl;
        if (m_inst.IsExtraStatement())
        {
            Errors::RecordError("Extra statement elements or extra operand.---->"+m_inst.GetExtraElement());
        }
    }
    
    //Check if the OpCode is halt.
    else
    {
        if (haltFound)
        {
            Errors::RecordError("Multiple HALT Statements.");
        }
        haltFound = true;
        translatedInstructions = formatOpCode(m_inst.GetNumericOpCode()) + formatAddress(0) + formatAddress(0);
        cout << left << setw(10) << loc << left << setw(15) << translatedInstructions << left << setw(15) << m_inst.GetInstruction() << endl;
        m_translatedInstructions.push_back(make_pair(stoll(translatedInstructions), loc));

        if (m_inst.IsExtraStatement())
        {
            Errors::RecordError("Extra statement elements.---->" + m_inst.GetExtraElement());
        }
    }

    if (m_inst.isLabel())
    {
        m_symtab.LookupSymbol(m_inst.GetLabel(), multiplyDefined);
        if (multiplyDefined == -999)
        {
            Errors::RecordError("Multiply Defined Label---->" + m_inst.GetLabel());
        }
    }

    //checkls if the Instruction is after end statement.
    if (endFound)
    {
        Errors::RecordError("Satement after END Instruction");
    }

    Errors::DisplayErrors();
}
/*void Assembler::MachineLanguageTranslation(bool& endFound, bool& haltFound, int loc);*/


/**/
/*
Assembler::AssemblyTranslation(bool& endFound, int loc)

NAME

        Assembler::AssemblyTranslation - Translates assembly language instructions.

SYNOPSIS

        void Assembler::AssemblyTranslation(bool& endFound, int loc);
            endFound        --> Indicates if the END statement has been encountered.
            loc             --> The location of the current instruction.

DESCRIPTION

        This function translates assembly language instructions  "org", "ds", and "dc".
        It processes the opcode, operands, and checks for errors.
        It outputs the location, machine code (when applicable), and the original instruction.

RETURNS

        None.

AUTHOR

        Saurav Dahal

DATE

        5:15pm 5/5/2023

*/
/**/
void Assembler::AssemblyTranslation(bool& endFound, int loc)
{
    // Initialize variables for translation process
    string translatedInstructions;
    int m_tempAddr = 0;
    string m_addr1, m_addr2;
    int multiplyDefined = 0;

    // Check if the opcode is "org"
    if (m_inst.GetOpCode() == "org")
    {
        // Display the location and instruction
        cout << left << setw(10) << loc << left << setw(15) << "" << left << setw(15) << m_inst.GetInstruction() << endl;

        // Check for missing operand
        if (m_inst.GetOperand1().empty())
        {
            Errors::RecordError("Missing operand.");
        }

        // Check for non-numeric operand
        if (!m_inst.IsOperand1Numeric())
        {
            Errors::RecordError("Operand must be numeric.---->" + m_inst.GetOperand1());
        }

        // Check if operand is too large
        if (m_inst.GetNumericOperand() > 100'000)
        {
            Errors::RecordError("Operand too large. Operand should be less than 100'000.---->" + m_inst.GetOperand1());
        }

        // Check if operand is negative
        if (m_inst.GetNumericOperand() < 0)
        {
            Errors::RecordError("Operand must be positive.---->" + m_inst.GetOperand1());
        }

        if (m_inst.IsExtraStatement())
        {
            Errors::RecordError("Extra statement elements.---->" + m_inst.GetExtraElement());
        }
    }

    // Check if the opcode is "ds"
    if (m_inst.GetOpCode() == "ds")
    {
        // Display the location and instruction
        cout << left << setw(10) << loc << left << setw(15) << "" << left << setw(15) << m_inst.GetInstruction() << endl;

        // Check for missing operand
        if (m_inst.GetOperand1().empty())
        {
            Errors::RecordError("Missing operand.");
        }

        // Check for non-numeric operand
        if (!m_inst.IsOperand1Numeric() && !m_inst.GetOperand1().empty())
        {
            Errors::RecordError("Operand must be numeric.---->" + m_inst.GetOperand1());
        }

        // Check if operand is too large
        if (m_inst.GetNumericOperand() > 99999)
        {
            Errors::RecordError("Operand too large. Operand should be less than 100'000.---->" + m_inst.GetOperand1());
        }

        // Check if operand is negative
        if (m_inst.GetNumericOperand() < 0)
        {
            Errors::RecordError("Operand must be positive.---->" + m_inst.GetOperand1());
        }

        if (m_inst.IsExtraStatement())
        {
            Errors::RecordError("Extra statement elements.---->" + m_inst.GetExtraElement());
        }
    }

    // Check if the opcode is "dc"
    if (m_inst.GetOpCode() == "dc")
    {
        // Format and display the location, translated instruction, and original instruction
        if (m_inst.IsOperand1Numeric())
        {
            ostringstream oss;
            oss << setw(12) << setfill('0') << stoll(m_inst.GetOperand1());
            cout << left << setw(10) << loc << left << setw(15) << oss.str() << left << setw(15) << m_inst.GetInstruction() << endl;
        }
        else
        {
            ostringstream oss;
            oss << setw(12) << setfill('?') << "";
            cout << left << setw(10) << loc << left << setw(15) << oss.str() << left << setw(15) << m_inst.GetInstruction() << endl;
        }

        // Check for missing operand
        if (m_inst.GetOperand1().empty())
        {
            Errors::RecordError("Missing operand.");
        }

        // Check for non-numeric operand
        if (!m_inst.IsOperand1Numeric() && !m_inst.GetOperand1().empty())
        {
            Errors::RecordError("Operand must be numeric.---->" + m_inst.GetOperand1());
        }

        // Check if constant is too large for memory
        if (m_inst.GetNumericOperand() > 999'999'999'999 || m_inst.GetNumericOperand() < -999'999'999'999)
        {
            Errors::RecordError("Constant too large for Memory.---->" + m_inst.GetOperand1());
        }

        if (m_inst.IsExtraStatement())
        {
            Errors::RecordError("Extra statement elements.---->" + m_inst.GetExtraElement());
        }
    }

    // Check if the instruction has a label
    if (m_inst.isLabel())
    {
        // Check if the abel is multiply defined
        m_symtab.LookupSymbol(m_inst.GetLabel(), multiplyDefined);
        if (multiplyDefined == -999)
        {
            Errors::RecordError("Multiply Defined Label---->" + m_inst.GetLabel());
        }
    }

    // Check if the instruction is after an "end" statement.
    if (endFound)
    {
        Errors::RecordError("Statement after END Instruction");
    }

    // Display any recorded errors
    Errors::DisplayErrors();
}
/*void Assembler::AssemblyTranslation(bool& endFound, int loc);*/


/**/
/*
Assembler::IllegalInstruction(bool& endFound, int loc)

NAME

        Assembler::IllegalInstruction - Handles illegal instructions.

SYNOPSIS

        void Assembler::IllegalInstruction(bool& endFound, int loc);
            endFound        --> Indicates if the END statement has been encountered.
            loc             --> The location of the current instruction.

DESCRIPTION

        This function handles illegal instructions by checking for missing or undefined opcodes
        and operands. It records errors, outputs the location, translated instructions
        (with '?' for undefined parts), and the original instruction.

RETURNS

        None.

AUTHOR

        Saurav Dahal

DATE

        5:15pm 5/5/2023

*/
/**/
void Assembler::IllegalInstruction(bool& endFound, int loc)
{
    // Initialize variables to store temporary address, address values, and translated instructions
    int m_tempAddr = 0;
    string m_addr1, m_addr2;
    string translatedInstructions;
    int multiplyDefined = 0;

    // Check if the numeric opcode is 0
    if (m_inst.GetNumericOpCode() == 0)
    {
        // Check if the opcode is missing
        if (m_inst.GetOpCode() == "")
            Errors::RecordError("Missing OpCode.");
        // Record an error if the opcode is illegal
        else
            Errors::RecordError("Illegal Operation code.---->" + m_inst.GetOpCode());

        // Look up the first operand in the symbol table and format the address
        if (m_symtab.LookupSymbol(m_inst.GetOperand1(), m_tempAddr)) {
            m_addr1 = formatAddress(m_tempAddr);
        }
        // If the first operand is not found in the symbol table and is not numeric, record an error
        else
        {
            m_addr1 = "?????";
            if (m_inst.GetOperand1().empty()) Errors::RecordError("Missing Operand.");
            if(!m_inst.IsOperand1Numeric() && !m_inst.GetOperand1().empty()) Errors::RecordError("Undefined Label---->" + m_inst.GetOperand1());
        }

        // Look up the second operand in the symbol table and format the address
        if (m_symtab.LookupSymbol(m_inst.GetOperand2(), m_tempAddr)) {
            m_addr2 = formatAddress(m_tempAddr);
        }
        // If the second operand is numeric, format the address
        else if (m_inst.IsOperand2Numeric()) {
            m_addr2 = formatAddress(static_cast<int>(m_inst.GetNumericOperand()));
        }
        // If the second operand is not found in the symbol table and is not numeric, record an error
        else
        {
            m_addr2 = "?????";
            if (!m_inst.IsOperand2Numeric() && !m_inst.GetOperand2().empty()) Errors::RecordError("Undefined Label---->" + m_inst.GetOperand2());
        }

        // Concatenate the translated instructions
        translatedInstructions = "??" + m_addr1 + m_addr2;

        // Output the translated instructions and the original instruction
        cout << left << setw(10) << loc << left << setw(15) << translatedInstructions << left << setw(15) << m_inst.GetInstruction() << endl;

        // Check if there are extra statement elements
        if (m_inst.IsExtraStatement())
        {
            Errors::RecordError("Extra statement elements.---->" + m_inst.GetExtraElement());
        }
    }

    // Check if the instruction has a label
    if (m_inst.isLabel())
    {
        // Look up the label in the symbol table and check if it's multiply defined
        m_symtab.LookupSymbol(m_inst.GetLabel(), multiplyDefined);
        if (multiplyDefined == -999)
        {
            Errors::RecordError("Multiply Defined Label");
        }
    }

    // Check if the end statement has been found
    if (endFound)
    {
        // Record an error if the instruction is after the end statement
        Errors::RecordError("Statement after END Instruction");
    }

    // Display the recorded errors
    Errors::DisplayErrors();
}
/*void Assembler::IllegalInstruction(bool& endFound, int loc)*/


/**/
/*
Assembler::RunProgramInEmulator() Assembler::RunProgramInEmulator()

NAME

        Assembler::RunProgramInEmulator - runs the assembled program in the emulator.

SYNOPSIS

        void Assembler::RunProgramInEmulator();

DESCRIPTION

        This function first checks if there are no errors in the program. If no errors are found,
        it iterates through each line in the file and processes the instructions. If the opcode
        is "dc", the constant value is inserted into memory at the symbol's location. After
        processing all instructions, the machine language instructions are inserted into memory
        and the emulator is instructed to run the program. If the program execution fails, an
        error message is printed.

RETURNS

        void

AUTHOR

        Saurav Dahal

DATE

        5:15pm 5/5/2023

*/
/**/
void Assembler::RunProgramInEmulator()
{
    // Reset the position of the file accessor
    m_facc.rewind();
    // Variable to store each line of the file
    string line;
    // Variable to store the location of a symbol
    int tempLoc;

    // Check if there are no errors in the program
    if (Errors::IsEmpty())
    {
        // Loop through each line in the file
        while (true)
        {
            // Check if there are no more lines in the file
            if (!m_facc.GetNextLine(line)) break;

            // Parse the instruction
            Instruction::InstructionType st = m_inst.ParseInstruction(line);

            // Check if the opcode is "dc"
            if (m_inst.GetOpCode() == "dc")
            {
                // Get the location of the symbol
                m_symtab.LookupSymbol(m_inst.GetLabel(), tempLoc);
                // Insert the constant value into memory at the symbol's location
                m_emul.insertMemory(tempLoc, m_inst.GetNumericOperand());
            }
        }

        // Insert the machine language instructions into memory
        for (auto it = m_translatedInstructions.begin(); it != m_translatedInstructions.end(); it++)
        {
            m_emul.insertMemory(it->second, it->first);
        }

        // Run the program in the emulator
        if (!m_emul.runProgram())
        {
            // Print an error message if the program execution failed
            cerr << "Error: Program execution in emulator failed." << endl;
        }
        else
        {
            cout << endl << "End of emulation" << endl;
        }
    }
    else
    {
        // Print an error message if there are errors in the program
        cout << "\nEmulator can only run if there are no errors." << endl;
    }
}
/*void Assembler::RunProgramInEmulator();*/



/**/
/*
Assembler::formatOpCode() Assembler::formatOpCode()

NAME

        Assembler::formatOpCode - formats an opcode as a 2-digit string.

SYNOPSIS

        string Assembler::formatOpCode(int num);
            num   --> the opcode integer to be formatted.

DESCRIPTION

        This function formats an opcode integer as a 2-digit string with leading zeros if
        necessary. It creates a string stream object, inserts the integer value into the stream
        with a width of 2, padded with '0', and returns the formatted string.

RETURNS

        Returns the formatted opcode string.

AUTHOR

        Saurav Dahal

DATE

        5:15pm 5/5/2023

*/
/**/
string Assembler::formatOpCode(int num) {
    // Create a string stream object
    ostringstream oss;
    // Insert the integer value into the stream with a width of 2, padded with '0' if necessary
    oss << setw(2) << setfill('0') << num;
    // Return the formatted string
    return oss.str();
}
/*string Assembler::formatOpCode(int num);*/


/**/
/*
Assembler::formatAddress() Assembler::formatAddress()

NAME

        Assembler::formatAddress - formats an address as a 5-digit string.

SYNOPSIS

        string Assembler::formatAddress(int num);
            num   --> the address integer to be formatted.

DESCRIPTION

        This function formats an address integer as a 5-digit string with leading zeros if
        necessary. It creates a string stream object, inserts the integer value into the stream
        with a width of 5, padded with '0', and returns the formatted string.

RETURNS

        Returns the formatted address string.

AUTHOR

        Saurav Dahal

DATE

        5:15pm 5/5/2023

*/
/**/
string Assembler::formatAddress(int num) {
    // Create a string stream object
    ostringstream oss;
    // Insert the integer value into the stream with a width of 5, padded with '0' if necessary
    oss << setw(5) << setfill('0') << num;
    // Return the formatted string
    return oss.str();
}
/*string Assembler::formatAddress(int num)*/