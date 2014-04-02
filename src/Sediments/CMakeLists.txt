set(HDR 
	sedimentobjt.h 
	../include/globals.h 
	../Iodll/iodll.h 
	../EcoClass/EcoDynClass.h 
	../params/params.h
	)
set(SRC 
	Sedimentobjt.cpp 
	SedimentobjtPlus.cpp 
	SedimentTimeSeriesobjt.cpp
	)
include_directories(
	${CURRENT_SOURCE_DIR} 
	../include 
	../Iodll 
	../params 
	../ECDProtocol 
	../Utilities 
	../EcoClass
	)
add_library(Sediments SHARED ${HDR} ${SRC})
# vim:et:sw=4:ts=4:

install(TARGETS Sediments
    DESTINATION lib
)
