//
// Class to parse and provide information about instructions.  Note: you will be adding more functionality.
//
#pragma once

// The elements of an instruction.
class Instruction {

public:

    Instruction() : m_NumOpCode(0), m_type(ST_Illegal), m_OperandValue(0), m_IsNumericOperand1(false), m_IsNumericOperand2(false) {}
    ~Instruction() { };

    // Codes to indicate the type of instruction we are processing.  Why is this inside the
    // class?  We should make this an enum class.  We will do this during a lecture.
    enum InstructionType {
        ST_MachineLanguage, 	// A machine language instruction.
        ST_AssemblerInstr,      // Assembler Language instruction.
        ST_Comment,             // Comment or blank line
        ST_End,                 // end instruction.
        ST_Illegal              //Illegal Instruction
    };
    // Parse the Instruction.
    InstructionType ParseInstruction(string a_line);

    // Compute the location of the next instruction.
    int LocationNextInstruction(int a_loc);

    // To access the label
    inline string& GetLabel() {

        return m_Label;
    };
    // To determine if a label is blank.
    inline bool isLabel() {

        return !m_Label.empty();
    };
    
    //To get OpCode in an instruction.
    inline string& GetOpCode() {

        return m_OpCode;
    };

    //To get the first Operand in an instruction.
    inline string& GetOperand1() {

        return m_Operand1;
    };


    //To get the second Operand in an instruction. 
    inline string& GetOperand2() {

        return m_Operand2;
    };

    //To get the operand if its numeric.
    inline long long& GetNumericOperand() {
        return m_OperandValue;
    }

    //To get the instruction.
    inline string& GetInstruction() {
        return m_instruction;
    }


    //To get the numeric value of opcode
    inline int& GetNumericOpCode() {
        return m_NumOpCode;
    }

    //To get extra statement.
    inline string& GetExtraElement()
    {
        return m_extra;
    }

    //Checks if the instruction has extra elements.
    inline bool IsExtraStatement() {
        return !m_extra.empty();
    }

    //checks if first operand is numeric.
    inline bool& IsOperand1Numeric() {
        return m_IsNumericOperand1;
    }

    //checks if the second operand is numeric.
    inline bool& IsOperand2Numeric() {
        return m_IsNumericOperand2;
    }

    //Checks if a label is alphanumeric
    inline bool IsLabelAlphaNumeric()
    {
        for (char c : m_Label) {
            if (!::isalnum(c)) {
                return false;
            }
        }
        return true;
    }

    //To remove comment 
    inline string RemoveComment(string& line) {
        size_t pos = line.find(';');
        if (pos == string::npos)
        {
            return line;
        }
        return line.erase(pos);
    }

private:


    // The elemements of a instruction
    string m_Label;            // The label.
    string m_OpCode;          // The symbolic op code.
    string m_Operand1;       // The first operand. 
    string m_Operand2;      // The second operand if it exists.
    string m_instruction;  // The original instruction.
    string m_extra;       // Any extra content in the current assembly instruction.

    // Derived values.
    int m_NumOpCode;     // The numerical value of the op code for machine language equivalents.
    InstructionType m_type; // The type of instruction.

    bool m_IsNumericOperand1;// == true if the operand1 is numeric.
    bool m_IsNumericOperand2;// == true if operand2 exists and is numeric.
    long long m_OperandValue;    // The value of the operand if it is numeric.
    void clearInstructionElements();

    map<string, int> m_machineInstrunctions = {
        {"add", 1},
        {"sub", 2},
        {"mult", 3},
        {"div", 4},
        {"copy", 5},
        {"read", 7},
        {"write", 8},
        {"b", 9},
        {"bm", 10},
        {"bz", 11},
        {"bp", 12},
        {"halt", 13}
    };

    vector<string> m_assemblyInstructions = { "dc","ds","org","end" };

    void ExtractExtraData(istringstream& ins);

    void ExtractOperand1(istringstream& ins);
    void ExtractOperand2(istringstream& ins);
};
