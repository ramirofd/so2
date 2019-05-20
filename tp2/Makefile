HOME=/home/Estudiante62
NETCDF_DIR=$(HOME)/Development/SO2/hpc/libs/netcdf

CC=gcc
CCFLAGS= -Werror -Wall -pedantic -O3

all: local cluster

local: main.c
	$(CC) -o ./main ./main.c $(CFLAGS)

cluster: main.c
	$(CC) -c -I$(NETCDF_DIR)/include/ main.c $(CCFLAGS) -fopenmp -std=gnu11
	$(CC) -o main main.o -L$(NETCDF_DIR)/lib/ -lnetcdf -fopenmp

clean:
	rm main