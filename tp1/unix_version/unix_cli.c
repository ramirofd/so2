#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#define   COMM_SIZE   10
#define   BUFF_SIZE   1461
#define   KNRM        "\x1B[0m"
#define   KRED        "\x1B[31m"
#define   KGRN        "\x1B[32m"
#define   KBLU        "\x1B[34m"

int parse_command(char* command);
int start_scanning(int* sockfd);
int update_firmware(int* sockfd);
int get_telemetry(int* sockfd);

int main(int argc, char* argv[])
{
  int sockfd, servlen, n;
  struct sockaddr_un serv_addr;

  const char *sun_path = "socket_unix";

  char command[COMM_SIZE];

  memset( (char *)&serv_addr, '\0', sizeof(serv_addr) );
	serv_addr.sun_family = AF_UNIX;
	strcpy( serv_addr.sun_path, sun_path );
	servlen = strlen( serv_addr.sun_path) + sizeof(serv_addr.sun_family);

  if ((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) 
  {
    perror("create");
    exit(1);	
	}

 	if (connect( sockfd, (struct sockaddr *)&serv_addr, servlen ) < 0) 
  {
		perror("connect");
		exit(1);
	}
  while(1)
    {
      memset(command, '\0', COMM_SIZE);
      n = read(sockfd, command, COMM_SIZE);
      if (n < 0) 
      {
        perror("read");
        exit(1);
      }
      switch(parse_command(command)){
        case 1:
          update_firmware(&sockfd);
          break;
        case 2:
          get_telemetry(&sockfd);
          break;
        case 3:
          start_scanning(&sockfd);
          break;
        case -1:
          // por ahora nada
          break;
      }
    }
}

int parse_command(char* command)
{
  if(!strcmp(command, "update"))
    return 1;

  if(!strcmp(command, "telemetry"))
    return 2;
  
  if(!strcmp(command, "scanning"))
    return 3;

  return -1;
}

int start_scanning(int* sockfd)
{
  printf("scanning process...\n");
  
    
  return 0;
}

int update_firmware(int* sockfd)
{
  int n;
  int file_size;
  // char buffer[BUFF_SIZE];
  
  printf("*** Update ***\n");
  n = write(*sockfd, "ok", 2);
  if (n < 0) 
  {
    perror( "escritura en socket" );
    exit( 1 );
  }

  char str[12];
  memset( str, '\0', 12 );
  n = read(*sockfd, str, 12);
  if (n < 0) 
  {
    perror("lectura de socket");
    exit(1);
  }
  file_size = atoi(str);
  printf("El archivo pesa: %d Bytes.\n",file_size);
  return 0;
}

int get_telemetry(int* sockfd)
{
  printf("telemetry process...\n");
  return 0;
}