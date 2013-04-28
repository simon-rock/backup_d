#include <gtkmm/main.h>
#include "backup_main.h"
#include "deamon.h"
#include "systools.h"
////////////////////////////////////////////////////////////////////////////////////////////////
// arg analysis
#include "arg_parser.h"
void show_help();
void print_version();
void show_error( const char * const msg, const int errcode = 0,
				const bool help = false );
void internal_error( const char * const msg );
const char * invocation_name = 0;

extern bool _running;
// main
int main(int argc, char *argv[])
{
    // arg analysis
    invocation_name = argv[0];
    bool bdeamon = false;
	const Arg_parser::Option options[] =
	{
		{ 'v', "version",    Arg_parser::no    },
		{ 'h', "help",       Arg_parser::no    },
		{ 256, "deamon",     Arg_parser::no    }};

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
		case 256: bdeamon = true; break;
		default : internal_error( "uncaught option" );
		}
	}
    if(bdeamon)
    {
        setdeamon();
        string db, ip, user, psw;
        get_dbconfig(db, ip, user, psw);
        if (backup_init(db.c_str(), ip.c_str(), user.c_str(), psw.c_str()) != 0)
        //if (backup_init("backup", "127.0.0.1", "root", "root") != 0)
        {
            cout << "init backupd error "<< endl;
            return 0;
        }
        backup_start();
        while(_running) sleep(10);
    }
    else
    {
        Gtk::Main kit(argc, argv);
        BackupMain window;
    //Shows the window and returns when it is closed.
        Gtk::Main::run(window);
        backup_stop();
    }

    return 0;
}

void show_help()
{
	printf( "\nUsage: %s [options] \n", invocation_name );
	printf( "\nOptions:\n"
		"  -h, --help                   display this help\n"
		"  -v  --version                print version number\n"
		"      --deamon                 run as deamon mode\n");
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
