set(HDR 
	EcoDynClass.h 
	../include/globals.h 
	../ECDProtocol/EcoDynProtocol.h 
	../Iodll/iodll.h
	)
set(SRC 
	EcoDynClass.cpp 
	EcoDynMisc.cpp
	)
include_directories(
	${CURRENT_SOURCE_DIR} 
	../include 
	../Iodll 
	../ECDProtocol 
	../Utilities
	)
add_library(EcoClass SHARED ${HDR} ${SRC})
# vim:et:sw=4:ts=4:

install(TARGETS EcoClass
    DESTINATION lib
)
