set(HDR
    tide.h
    ../Iodll/iodll.h
    ../params/params.h
    )
set(SRC
    Tide.cpp
    )
include_directories(
	${CURRENT_SOURCE_DIR}
	../include
	../Iodll
	../params
	)
add_library(Tidedll SHARED ${HDR} ${SRC})
# vim:et:sw=4:ts=4:

install(TARGETS Tidedll
    DESTINATION lib
)
