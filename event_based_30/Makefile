CC = gcc
CFLAGS=-O2 -Wall
LDLIBS = -lpthread

all: multiclient stockclient stockserver

multiclient: multiclient.c csapp.c csapp.h
stockclient: stockclient.c csapp.c csapp.h
stockserver: stockserver.c stockserver.h echo.c csapp.c csapp.h binary_tree_test.c binary_tree_test.h binary_tree.c binary_tree.h file_manage.c file_manage.h argv.c argv.h 

clean:
	rm -rf *~ multiclient stockclient stockserver *.o
