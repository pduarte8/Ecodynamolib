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

install(TARGETS Tidedll
    DESTINATION lib
)

# vim:et:sw=4:ts=4:
