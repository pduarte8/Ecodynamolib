# Makefile created by Pedro for the DissObjt with _PORT_FORTRAN_

#define the C compiler to use
CC = g++
CFLAGS = -Wall -shared -fPIC -D_PORT_FORTRAN_ -fpermissive -D_GLIBCXX_USE_CXX11_ABI=0 

#define directories/files containing header files
INCLUDE = -I../include -I../Iodll -I../EcoClass -I../ECDProtocol -I../Utilities -I../params 

#define header and other files


#define directories/files containing source files
SRC = Dissobjt.cpp SangoDissobjt.cpp RiaF2Dissobjt.cpp


Dissobjt.o:
	$(CC) $(CFLAGS) $(INCLUDE) -o ../../ecolib/libdissobjt.so $(SRC)	

#Dissobjt.so:
#	$(CC) -shared -o Dissobjt.so Dissobjt.o
     clean:
	rm -f core Dissobjt.o
