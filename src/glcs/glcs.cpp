/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/glcs/glsc.cpp
 * Project:     Glich: Extendable Script Language.
 * Purpose:     Glich Script interactive app glcs source file
 * Author:      Nick Matthews
 * Website:     https://github.com/nickmat/glich
 * Created:     8th February 2023
 * Copyright:   Copyright (c) 2023, Nick Matthews.
 * Licence:     GNU GPLv3
 *
 *  Glich is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Glich is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Glich.  If not, see <http://www.gnu.org/licenses/>.
 *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

*/

#include <glc/glc.h>

#include "glcsresource.h"


#include <iostream>
#include <fstream>
#include <sstream>


#ifdef _WIN32
#include <Windows.h>
#include <Wincon.h>
#endif

#define VERSION   "0.1.0"
#define PROGNAME  " (Glich Script)"
#define COPYRIGHT  "2023 Nick Matthews"

const char* g_version = VERSION;
const char* g_progName = PROGNAME;
const char* g_copyright = "Copyright (c) " COPYRIGHT "\n";

#if defined(NDEBUG) || !defined(_DEBUG)
const char* g_title = PROGNAME " - Version " VERSION "\n";
#else
const char* g_title = PROGNAME " - Version " VERSION " Debug\n";
#endif


namespace hg = glich;
using std::string;
using std::vector;


enum class StmtType { semicolon, curlybracket, both, if_endif, do_loop };

string left_trim( const string& str )
{
    size_t pos = 0;
    while( str.find( ' ', pos ) == pos ) {
        pos++;
    }
    if( pos != string::npos ) {
        return str.substr( pos );
    }
    return "";
}

string get_first_word( const string& str, string* tail, char sep )
{
    size_t pos = str.find( sep );
    string result = str.substr( 0, pos );
    if( tail ) {
        *tail = ( pos == string::npos ) ? "" : left_trim( str.substr( pos + 1 ) );
    }
    return result;
}

string read_file( const string& name )
{
    std::ifstream in( name.c_str() );
    std::stringstream ss;
    ss << in.rdbuf();
    return ss.str();
}

void do_usage( bool glich_prog )
{
    char* prog = glich_prog ? "glich" : "glcs";
    char* i_default = glich_prog ? " (Default)" : "";
    char* e_default = glich_prog ? "" : " (Default)";
    if( glich_prog )
    std::cout << prog << g_title << g_copyright << "\n";
    std::cout << 
        "Usage:\n"
        "  " << prog << " [options]\n"
        "\n"
        "Options:\n"
        "  -h    Show this help message and exit.\n"
        "  -e    End after running scripts." << i_default << "\n"
        "  -i    Enter interactive mode." << e_default << "\n"
        "  name  Run the file 'name' as a script.\n"
        "        Multiple files are run in the order that they appear.\n" 
        "\n"
    ;
}

void do_help( const string& option, bool glich_prog )
{
    if ( option == "usage" ) {
        do_usage( glich_prog );
        return;
    }
    std::cout <<
        "\n"
        "Welcome to the Glich interactive program.\n"
        "\n"
        "Briefly, a script consists of one or more statements. These statements are\n"
        "detailed in the online manual at http://historycal.org/man/script/. A single\n"
        "statements is entered, if necessary over a number of lines, and it will be\n"
        "executed as soon as the statement is completed.\n"
        "\n"
        "In addition to the statements, the following commands are available: 'help',\n"
        "'info', 'run' and 'end'. These are used as follows.\n"
        "\n"
        " help             Display this text.\n"
        " help usage       Display the command line details.\n"
        //" help <statement> Display help for the given statement.\n"
        " info             List all the currently defined schemes.\n"
        " info <scheme>    Display scheme details of the given scheme code.\n"
        " run <filename>   Import and run the script in the named file.\n"
        " end              Exit the program.\n"
        "\n"
        "If what you enter is neither a named statement or a command then, if it ends\n"
        "with a ';' character, it is treated as an assignment statement. Otherwise it is\n"
        "treated as an expression, wrapped within a 'write' statement and executed.\n"
        "\n"
    ;
}

string compress_statement( const string& statement )
{
    string stmt;
    bool in_quote = false;
    bool in_peren = false;
    int brace_cnt = 0;
    bool start_comment = false;
    bool end_comment = false;
    bool in_lcomment = false;
    bool in_mcomment = false;

    for ( auto ch : statement ) {
        if ( in_lcomment ) {
            if ( ch == '\n' ) {
                in_lcomment = false;
            }
        } else if(in_mcomment ){
            if ( end_comment ) {
                if ( ch == '/' ) {
                    in_mcomment = false;
                }
            }
            end_comment = ( ch == '*' );
        } else if ( in_quote ) {
            if ( ch == '"' ) {
                stmt += ' ';
                in_quote = false;
            }
        } else if ( start_comment ) {
            if ( ch == '/' ) {
                in_lcomment = true;
            } else if ( ch == '*' ) {
                in_mcomment = true;
            } else {
                stmt += '/';
                stmt += ch;
            }
            start_comment = false;
        } else if ( ch == '/' ) {
            start_comment = true;
        } else if ( in_peren ) {
            if ( ch == ')' ) {
                stmt += ' ';
                in_peren = false;
            }
        } else if ( brace_cnt > 1 ) {
            if ( ch == '{' ) {
                brace_cnt++;
            } else if ( ch == '}' ) {
                --brace_cnt;
                if ( brace_cnt == 1 ) {
                    stmt += ' ';
                }
            }
        } else if ( ch == '"' ) {
            in_quote = true;
        } else if ( ch == '(' ) {
            in_peren = true;
        } else if ( ch == '{' ) {
            brace_cnt++;
            if ( brace_cnt == 1 ) {
                stmt += ch;
            }
        } else {
            stmt += ch;
        }
    }
    return stmt;
}

bool terminated_char( const string& stmt, int character )
{
    if( stmt.empty() ) {
        return false;
    }
    string input = stmt;
    input.erase( std::remove( input.begin(), input.end(), ' ' ), input.end() );
    int ch = stmt.back();
    return (ch == character);
}
bool terminated_semicolon( const string& statement )
{
    string stmt = compress_statement( statement );
    return terminated_char( stmt, ';' );
}

bool terminated_curlybracket( const string& statement )
{
    string stmt = compress_statement( statement );
    return terminated_char( stmt, '}' );
}

bool terminated_both( const string& statement )
{
    string stmt = compress_statement( statement );
    for ( auto ch : stmt ) {
        if ( ch == '{' ) {
            return terminated_char( stmt, '}' );
        }
    }
    return terminated_char( stmt, ';' );
}

bool terminated_word( const string& statement, const string& end )
{
    string stmt = compress_statement( statement );
    string start = get_first_word( stmt, nullptr, ' ' );
    int count = 0;
    string word;
    bool in_word = false;
    bool is_funct = false;
    for ( auto ch : stmt ) {
        if ( in_word ) {
            if ( isalnum( ch ) || ch == '_' || ch == ':' || ch == '.' ) {
                word += ch;
            } else {
                if ( is_funct ) {
                    is_funct = false;
                } else if ( word == start ) {
                    count++;
                } else if ( word == end ) {
                    if ( count > 1 ) {
                        --count;
                    } else {
                        return true;
                    }
                }
                in_word = false;
            }
        } else if ( isascii( ch ) && ( isalpha( ch ) || ch == '_' || ch == ':' ) ) {
            in_word = true;
            word = ch;
        } else if ( ch == '@' ) {
            is_funct = true;
        }
    }
    return false;
}

string get_statement( const string& start, StmtType type )
{
    string statement = start + "\n";
    for ( int lnum = 2;; lnum++ ) {
        string prompt = std::to_string( lnum );
        while ( prompt.size() < 4 ) {
            prompt += ".";
        }
        prompt += ":";
        std::cout << prompt << " ";
        string line;
        std::getline( std::cin, line );
        line = left_trim( line );
        if ( line.empty() ) {
            break;
        }
        statement += line + "\n";
        switch ( type )
        {
        case StmtType::semicolon:
            if ( terminated_semicolon( statement ) ) {
                return statement;
            }
            break;
        case StmtType::curlybracket:
            if ( terminated_curlybracket( statement ) ) {
                return statement;
            }
            break;
        case StmtType::both:
            if ( terminated_both( statement ) ) {
                return statement;
            }
            break;
        case StmtType::if_endif:
            if ( terminated_word( statement, "endif" ) ) {
                return statement;
            }
            break;
        case StmtType::do_loop:
            if ( terminated_word( statement, "loop" ) ) {
                return statement;
            }
            break;
        default:
            return "";
        }
    }
    return statement;
}

int main( int argc, char* argv[] )
{
#ifdef _WIN32
    // Set Windows console in/out to use utf-8.
    SetConsoleCP( CP_UTF8 );
    setvbuf( stdin, nullptr, _IOFBF, 1000 );
    SetConsoleOutputCP( CP_UTF8 );
    setvbuf( stdout, nullptr, _IOFBF, 1000 );
#endif
    hg::init_glc( hg::InitLibrary::Hics );
    vector<string> filenames;
    bool run_default = true;

    bool interactive = true;
    bool glich_prog = false;
    std::string prog( argv[0] );
    if( prog.size() >= 9 && prog.substr( prog.size() - 9 ) == "glich.exe" ) {
        interactive = false;
        glich_prog = true;
    }
    else if( prog.size() >= 5 && prog.substr( prog.size() - 5 ) == "glich" ) {
        interactive = false;
        glich_prog = true;
    }

    for( int i = 1; i < argc; i++ ) {
        if( argv[i][0] == '-' ) {
            switch( argv[i][1] )
            {
            case 'h': // Help
                do_usage( glich_prog );
                hg::exit_glc();
                return 0;
            case 'n': // No default script.
                run_default = false;
                break;
            case 'e': // Exit without running command line.
                interactive = false;
                break;
            case 'i': // Run command line.
                interactive = true;
                break;
            default:
                std::cout << "Command line flag not recognised.\n";
            }
        }
        else {
            filenames.push_back( string( argv[i] ) );
        }
    }

    if( interactive ) {
        char* prog = glich_prog ? "glich" : "glcs";
        std::cout << prog << g_title << g_copyright <<
            "Enter 'help' for more information.\n"
            "Enter 'end' to exit program.\n"
            "\n"
            ;
    }

    // Run script files if given.
    for( size_t i = 0; i < filenames.size(); i++ ) {
        string script = read_file( filenames[i] );
        string response = hg::get_glc()->run_script( script );
        if( response.size() ) {
            std::cout << response << "\n";
        }
    }

    if( interactive ) {
        for( ;;) {
            std::cout << "glcs: ";
            string cmnd;
            std::getline( std::cin, cmnd );
            string word, tail;
            word = get_first_word( cmnd, &tail, ' ' );

            if( word == "end" ) {
                break;
            }
            else if( word == "help" ) {
                do_help( tail, glich_prog );
                continue;
            }
            else if( word == "run" ) {
                cmnd = read_file( tail );
            }
            else if(
                word == "let" || word == "set" || word == "write"
                || word == "writeln" || word == "mark" || word == "clear"
                || word == "call" )
            {
                if( !terminated_semicolon( tail ) ) {
                    cmnd = get_statement( cmnd, StmtType::semicolon );
                }
            }
            else if(
                word == "function" || word == "command" || word == "object" ||
                word == "scheme" || word == "grammar" || word == "format" || word == "lexicon" )
            {
                if( !terminated_curlybracket( tail ) ) {
                    cmnd = get_statement( cmnd, StmtType::curlybracket );
                }
            }
            else if( word == "if" ) {
                if( !terminated_word( cmnd, "endif" ) ) {
                    cmnd = get_statement( cmnd, StmtType::if_endif );
                }
            }
            else if( word == "do" ) {
                if( !terminated_word( cmnd, "loop" ) ) {
                    cmnd = get_statement( cmnd, StmtType::do_loop );
                }
            }
            else {
                if( !terminated_semicolon( cmnd ) && !cmnd.empty() ) {
                    cmnd = "let answer = " + cmnd + "; write answer//*/\n;";
                }
            }
            string output = hg::get_glc()->run_script( cmnd );
            if( output.size() ) {
                std::cout << output << "\n";
            }
        }
    }
    else if( filenames.empty() ) {
        do_usage( glich_prog );
    }
    hg::exit_glc();
    return 0;
}


// End of src/glcs/glsc.cpp
