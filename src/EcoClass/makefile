# Makefile created by Pedro for the EcoDynClass with _PORT_FORTRAN_

#define the C compiler to use
CC = g++
CFLAGS = -Wall -shared -fPIC -D_PORT_FORTRAN_ -fpermissive 

#define directories/files containing header files
INCLUDE = -I../include -I../Iodll -I../ECDProtocol -I../Utilities 

#define header and other files


#define directories/files containing source files
SRC = EcoDynClass.cpp EcoDynMisc.cpp 


EcoClass.o:
	$(CC) $(CFLAGS) $(INCLUDE) -o ../../ecolib/libecoclass.so $(SRC)	


     clean:
	rm -f core EcoClass.o
