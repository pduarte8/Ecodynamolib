set(HDR 
	tairobjt.h 
	../include/globals.h 
	../Iodll/iodll.h 
	../EcoClass/EcoDynClass.h
	)
set(SRC 
	AirTemperatureTimeSeries.cpp 
	AirTemperatureTimeSeries2.cpp
	TAirobjt.cpp 
	TAirAdriatic.cpp 
	TAirAtlantico.cpp 
	TAirMira.cpp 
	TAirRiaF.cpp
	)
include_directories(
	${CURRENT_SOURCE_DIR} 
	../include 
	../Iodll 
	../ECDProtocol 
	../Utilities 
	../EcoClass
	)
add_library(TAirObjt SHARED ${HDR} ${SRC})

install(TARGETS TAirObjt
    DESTINATION lib
)

# vim:et:sw=4:ts=4:
