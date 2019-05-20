HOME=/home/Estudiante62
NETCDF_DIR=$(HOME)/Development/SO2/hpc/libs/netcdf

CC=gcc
ICC=icc
CCFLAGS= -Werror -Wall -pedantic -O3

all: local cluster

local: main.c
	$(CC) -o ./main ./main.c $(CCFLAGS)

cluster: main.c
	$(ICC) -c -I$(NETCDF_DIR)/include/ main.c $(CCFLAGS) -qopenmp -std=gnu11
	$(ICC) -o main main.o -L$(NETCDF_DIR)/lib/ -lnetcdf -qopenmp

clean:
	rm main
