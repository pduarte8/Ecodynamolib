set(HDR 
	windobjt.h 
	../Iodll/iodll.h 
	../EcoClass/EcoDynClass.h
	)
set(SRC 
	Windobjt.cpp 
	WindDailyTimeSeries.cpp 
	WindTimeSeries.cpp 
	WindTimeSeriesV1.cpp
	)
include_directories(
	${CURRENT_SOURCE_DIR} 
	../include 
	../Iodll 
	../ECDProtocol 
	../Utilities 
	../EcoClass
	)
add_library(WindObjt SHARED ${HDR} ${SRC})
# vim:et:sw=4:ts=4:
