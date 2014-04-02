set(HDR 
	liteobjt.h 
	../include/globals.h 
	../Iodll/iodll.h 
	../EcoClass/EcoDynClass.h
	)
set(SRC 
	Liteobjt.cpp 
	LightWithIceObjt.cpp
	)
include_directories(
	${CURRENT_SOURCE_DIR} 
	../include 
	../Iodll 
	../ECDProtocol 
	../Utilities 
	../EcoClass
	)
add_library(LiteObjt SHARED ${HDR} ${SRC})

install(TARGETS LiteObjt
    DESTINATION lib
)

# vim:et:sw=4:ts=4:
