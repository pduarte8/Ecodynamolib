set(HDR 
	zoopobjt.h 
	../include/globals.h 
	../Iodll/iodll.h 
	../EcoClass/EcoDynClass.h
	)
set(SRC 
	Zoopobjt.cpp 
	ZoopobjtBasic.cpp 
	CalaZoopobjt.cpp 
	CarlZoopobjt.cpp 
	SangoZoopobjt.cpp 
	VerticalModelZoopobjt.cpp
	)
include_directories(
	${CURRENT_SOURCE_DIR} 
	../include 
	../Iodll 
	../ECDProtocol 
	../Utilities 
	../EcoClass
	)
add_library(ZoopObjt SHARED ${HDR} ${SRC})
# vim:et:sw=4:ts=4:
