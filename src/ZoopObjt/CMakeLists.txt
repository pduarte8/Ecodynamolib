set(HDR 
	zoopobjt.h 
	../include/globals.h 
	../Iodll/iodll.h 
	../EcoClass/EcoDynClass.h 
	../params/params.h
	)
set(SRC 
	Zoopobjt.cpp 
    ZoopobjtBasic.cpp 
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
add_library(ZoopObjt SHARED ${HDR} ${SRC})

install(TARGETS ZoopObjt
    DESTINATION lib
)

# vim:et:sw=4:ts=4:
