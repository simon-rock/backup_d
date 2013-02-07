#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include  <string>
#include <cstdio>
#include <cstdlib>
#include <string.h>
using std::string;
////////////////////////////////////////////////////////////////////////////////////////////////
// arg analysis
#include "com_inc.h"
#include "arg_parser.h"
void show_help();
void print_version();
void show_error( const char * const msg, const int errcode = 0,
				const bool help = false );
void internal_error( const char * const msg );
const char * invocation_name = 0;

// cmd hand
enum _cmdtype
{
	CMD_VIEW = 0,
	CMD_START,
	CMD_STOP,
	CMD_STOP_SVRD,
	CMD_NO
};

void list_backup_status();
void control_backup(string& _cmd);
// main
int main(int argc, char *argv[])
{
	string strcmd;
	_cmdtype t = CMD_NO;
    // arg analysis
    invocation_name = argv[0];
	const Arg_parser::Option options[] =
	{
		{ 'v', "version",    Arg_parser::no    },
		{ 'h', "help",       Arg_parser::no    },
		{ 'l', "list",       Arg_parser::no    },
		{ 256, "start",      Arg_parser::yes    },
		{ 257, "stop",       Arg_parser::yes    },
		{ 258, "stopbackup", Arg_parser::no    }};

	const Arg_parser parser( argc, argv, options );
	if( parser.error().size() )
	{ show_error( parser.error().c_str(), 0, true ); return BK_ERROR; }

	for( int argind = 0; argind < parser.arguments(); ++argind )
	{
		if (t != CMD_NO)
		{
			break;
		}
		const int code = parser.code( argind );
		const char * arg = parser.argument( argind ).c_str();
		switch( code )
		{
		case 'h': show_help(); return 0;
		case 'v': print_version(); return 0;
		case 'l': t = CMD_VIEW; break;
		case 256: if( arg[0] ) {t = CMD_START; strcmd += "0:";strcmd += arg;} break;
		case 257: if (arg[0] ) {t = CMD_STOP; strcmd += "1:";strcmd += arg;} break;
		case 258: {t = CMD_STOP_SVRD; strcmd += "9:";} break;
		default : internal_error( "uncaught option" );
		}
	} 
	switch(t)
	{
		case CMD_VIEW:
			list_backup_status();
			break;
		case CMD_START:
			control_backup(strcmd);
			break;
		case CMD_STOP:
			control_backup(strcmd);
			break;
		case CMD_STOP_SVRD:
			control_backup(strcmd);
			break;
		case CMD_NO:
			show_help();
			break;
		default:
			show_help();
			break;
	}
    return 0;
}

void list_backup_status()
{
	char r_buf[4096];

	memset(r_buf,0,sizeof(r_buf));
	int  fd=open(FIFOVIEW,O_RDONLY,0);
	if(fd==-1)
	{
		printf("open %s for read error\n", FIFOVIEW);
		exit(0);	
	}
	while(1)
	{
		memset(r_buf,0,sizeof(r_buf));
		int ret_size=read(fd,r_buf,4000);
		if(ret_size==-1 || ret_size == 0)
			break;
		printf("%s", r_buf);
		sleep(1);
	}
}
void control_backup(string& _cmd)
{
	if((mkfifo(FIFOCONTROL,O_CREAT|O_EXCL)<0)&&(errno!=EEXIST))
		printf("cannot create control\n");
	int fd=open(FIFOCONTROL,O_WRONLY,0);
	if(fd==-1)
	{
		printf("open error\n");
		return;
	}
	int real_wnum=write(fd,_cmd.c_str(),_cmd.length());
	if(real_wnum==-1)
	{
		printf("send cmd error\n");
	}
}

void show_help()
{
	printf( "\nUsage: %s [options] \n", invocation_name );
	printf( "\nOptions:\n"
		"  -h, --help                   display this help\n"
		"  -v  --version                print version number\n"
		"  -l  --list	                print backup status\n"
		"      --start <brick id>       start brick backup\n"
		"      --stop  <brick id>       stop brick backup\n"
		"	   --stopbackup             stop backup server");
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
			std::fprintf( stderr, ": %s", strerror( errcode ) );
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
