set(HDR 
	bacteriaobjt.h 
	../include/globals.h 
	../Iodll/iodll.h 
	../EcoClass/EcoDynClass.h
	) 
set(SRC 
	Bacteria.cpp 
	EscherichiaColi.cpp
	)
include_directories(
	${CURRENT_SOURCE_DIR} 
	../include 
	../Iodll 
	../ECDProtocol 
	../Utilities 
	../EcoClass
	)
add_library(BacterObjt SHARED ${HDR} ${SRC})
# vim:et:sw=4:ts=4:
