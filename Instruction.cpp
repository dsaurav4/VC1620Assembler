#include "stdafx.h"
#include "Instruction.h"
#include "Errors.h"


/**/
/*
Instruction::clearInstructionElements() Instruction::clearInstructionElements()

NAME

        Instruction::clearInstructionElements - clears instruction elements.

SYNOPSIS

        void Instruction::clearInstructionElements();

DESCRIPTION

        This function clears all instruction elements by setting them to their default values.

RETURNS

        void

AUTHOR

        Saurav Dahal

DATE

        5:15pm 5/5/2023

*/
/**/
void Instruction::clearInstructionElements()
{
     m_Label = "";
     m_OpCode = "";
     m_Operand1 = "";
     m_Operand2 = "";
     m_instruction = "";
     m_extra = "";
     m_NumOpCode = 0;     
     m_type = ST_Illegal; 
     m_OperandValue  = 0;    
     m_IsNumericOperand1 = false;
     m_IsNumericOperand2 = false;
}
/*void Instruction::clearInstructionElements();*/


/**/
/*
Instruction::ParseInstruction() Instruction::ParseInstruction()

NAME

        Instruction::ParseInstruction - parses an instruction from a given line.

SYNOPSIS

        Instruction::InstructionType Instruction::ParseInstruction(string a_line);
            a_line  --> the line to parse the instruction from.

DESCRIPTION

        This function parses an instruction from a given line and sets the appropriate
        instruction elements accordingly. It returns the instruction type.

RETURNS

        Returns the instruction type as an Instruction::InstructionType enumeration.

AUTHOR

        Saurav Dahal

DATE

        5:15pm 5/5/2023

*/
/**/
Instruction::InstructionType Instruction::ParseInstruction(string a_line)
{
    clearInstructionElements();

    m_instruction = a_line;

    RemoveComment(a_line);

    if (a_line.empty() || all_of(a_line.begin(), a_line.end(), isspace)) {
        m_type = ST_Comment;
        return m_type;
    }

    replace(a_line.begin(), a_line.end(), ',', ' ');
    istringstream ins(a_line);

    if (a_line[0] != ' ' && a_line[0] != '\t')
    {
        ins >> m_Label;
    }
    ins >> m_OpCode;

    transform(m_OpCode.begin(), m_OpCode.end(), m_OpCode.begin(), [](unsigned char c) { return tolower(c); }); //converting opcode to lowercase

    // Handle different instruction formats
    if (m_machineInstrunctions.count(m_OpCode)) //checking for machine language instructions
    {
        m_NumOpCode = m_machineInstrunctions[m_OpCode];

        if (m_OpCode == "halt")
        {
            m_type = ST_MachineLanguage;
            ExtractExtraData(ins);
            return m_type;
        }

        ExtractOperand1(ins);

        if ((m_NumOpCode >= 1 && m_NumOpCode <= 5) || (m_NumOpCode >= 10 && m_NumOpCode <= 12)) //checking if it's an instruction that requires two operands
        {
            ExtractOperand2(ins);

        }
        m_type = ST_MachineLanguage;

        ExtractExtraData(ins);

        return m_type;
    }

    else if (find(m_assemblyInstructions.begin(), m_assemblyInstructions.end(), m_OpCode) != m_assemblyInstructions.end())
    {
        if (m_OpCode == "end") 
        {
            ExtractExtraData(ins);
            return ST_End;
        }

        ExtractOperand1(ins);

        ExtractExtraData(ins);

        m_type = ST_AssemblerInstr;
        return m_type;
    }

    else
    {
        ExtractOperand1(ins);

        ExtractOperand2(ins);

        ExtractExtraData(ins);

        return m_type; // Invalid m_OpCode
    }
}
/*Instruction::InstructionType Instruction::ParseInstruction(string a_line);*/



/**/
/*
Instruction::ExtractExtraData() Instruction::ExtractExtraData()

NAME

        Instruction::ExtractExtraData - extracts extra data from the input stream.

SYNOPSIS

        void Instruction::ExtractExtraData(istringstream& ins);
            ins  --> the input stream to extract extra data from.

DESCRIPTION

        This function extracts extra data from the input stream and stores it in the m_extra member.

RETURNS

        void

AUTHOR

        Saurav Dahal

DATE

        5:15pm 5/5/2023

*/
/**/
void Instruction::ExtractExtraData(istringstream& ins) {
    if (ins.peek() != EOF) {
        ins >> ws;
        if (ins.peek() != EOF) {
            string temp(istreambuf_iterator<char>(ins), {});
            m_extra = temp;
        }
    }
}
/*void Instruction::ExtractExtraData(istringstream& ins);*/



/**/
/*
Instruction::ExtractOperand1() Instruction::ExtractOperand1()

NAME

        Instruction::ExtractOperand1 - extracts the first operand from the input stream.

SYNOPSIS

        void Instruction::ExtractOperand1(istringstream& ins);
            ins  --> the input stream to extract the first operand from.

DESCRIPTION

        This function extracts the first operand from the input stream and sets the
        m_Operand1 and m_IsNumericOperand1 members accordingly.

RETURNS

        void

AUTHOR

        Saurav Dahal

DATE

        5:15pm 5/5/2023

*/
/**/
void Instruction::ExtractOperand1(istringstream& ins) {
    if (ins.peek() != EOF)
    {
        ins >> ws;
        if (ins.peek() != EOF)
        {
            ins >> m_Operand1;
            if (all_of(m_Operand1.begin(), m_Operand1.end(), ::isdigit))
            {
                m_IsNumericOperand1 = true;
                m_OperandValue = stoll(m_Operand1);
            }
        }
    }
}
/*void Instruction::ExtractOperand1(istringstream& ins);*/


/**/
/*
Instruction::ExtractOperand2() Instruction::ExtractOperand2()

NAME

        Instruction::ExtractOperand2 - extracts the second operand from the input stream.

SYNOPSIS

        void Instruction::ExtractOperand2(istringstream& ins);
            ins  --> the input stream to extract the second operand from.

DESCRIPTION

        This function extracts the second operand from the input stream and sets the
        m_Operand2 and m_IsNumericOperand2 members accordingly.

RETURNS

        void

AUTHOR

        Saurav Dahal

DATE

        5:15pm 5/5/2023

*/
/**/
void Instruction::ExtractOperand2(istringstream& ins) {
    if (ins.peek() != EOF)
    {
        ins >> ws;
        if (ins.peek() != EOF)
        {
            ins >> m_Operand2;
            if (all_of(m_Operand2.begin(), m_Operand2.end(), ::isdigit))
            {
                m_IsNumericOperand2 = true;
                m_OperandValue = stoll(m_Operand2);
            }
        }
    }
}
/*void Instruction::ExtractOperand2(istringstream& ins);*/



/**/
/*
Instruction::LocationNextInstruction() Instruction::LocationNextInstruction()

NAME

        Instruction::LocationNextInstruction - calculates the location of the next instruction.

SYNOPSIS

        int Instruction::LocationNextInstruction(int a_loc);
            a_loc  --> the current location of the instruction.

DESCRIPTION

        This function calculates the location of the next instruction based on the current
        instruction's opcode and operand(s). It takes into account special opcodes like
        "org" and "ds" which affect the location of the next instruction.

RETURNS

        Returns an integer representing the location of the next instruction.

AUTHOR

        Saurav Dahal

DATE

        5:15pm 5/5/2023

*/
/**/
int Instruction::LocationNextInstruction(int a_loc) {

    // If the opcode is "org", then check if there is an integer provided. If it is provided,
    // then the next instruction will be set to the integer. Else, set it to 100 by default.
    if (m_OpCode == "org")
    {
        if (IsOperand1Numeric()) return stoi(m_Operand1);
        return a_loc + 1;
    }



    // If the opcode is "ds", then check if there is an integer provided. If it is provided,
    // then the location of next instruction will be determined by 
    // incrementing the current location with the value of the integer. Else, return the location of next instruction by
    // incrementing the current location by 1.
    if (m_OpCode == "ds")
    {
        if(IsOperand1Numeric()) return a_loc + stoi(m_Operand1);
        return a_loc+1;
    }
    
    return a_loc + 1;
}
/*int Instruction::LocationNextInstruction(int a_loc);*/