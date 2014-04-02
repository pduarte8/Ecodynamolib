set(HDR
	IceAlgaeObjt.h
	../include/globals.h
	../EcoClass/EcoDynClass.h
	../Iodll/iodll.h
	../params/params.h
	../PhytObjt/prodobjt.h
	)
set(SRC
    IceAlgaeObjt.cpp
    )
include_directories(
	${CURRENT_SOURCE_DIR}
	../include
	../Iodll
	../params
	../ECDProtocol
	../Utilities
	../EcoClass
	../PhytObjt
	)
add_library(IceAlgaeObjt SHARED ${HDR} ${SRC})
# vim:et:sw=4:ts=4:
