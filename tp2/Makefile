CC=gcc
CFLAGS= -Werror -Wall -pedantic -lnetcdf -fopenmp -O3

all: main.c
	$(CC) -o ./main ./main.c $(CFLAGS)

clean:
	rm main