# Makefile created by Pedro for the IceAlgaeObjt with _PORT_FORTRAN_

#define the C compiler to use
CC = g++
CFLAGS = -Wall -shared -fPIC -D_PORT_FORTRAN_ -fpermissive -D_GLIBCXX_USE_CXX11_ABI=0

#define directories/files containing header files
INCLUDE = -I../include -I../Iodll -I../EcoClass -I../ECDProtocol -I../Utilities -I../params -I../PhytObjt 

#define header and other files


#define directories/files containing source files
SRC = IceAlgaeObjt.cpp 


IceAlgaeobjt.o:
	$(CC) $(CFLAGS) $(INCLUDE) -o ../../ecolib/libicealgaeobjt.so $(SRC)	

#IceAlgaeobjt.so:
#	$(CC) -shared -o IceAlgaeobjt.so IceAlgaeobjt.o
     clean:
	rm -f core IceAlgaeObjt.o
