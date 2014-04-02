set(HDR
    params.h
    )
set(SRC
    Params.cpp
    )
include_directories(
	${CURRENT_SOURCE_DIR}
	../include
	../Iodll
	../params
	../ECDProtocol
	../EcoClass
	../Utilities
	../PhytObjt
	../tidedll
	)
add_library(params SHARED ${HDR} ${SRC})
# vim:et:sw=4:ts=4:

install(TARGETS params
    DESTINATION lib
)
