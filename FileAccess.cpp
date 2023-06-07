//
//  Implementation of file access class.
//
#include "stdafx.h"
#include "FileAccess.h"

/**/
/*
FileAccess::FileAccess() FileAccess::FileAccess()

NAME

        FileAccess::FileAccess - constructor for FileAccess class.

SYNOPSIS

        FileAccess::FileAccess(int argc, char *argv[]);
            argc    --> the count of command line arguments.
            argv[]  --> the array of command line arguments.

DESCRIPTION

        The constructor checks if there is exactly one runtime parameter, opens the file,
        and reports an error if the file cannot be opened.

RETURNS

        N/A

AUTHOR

        Saurav Dahal

DATE

        5:15pm 5/5/2023

*/
/**/
FileAccess::FileAccess( int argc, char *argv[] )
{
    // Check that there is exactly one run time parameter.
    if( argc != 2 ) {
        cerr << "Usage: Assem <FileName>" << endl;
        exit( 1 );
    }
    // Open the file.  One might question if this is the best place to open the file.
    // One might also question whether we need a file access class.
    m_sfile.open( argv[1], ios::in );

    // If the open failed, report the error and terminate.
    if( ! m_sfile ) {
        cerr << "Source file could not be opened, assembler terminated."
            << endl;
        exit( 1 ); 
    }
}
/*FileAccess::FileAccess( int argc, char *argv[] );*/



/**/
/*
FileAccess::~FileAccess() FileAccess::~FileAccess()

NAME

        FileAccess::~FileAccess - destructor for FileAccess class.

SYNOPSIS

        FileAccess::~FileAccess();

DESCRIPTION

        The destructor closes the file. This is not necessary since the file will be closed
        when the program terminates, but it is good form.

RETURNS

        N/A

AUTHOR

        Saurav Dahal

DATE

        5:15pm 5/5/2023

*/
/**/
FileAccess::~FileAccess( )
{
    // Not that necessary in that the file will be closed when the program terminates, but good form.
    m_sfile.close( );
}
/*FileAccess::~FileAccess( );*/



/**/
/*
FileAccess::GetNextLine() FileAccess::GetNextLine()

NAME

        FileAccess::GetNextLine - gets the next line from the file.

SYNOPSIS

        bool FileAccess::GetNextLine(string &a_line);
            a_line   --> reference to the string where the next line will be stored.

DESCRIPTION

        This function gets the next line from the file and stores it in the given string.
        If there is no more data, it returns false.

RETURNS

        Returns true if the next line is successfully retrieved, otherwise returns false.

AUTHOR

        Saurav Dahal

DATE

        5:15pm 5/5/2023

*/
/**/
bool FileAccess::GetNextLine( string &a_line )
{
    // If there is no more data, return false.
    if( m_sfile.eof() ) {
    
        return false;
    }
    getline( m_sfile, a_line );
    
    // Return indicating success.
    return true;
}
/*bool FileAccess::GetNextLine( string &a_line );*/


/**/
/*
FileAccess::rewind() FileAccess::rewind()

NAME

        FileAccess::rewind - resets the file to the beginning.

SYNOPSIS

        void FileAccess::rewind();

DESCRIPTION

        This function clears all file flags and goes back to the beginning of the file.

RETURNS

        void

AUTHOR

        Saurav Dahal

DATE

        5:15pm 5/5/2023

*/
/**/
void FileAccess::rewind( )
{
    // Clean all file flags and go back to the beginning of the file.
    m_sfile.clear();
    m_sfile.seekg( 0, ios::beg );
}
/*void FileAccess::rewind( )*/
