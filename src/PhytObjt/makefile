# Makefile created by Pedro for the PhytObjt with _PORT_FORTRAN_

#define the C compiler to use
CC = g++
CFLAGS = -Wall -shared -fPIC -D_PORT_FORTRAN_ -fpermissive -D_GLIBCXX_USE_CXX11_ABI=0 

#define directories/files containing header files
INCLUDE = -I../include -I../Iodll -I../EcoClass -I../ECDProtocol -I../Utilities -I../params -I../BGCLibraries 

#define header and other files


#define directories/files containing source files
SRC = Phytobjt.cpp CrestumaLeverPhyto2DV.cpp CrestumaLeverPhyto2DVIntLim.cpp GenericPhytoplankton.cpp 


Phytobjt.o:
	$(CC) $(CFLAGS) $(INCLUDE) -o ../../ecolib/libphytobjt.so $(SRC) 	

#Phytobjt.so:
#	$(CC) -shared -o Phytobjt.so Phytobjt.o
     clean:
	rm -f core Phytobjt.o
