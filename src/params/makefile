# Makefile created by Pedro for IODLL with _PORT_FORTRAN_

#define the C compiler to use
CC = g++
CFLAGS = -Wall -shared -fPIC -D_PORT_FORTRAN_ -fpermissive 

#define directories/files containing header files
#INCLUDE = -I../include -I../Iodll 

#define header and other files


#define directories/files containing source files
SRC = Params.cpp 


Params.o:
	$(CC) $(CFLAGS) $(INCLUDE) -o ../../ecolib/libparams.so $(SRC)	


     clean:
	rm -f core Params.o
