#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>

int main(int argc, char* argv[])
{
    int sockfd, servlen;
    struct sockaddr_un serv_addr;

    const char *sun_path = "socket_unix";

    memset( (char *)&serv_addr, '\0', sizeof(serv_addr) );
	serv_addr.sun_family = AF_UNIX;
	strcpy( serv_addr.sun_path, sun_path );
	servlen = strlen( serv_addr.sun_path) + sizeof(serv_addr.sun_family);

    if ((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
		perror( "create" );
		exit( 1 );
	}


 	if (connect( sockfd, (struct sockaddr *)&serv_addr, servlen ) < 0) {
		perror( "connect" );
		exit( 1 );
	}
}