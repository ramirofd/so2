#include <security/pam_appl.h>
#include <security/pam_misc.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>

#define   USER_SIZE   21
#define   BUFF_SIZE   41
#define   COMM_SIZE   41
#define   KNRM        "\x1B[0m"
#define   KRED        "\x1B[31m"
#define   KGRN        "\x1B[32m"
#define   KBLU        "\x1B[34m"

static struct pam_conv conv = { misc_conv, NULL };

int main (int argc, char *argv[])
{
  char user[USER_SIZE];
  int attemps=0;
  pam_handle_t *pamh=NULL;
  int retval;
  int authenticated=0;

  int stream_sockfd, servlen, stream_cli_sockfd, n, pid;
  socklen_t clilen;
  struct sockaddr_un cli_addr, serv_addr;

  char buffer[BUFF_SIZE];
  char command[COMM_SIZE];

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

  if (attemps==3 && authenticated==0)
    exit(1);
  else
    {
      if ((stream_sockfd = socket( AF_UNIX, SOCK_STREAM, 0)) < 0) 
        {
          perror( "create");
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
              printf("Satellite connected!\n");
              close(stream_sockfd);
              while (1)
                { 
                  memset(command, 0, COMM_SIZE);
                  printf("%srun%s> ", KBLU, KNRM);
                  scanf("%40s",command);
                  //-------- Desde aca ---------//
                  /*
                    tengo que reemplazar por la ejecucion de cada comando
                  */
                  memset(buffer, 0, BUFF_SIZE);
                  n = read(stream_cli_sockfd, buffer, BUFF_SIZE);
                  if(n < 0){
                    perror("socket read");
                    exit(1);
                  }

                  printf( "Recibí: %s", buffer );

                  n = write(stream_cli_sockfd, command, COMM_SIZE );
                  if ( n < 0 ) {
                    perror( "socket write" );
                    exit( 1 );
                  }
                  //------- Hasta aca ---------///
                }
            }
          else
          {
            close(stream_cli_sockfd);
          } 
        }
    }
  
  return 0;
}