# Makefile created by Pedro for IODLL with _PORT_FORTRAN_

#define the C compiler to use
CC = g++
CFLAGS = -Wall -shared -fPIC -D_PORT_FORTRAN_ -fpermissive -D_GLIBCXX_USE_CXX11_ABI=0 

#define directories/files containing header files
INCLUDE = -I../include -I../Iodll -I../Utilities -I../params 

#define header and other files


#define directories/files containing source files
SRC = PIFunctions.cpp TemperatureLimitation.cpp NutrientLimitation.cpp

BGC.o:
	$(CC) $(CFLAGS) $(INCLUDE) -o ../../ecolib/libBGCFunctions.so $(SRC)	


     clean:
	rm -f core BGCFunctions.o
