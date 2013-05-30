#include <gtkmm.h>
#include <gtkmm/main.h>

#include "masterwin.h"
#include "systools.h"
#include <iostream>
////////////////////////////////////////////////////////////////////////////////////////////////
// arg analysis
#include "arg_parser.h"
#include <cstdio>
#include <cstdlib>
void show_help();
void print_version();
void show_error( const char * const msg, const int errcode = 0,
				const bool help = false );
void internal_error( const char * const msg );
const char * invocation_name = 0;

// main
int main (int argc, char *argv[])
{
        // arg analysis
    invocation_name = argv[0];
	const Arg_parser::Option options[] =
	{
		{ 'v', "version",    Arg_parser::no    },
		{ 'h', "help",       Arg_parser::no    }};

	const Arg_parser parser( argc, argv, options );
	if( parser.error().size() )
	{ show_error( parser.error().c_str(), 0, true ); return BK_ERROR; }

	for( int argind = 0; argind < parser.arguments(); ++argind )
	{
		const int code = parser.code( argind );
		switch( code )
		{
		case 'v': print_version(); return 0;
		case 'h': show_help(); return 0;
		default : internal_error( "uncaught option" );
		}
	}
    string db, ip, user, psw;
    get_dbconfig(db, ip, user, psw);
    if(db_config::init(db.c_str(), ip.c_str(), user.c_str(), psw.c_str()) != BK_DB_SUCESS)
    {
        std::cout << "can't connect config" << std::endl;
        return -1;
    }

    Gtk::Main kit(argc, argv);
    MasterWin widebright;
    //Shows the window and returns when it is closed.
    Gtk::Main::run(widebright);
    return 0;
}

void show_help()
{
	printf( "\nUsage: %s [options] \n", invocation_name );
	printf( "\nOptions:\n"
		"  -h, --help                   display this help\n"
		"  -v  --version                print version number\n");
}

void print_version()
{
    printf("%s\n", VERSION);
}

void show_error( const char * const msg, const int errcode, const bool help )
{
	if( msg && msg[0] )
	{
		std::fprintf( stderr, "%s: %s", invocation_name, msg );
		if( errcode > 0 )
			std::fprintf( stderr, ": %s", std::strerror( errcode ) );
		std::fprintf( stderr, "\n" );
	}
	if( help && invocation_name && invocation_name[0] )
		std::fprintf( stderr, "Try '%s --help' for more information.\n",
		invocation_name );
}

void internal_error( const char * const msg )
{
	std::fprintf( stderr, "%s: internal error: %s.\n", invocation_name, msg );
	std::exit( 3 );
}
