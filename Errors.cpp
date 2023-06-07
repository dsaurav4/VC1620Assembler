// Errors.cpp

#include "stdafx.h"
#include "Errors.h"

// Define the static m_ErrorMsgs vector
vector<string> Errors::m_ErrorMsgs;

// Define the static ErrorCount integer
int Errors::errorCount;


/**/
/*
Errors::InitErrorReporting() Errors::InitErrorReporting()

NAME

        Errors::InitErrorReporting - initializes error reporting.

SYNOPSIS

        void Errors::InitErrorReporting();

DESCRIPTION

        This function initializes error reporting by clearing the error messages vector
        and setting the error count to 0.

RETURNS

        void

AUTHOR

        Saurav Dahal

DATE

        5:15pm 5/5/2023

*/
/**/
void Errors::InitErrorReporting()
{
    m_ErrorMsgs.clear();
    errorCount = 0;
}
/*void Errors::InitErrorReporting();*/


/**/
/*
Errors::RecordError() Errors::RecordError()

NAME

        Errors::RecordError - records an error message.

SYNOPSIS

        void Errors::RecordError(string a_emsg);
            a_emsg   --> the error message to be recorded.

DESCRIPTION

        This function records an error message by adding it to the m_ErrorMsgs vector
        and incrementing the error count.

RETURNS

        void

AUTHOR

        Saurav Dahal

DATE

        5:15pm 5/5/2023

*/
/**/
void Errors::RecordError(string a_emsg)
{
    m_ErrorMsgs.push_back(a_emsg);
    errorCount++;
}
/*void Errors::RecordError(string a_emsg);*/


/**/
/*
Errors::DisplayErrors() Errors::DisplayErrors()

NAME

        Errors::DisplayErrors - displays the collected error messages.

SYNOPSIS

        void Errors::DisplayErrors();

DESCRIPTION

        This function displays the collected error messages. If there are no error messages,
        it displays "No errors detected." If there are error messages, it displays them one
        by one and then clears the m_ErrorMsgs vector.

RETURNS

        void

AUTHOR

        Saurav Dahal

DATE

        5:15pm 5/5/2023

*/
/**/
void Errors::DisplayErrors()
{
    // If there are no error messages, display "No errors detected."
    if(!m_ErrorMsgs.empty())
    {
        // If there are error messages, display them one by one
        for (const auto& errorMsg : m_ErrorMsgs)
        {
            cout << "!!!ERROR: " << errorMsg << endl;
        }
        m_ErrorMsgs.clear();
    }
}
/*void Errors::DisplayErrors();*/


/**/
/*
Errors::IsEmpty() Errors::IsEmpty()

NAME

        Errors::IsEmpty - checks if there are no recorded error messages.

SYNOPSIS

        bool Errors::IsEmpty();

DESCRIPTION

        This function checks if there are no recorded error messages by comparing the error
        count to 0.

RETURNS

        Returns true if there are no recorded error messages, otherwise returns false.

AUTHOR

        Saurav Dahal

DATE

        5:15pm 5/5/2023

*/
/**/
bool Errors::IsEmpty() {
    return errorCount == 0;
}
/*bool Errors::IsEmpty();*/