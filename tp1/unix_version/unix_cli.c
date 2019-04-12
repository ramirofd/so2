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
  int sockfd, servlen, n, fin=0;
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
          get_telemetry(&sockfd);
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

  printf("*** Scan ***\n");
  // envio comando a cliente
  
  //checkeo que lo recibio correctamente

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
      printf("Sent: %d\n",n);
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
    fclose(file);

  return 0;
}

int update_firmware(int* sockfd)
{
  int n, i=0;
  int file_size;
  char buffer[BUFF_SIZE];
  
  printf("*** Update ***\n");
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
  file = fopen("new_client_unix", "wb+");

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
  printf("*** END ***\n");
  fclose(file);
  system("/bin/bash ./replace.sh");
  return 0;
}

int get_telemetry(int* sockfd)
{
  printf("telemetry process...\n");
  return 0;
}