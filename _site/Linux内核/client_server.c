#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/tcp.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

#include <netdb.h>

#define SERVER_ADDRESS "127.0.0.7"
#define SERVER_PORT 4777


#define myfail_if( test, msg ) do { if((test)){ fprintf(stderr, msg "\n"); exit(1); } } while (0)
#define myfail_unless( test, msg ) myfail_if( !(test), msg )

int connect_client( char *addr, int actual_port )
{
    int client_fd;

    struct addrinfo hint;
    struct addrinfo *ailist, *aip;


    memset( &hint, '\0', sizeof( struct addrinfo ) );
    hint.ai_socktype = SOCK_STREAM;

    myfail_if( getaddrinfo( addr, NULL, &hint, &ailist ) != 0, "getaddrinfo failed." );

    int connected = 0;
    for( aip = ailist; aip; aip = aip->ai_next ) {
        ((struct sockaddr_in *)aip->ai_addr)->sin_port = htons( actual_port );
        client_fd = socket( aip->ai_family, aip->ai_socktype, aip->ai_protocol );

        if( client_fd == -1) { continue; }
        if( connect( client_fd, aip->ai_addr, aip->ai_addrlen) == 0 ) {
            connected = 1;
            break;
        }
        close( client_fd );
    }

    freeaddrinfo( ailist );

    myfail_unless( connected, "Didn't connect." );
    return client_fd;
}


void client(){
    sleep(1);
    int client_fd = connect_client( SERVER_ADDRESS, SERVER_PORT );

    printf("Client closing its fd... ");
    myfail_unless( 0 == close( client_fd ), "close failed" );
    fprintf(stdout, "Client exiting.\n");
    exit(0);
}


int init_server( struct sockaddr * saddr, socklen_t saddr_len )
{
    int sock_fd;

    sock_fd = socket( saddr->sa_family, SOCK_STREAM, 0 );
    if ( sock_fd < 0 ){
        return sock_fd;
    }

    myfail_unless( bind( sock_fd, saddr, saddr_len ) == 0, "Failed to bind." );
    return sock_fd;
}

int start_server( const char * addr, int port )
{
    struct addrinfo *ailist, *aip;
    struct addrinfo hint;
    int sock_fd;

    memset( &hint, '\0', sizeof( struct addrinfo ) );
    hint.ai_socktype = SOCK_STREAM;
    myfail_if( getaddrinfo( addr, NULL, &hint, &ailist ) != 0, "getaddrinfo failed." );

    for( aip = ailist; aip; aip = aip->ai_next ){
        ((struct sockaddr_in *)aip->ai_addr)->sin_port = htons( port );
        sock_fd = init_server( aip->ai_addr, aip->ai_addrlen );
        if ( sock_fd > 0 ){
            break;
        } 
    }
    freeaddrinfo( aip );

    myfail_unless( listen( sock_fd, 2 ) == 0, "Failed to listen" );
    return sock_fd;
}


int server_accept( int server_fd )
{
    printf("Accepting\n");
    int client_fd = accept( server_fd, NULL, NULL );
    myfail_unless( client_fd > 0, "Failed to accept" );
    return client_fd;
}


void server() {
    int server_fd = start_server(SERVER_ADDRESS, SERVER_PORT);
    int client_fd = server_accept( server_fd );

    printf("Server sleeping\n");
    sleep(60);

    printf( "Errno before: %s\n", strerror( errno ) );
    printf( "Write result: %d\n", write( client_fd, "123", 3 ) );
    printf( "Errno after:  %s\n", strerror( errno ) );

    close( client_fd );
}


int main(void){
    pid_t clientpid;
    pid_t serverpid;

    clientpid = fork();

    if ( clientpid == 0 ) {
        client();
    } else {
        serverpid = fork();

        if ( serverpid == 0 ) {
            server();
        }
        else {
            int clientstatus;
            int serverstatus;

            waitpid( clientpid, &clientstatus, 0 );
            waitpid( serverpid, &serverstatus, 0 );

            printf( "Client status is %d, server status is %d\n", 
                    clientstatus, serverstatus );
        }
    }

    return 0;
}