set(HDR 
	AgentsTable.h 
	ECDPAgents.h 
	ECDPMessages.h 
	EcoDynProtocol.h 
	Region.h 
	../Utilities/Queue.h 
	../Utilities/Parser.h
	)
set(SRC 
	ECDProtocol.cpp 
	ECDProtocol_Custom_fmt.cpp 
	ECDProtocol_Custom_free.cpp 
	ECDProtocol_Custom_get.cpp 
	ECDProtocol_Custom_Public_1.cpp 
	ECDProtocol_Custom_Public_2.cpp 
	ECDProtocol_Custom_Public_3.cpp 
	ECDProtocol_Custom_Public_4.cpp 
	ECDProtocol_Custom_Public_5.cpp 
	ECDProtocol_fmt_1.cpp 
	ECDProtocol_fmt_2.cpp 
	ECDProtocol_fmt_3.cpp 
	ECDProtocol_fmt_4.cpp 
	ECDProtocol_fmt_5.cpp 
	ECDProtocol_get_1.cpp 
	ECDProtocol_get_2.cpp 
	ECDProtocol_get_3.cpp 
	ECDProtocol_get_4.cpp 
	ECDProtocol_get_5.cpp 
	ECDProtocol_util.cpp 
	Region.cpp
	)
include_directories(
	${CURRENT_SOURCE_DIR} 
	../include 
	../Utilities
	)
add_library(ECDProtocol SHARED ${HDR} ${SRC})
# vim:et:sw=4:ts=4:

install(TARGETS ECDProtocol
    DESTINATION lib
)
