CC=gcc
PAMFLAGS=  -lpam -lpam_misc
CFLAGS= -Werror -Wall -pedantic

inet: ./inet_version/inet_srv.c ./inet_version/inet_cli.c 
	cppcheck --enable=warning ./inet_version/
	$(CC) -o ./server_inet ./inet_version/inet_srv.c $(PAMFLAGS) $(CFLAGS)
	$(CC) -o ./client_inet ./inet_version/inet_cli.c $(CFLAGS)

unix: ./unix_version/unix_srv.c ./unix_version/unix_cli.c 
	cppcheck --enable=warning ./unix_version/
	$(CC) -o ./server_unix ./unix_version/unix_srv.c $(PAMFLAGS) $(CFLAGS)
	$(CC) -o ./client_unix ./unix_version/unix_cli.c $(CFLAGS)

cleanu:
	rm server_unix socket_unix client_unix world_rcv.jpg new_client_unix socketsc_unix

cleani:
	rm server_inet client_inet world_rcv.jpg new_client_inet

