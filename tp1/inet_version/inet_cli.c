#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <errno.h>
#include <unistd.h>
#include <string.h>

#define   COMM_SIZE   10
#define   BUFF_SIZE   1461
#define   KNRM        "\x1B[0m"
#define   KRED        "\x1B[31m"
#define   KGRN        "\x1B[32m"
#define   KBLU        "\x1B[34m"
#define   KYEL        "\x1B[33m"
#define   PORT        6020
#define   PORT_UDP    6021

#define   VERSION     "v1.0.1"

int parse_command(char* command);
int start_scanning(int* sockfd);
int update_firmware(int* sockfd);
int get_telemetry(int* sockfd, char *server);

int main(int argc, char* argv[])
{
  int sockfd, n, fin=0;
  struct sockaddr_in serv_addr;
  struct hostent *server;

  char command[COMM_SIZE];
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    {
      perror("create");
      exit(1);	
    }

  server = gethostbyname(argv[1]);
  if (server == NULL) {
		perror("No host");
		exit(0);
	}

  char export[128];
  memset(export, '\0', COMM_SIZE);
  strcat(export, "SERV_ADDR=");
  strcat(export, argv[1]);
  putenv(export);

  memset( (char *)&serv_addr, '0', sizeof(serv_addr) );
	serv_addr.sin_family = AF_INET;
	bcopy( (char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length );
	serv_addr.sin_port = htons(PORT);

  

 	if (connect( sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr) ) < 0) 
  {
		perror("connect");
		exit(1);
	}
  while(fin==0)
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
          get_telemetry(&sockfd, argv[1]);
          break;
        case 3:
          start_scanning(&sockfd);
          break;
        case 4:
          fin=1;
          break;
      }
    }
    close(sockfd);
}

int parse_command(char* command)
{
  if(!strcmp(command, "update"))
    return 1;

  if(!strcmp(command, "telemetry"))
    return 2;
  
  if(!strcmp(command, "scanning"))
    return 3;

  if(!strcmp(command, "fin"))
    return 4;

  return -1;
}

int start_scanning(int* sockfd)
{
  int n, i=0;
  char buffer[BUFF_SIZE];
  char ok[3];

  printf("*** %sScan%s ***\n", KYEL, KNRM);

  printf("Starting scan...\n");

  FILE *file;
  file = fopen("world.jpg", "rb");

  // IMPRIMIR LOS VALORES DE N EN CLIENTE Y SERVIDOR PARA VER SI SE MANDA BIEN EL ARCHIVO!!!
  memset(buffer,'\0',BUFF_SIZE);
  n=fread(buffer,1,BUFF_SIZE,file);
  i+=n;
  while(n>=(BUFF_SIZE))
    {
      memset(buffer,'\0',BUFF_SIZE);
      n=fread(buffer,1,BUFF_SIZE,file);
      i+=n;
    }
  fclose(file);
  file = fopen("world.jpg", "rb");
  printf("File size: %d Bytes.\n",i);
  char str[11];
  memset( str, '\0', 11);
  sprintf(str, "%d", i);
  n = write(*sockfd, str, 11);
  if (n < 0) 
    {
      perror( "write" );
      exit( 1 );
    }
  i = 0;
  
  while(i<atoi(str))
    {
      memset(buffer,'\0',BUFF_SIZE);
      n=fread(buffer,1,BUFF_SIZE,file);
      i+=n;
      n = write(*sockfd, buffer, n);
      if (n < 0) 
        {
          perror( "write" );
          exit( 1 );
        }
      // leo respuesta
      memset( ok, '\0', 3 );
      n = read(*sockfd, ok, 3);
      if (n < 0) 
        {
          perror("read");
          exit(1);
        }
    }
    printf("*** %sEND%s ***\n", KYEL, KNRM);

    fclose(file);

  return 0;
}

int update_firmware(int* sockfd)
{
  int n, i=0;
  int file_size;
  char buffer[BUFF_SIZE];
  
  printf("*** %sUpdate%s ***\n", KYEL, KNRM);
  n = write(*sockfd, "ok", 2);
  if (n < 0) 
  {
    perror( "write" );
    exit( 1 );
  }

  char str[12];
  memset( str, '\0', 12 );
  n = read(*sockfd, str, 12);
  if (n < 0) 
  {
    perror("read");
    exit(1);
  }
  file_size = atoi(str);
  printf("File size: %d Bytes.\n",file_size);

  FILE *file;
  file = fopen("new_client_inet", "wb+");

  while(i<file_size)
    {
      memset(buffer, '\0', BUFF_SIZE);
      n = read(*sockfd, buffer, BUFF_SIZE-1);
      if (n < 0) 
        {
          perror("read");
          exit(1);
        }
      i+=n;
      printf("Read: %d\n",n);
      fwrite(buffer, 1, n, file);
      n = write(*sockfd, "ok", 2);
      if (n < 0) 
        {
          perror( "write" );
          exit( 1 );
        }
    }
  printf("%d\n",i);
  printf("*** %sEND%s ***\n", KYEL, KNRM);
  fclose(file);
  system("/bin/bash ./replace_inet.sh");
  return 0;
}

int get_telemetry(int* sockfd, char * server)
{
  int n, socket_client, res;
  struct sockaddr_in struct_cli;
  struct hostent *server_ent;

  FILE *command;

  char buffer[BUFF_SIZE];
  char command_buffer[BUFF_SIZE];

  printf("*** %sTelemetry Data%s ***\n", KYEL, KNRM);
  
  n = write(*sockfd, "ok", 2);
  if (n < 0) 
  {
    perror( "write" );
    return -1;
  }

  server_ent = gethostbyname(server);
	if ( server == NULL ) 
  {
    perror("No host");
    return -1;
	}

  socket_client = socket( AF_INET, SOCK_DGRAM, 0 );
	if (socket_client < 0) {
		perror( "create" );
		exit( 1 );
	}

	struct_cli.sin_family = AF_INET;
  struct_cli.sin_port = htons(PORT_UDP);
  struct_cli.sin_addr = *((struct in_addr *)server_ent->h_addr );
	memset( &(struct_cli.sin_zero), '\0', 8 );

  memset(buffer, '\0', BUFF_SIZE);
  strcat(buffer,VERSION);
  strcat(buffer,";");

  memset(command_buffer, '\0', BUFF_SIZE);
  command = popen("free -m | grep Mem | awk '{printf(\"%2.1f\", (($2-$4)/$2)*100.0)}'", "r");
  if (command == NULL) {
    printf("Failed to run command\n" );
    exit(1);
  }
  n = fread(command_buffer, 1, BUFF_SIZE, command);
  pclose(command);
  strcat(buffer,command_buffer);
  strcat(buffer,"%;");

  memset(command_buffer, '\0', BUFF_SIZE);
  command = popen("ps -A -o pcpu | tail -n+2 | paste -sd+ | bc", "r");
  if (command == NULL) {
    printf("Failed to run command\n" );
    exit(1);
  }
  n = fread(command_buffer, 1, BUFF_SIZE, command);
  pclose(command);
  command_buffer[n-1]='\0';
  strcat(buffer,command_buffer);
  strcat(buffer,"%;");

  memset(command_buffer, '\0', BUFF_SIZE);
  command = popen("uptime -p", "r");
  if (command == NULL) {
    printf("Failed to run command\n" );
    exit(1);
  }
  n = fread(command_buffer, 1, BUFF_SIZE, command);
  pclose(command);
  command_buffer[n-1]='\0';
  strcat(buffer,command_buffer);
  strcat(buffer,";");

  memset(command_buffer, '\0', BUFF_SIZE);
  command = popen("hostname", "r");
  if (command == NULL) {
    printf("Failed to run command\n" );
    exit(1);
  }
  n = fread(command_buffer, 1, BUFF_SIZE, command);
  pclose(command);
  command_buffer[n-1]='\0';
  strcat(buffer,command_buffer);
  strcat(buffer,";");

  res = sendto(socket_client, buffer, BUFF_SIZE, 0, (struct sockaddr *)&struct_cli, sizeof(struct_cli));
  if(res<0)
    {
      perror("send");
      return -1;
    }
  close(socket_client);
  return 0;
}
