set(HDR 
	tideobjt.h 
	../Tidedll/tide.h 
	../Iodll/iodll.h 
	../EcoClass/EcoDynClass.h
	)
set(SRC 
	Tideobjt.cpp 
	TideObjectForSangoWanHarmonics.cpp
	)
include_directories(
	${CURRENT_SOURCE_DIR} 
	../include 
	../Iodll 
	../params 
	../ECDProtocol 
	../Utilities 
	../EcoClass 
	../Tidedll
	)
add_library(TideObjt SHARED ${HDR} ${SRC})

install(TARGETS TideObjt
    DESTINATION lib
)

# vim:et:sw=4:ts=4:
