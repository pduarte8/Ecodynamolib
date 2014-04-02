set(HDR 
	iceobjt.h 
	../include/globals.h 
	../Iodll/iodll.h 
	../EcoClass/EcoDynClass.h
	)
set(SRC 
	Iceobjt.cpp 
	IceTimeSeries.cpp
	)
include_directories(
	${CURRENT_SOURCE_DIR} 
	../include 
	../Iodll 
	../ECDProtocol 
	../Utilities 
	../EcoClass
	)
add_library(IceObjt SHARED ${HDR} ${SRC})
# vim:et:sw=4:ts=4:

install(TARGETS IceObjt
    DESTINATION lib
)
