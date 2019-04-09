CC=gcc
PAMFLAGS=  -lpam -lpam_misc
CFLAGS= -Werror -Wall -pedantic

unix: ./unix_version/unix_srv.c
	$(CC) -o ./server_unix ./unix_version/unix_srv.c $(PAMFLAGS) $(CFLAGS)

clean:
	rm server_unix