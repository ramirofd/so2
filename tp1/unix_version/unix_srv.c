#include <security/pam_appl.h>
#include <security/pam_misc.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <time.h>

#define   USER_SIZE   21
#define   BUFF_SIZE   1460
#define   COMM_SIZE   41
#define   KNRM        "\x1B[0m"
#define   KRED        "\x1B[31m"
#define   KGRN        "\x1B[32m"
#define   KBLU        "\x1B[34m"

static struct pam_conv conv = { misc_conv, NULL };

int parse_command(char* command, char* arg);
int start_scanning(int* sockfd);
int update_firmware(int* sockfd, char* path);
int get_telemetry(int* sockfd);

int main (int argc, char *argv[])
{
  char user[USER_SIZE];
  int attemps=0;
  pam_handle_t *pamh=NULL;
  int retval;
  int authenticated=0;

  int stream_sockfd, servlen, stream_cli_sockfd, pid;
  socklen_t clilen;
  struct sockaddr_un cli_addr, serv_addr;

  //char buffer[BUFF_SIZE];
  char command[COMM_SIZE];
  char path[BUFF_SIZE];

  const char *sun_path = "socket_unix";
  
  while (attemps<3 && authenticated==0)
  {
    printf("Please login with your system user.\n");
    printf("user: ");
    scanf("%20s",user);

    retval = pam_start("server_auth_mod", user, &conv, &pamh);

    if (retval == PAM_SUCCESS)
      retval = pam_authenticate(pamh, 0);

    if (retval == PAM_SUCCESS)
      retval = pam_acct_mgmt(pamh, 0);

    if (retval == PAM_SUCCESS) 
      {
        printf("%sAuthenticated%s\n", KGRN, KNRM);
        authenticated=1;
      }
    else 
      {
        printf("%sNot Authenticated%s\n", KRED, KNRM);
        attemps++;
      }

    if (pam_end(pamh,retval) != PAM_SUCCESS) 
      {
        pamh = NULL;
        fprintf(stderr, "server: failed to release authenticator\n");
        exit(1);
      }  
  }
  fgetc(stdin);
  if (attemps==3 && authenticated==0)
    exit(1);
  else
    {
      if ((stream_sockfd = socket( AF_UNIX, SOCK_STREAM, 0)) < 0) 
        {
          perror("create");
          exit(1);
        }

      unlink(sun_path);

      memset(&serv_addr, 0, sizeof(serv_addr));
      serv_addr.sun_family = AF_UNIX;
      strcpy( serv_addr.sun_path, sun_path );
      servlen = strlen(serv_addr.sun_path) + sizeof(serv_addr.sun_family);

      if (bind(stream_sockfd,(struct sockaddr *)&serv_addr,servlen) < 0) 
        {
          perror("bind"); 
          exit(1);
        }

      listen(stream_sockfd, 1);
      clilen = sizeof(cli_addr);
      printf("Waiting for satellite to connect...\n");
      while (1)
        {
          stream_cli_sockfd = accept(stream_sockfd, (struct sockaddr *) &cli_addr, &clilen);
          if (stream_cli_sockfd < 0)
            {
              perror("accept");
              exit(1);
            }
          pid = fork();
          if (pid < 0)
            {
              perror("fork");
              exit(1);
            }
          if (pid==0)
            {
              int fin = 0, n;
              printf("Satellite connected!\n");
              close(stream_sockfd);
              while (fin==0)
                { 
                  memset(command, 0, COMM_SIZE);
                  printf("%srun%s> ", KBLU, KNRM);
                  fgets(command,COMM_SIZE,stdin);
                  
                  //despues borrar lo de abajo
                  switch(parse_command(command, path)){
                    case 1:
                      update_firmware(&stream_cli_sockfd, path);
                      fin=1;
                      break;
                    case 2:
                      get_telemetry(&stream_cli_sockfd);
                      break;
                    case 3:
                      start_scanning(&stream_cli_sockfd);
                      break;
                    case 4:
                      n = write(stream_cli_sockfd, "fin", 3);
                      if (n < 0) 
                        {
                          perror( "write" );
                          exit( 1 );
                        }
                      fin=1;
                      break;
                    case -1:
                      printf("Use one of the following commands:\n");
                      printf("  update_firmware /path/to/firmware\n");
                      printf("  start_scanning\n");
                      printf("  get_telemetry\n");
                      break;
                  }
                }
              printf("Satellite disconnected...\n");
              printf("Waiting for satellite to connect...\n");
              exit(0);
            }
          else
          {
            close(stream_cli_sockfd);
          } 
        }
    }
  
  return 0;
}

int parse_command(char* command, char* arg)
{
  char* comm = strtok(command, " \n");

  if(!strcmp(comm, "update_firmware"))
    {
      char* argtk = strtok(NULL, "\n");
      if(argtk==NULL)
        {
          printf("Usage: update_firmware /path/to/firmware\n");
          return -2;
        }
      memset(arg, '\0', BUFF_SIZE);
      strcpy(arg, argtk);
      return 1;
    }

  if(!strcmp(comm, "get_telemetry"))
    return 2;
  
  if(!strcmp(comm, "start_scanning"))
    return 3;

  if(!strcmp(comm, "close"))
    return 4;

  return -1;
}

// Implementar funcionalidad de cada metodo.
int start_scanning(int* sockfd)
{
  int n, i=0;
  int file_size;
  char buffer[BUFF_SIZE];
  clock_t start, end;
  double cpu_time_used;

  write(*sockfd, "scanning", 8);

  printf("*** Scan ***\n");

  char str[12];
  memset( str, '\0', 12 );
  n = read(*sockfd, str, 12);
  if (n < 0) 
  {
    perror("lectura de socket");
    exit(1);
  }
  file_size = atoi(str);
  printf("File size: %d Bytes.\n",file_size);

  FILE *file;
  file = fopen("world_rcv.jpg", "wb+");

  start = clock();  

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
      fwrite(buffer, 1, n, file);
      n = write(*sockfd, "ok", 2);
      if (n < 0) 
        {
          perror( "write" );
          exit( 1 );
        }
      
    }
  end = clock();
  cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
  printf("Time taken to receive the complete scanning: %f seconds.\n", cpu_time_used);

  printf("*** END ***\n");
  fclose(file);
  return 0;
}

int update_firmware(int* sockfd, char* path)
{
  int n, i=0;
  char buffer[BUFF_SIZE];
  char ok[3];

  printf("*** Update ***\n");
  // envio comando a cliente
  n = write(*sockfd, "update", 6);
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
  //checkeo que lo recibio correctamente
  if(!strcmp(ok,"ok")){
    printf("Starting update...\n");

    FILE *file;
    file = fopen(path, "rb");

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
    file = fopen(path, "rb");
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
  }

  return 0;
}

int get_telemetry(int* sockfd)
{
  printf("telemetry process...\n");
  write(*sockfd, "telemetry", 9);
  return 0;
}