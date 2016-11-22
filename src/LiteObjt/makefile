# Makefile created by Pedro for the LitObjt with _PORT_FORTRAN_

#define the C compiler to use
CC = g++
CFLAGS = -O3 -Wall -shared -fPIC -D_PORT_FORTRAN_ -fpermissive 

#define directories/files containing header files
INCLUDE = -I../include -I../Iodll -I../EcoClass -I../ECDProtocol -I../Utilities 

#define header and other files


#define directories/files containing source files
SRC = Liteobjt.cpp 


Liteobjt.o:
	$(CC) $(CFLAGS) $(INCLUDE) -o ../../ecolib/libliteobjt.so Liteobjt.cpp	

#Liteobjt.so:
#	$(CC) -shared -o Liteobjt.so Liteobjt.o
     clean:
	rm -f core Liteobjt.o
