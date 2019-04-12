CC=gcc
PAMFLAGS=  -lpam -lpam_misc
CFLAGS= -Werror -Wall -pedantic

unix: ./unix_version/unix_srv.c ./unix_version/unix_cli.c 
	cppcheck --enable=warning ./unix_version/
	$(CC) -o ./server_unix ./unix_version/unix_srv.c $(PAMFLAGS) $(CFLAGS)
	$(CC) -o ./client_unix ./unix_version/unix_cli.c $(CFLAGS)

clean:
	rm server_unix socket_unix client_unix world_rcv.jpg new_client_unix
