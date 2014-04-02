set(HDR 
	zoobobjt.h 
	../include/globals.h 
	../Iodll/iodll.h 
	../EcoClass/EcoDynClass.h 
	../Utilities/Queue.h
	)
set(SRC 
	Zoobobjt.cpp 
	SuspensionFeed.cpp 
	BivalvesForSango.cpp 
	ChlamysFarreri.cpp 
	ChlamysFarreriIndividual.cpp 
	ChlamysFarreriV8.cpp 
	CrassostreaGigas.cpp 
	CrassostreaGigas2.cpp 
	CrassostreaGigas3.cpp 
	CrassostreaGigas4.cpp 
	CrassostreaGigasEmpirical.cpp 
	CrassostreaGigasForSango.cpp 
	CrassostreaGigasForSangoBarille.cpp 
	CrassostreaGigasForSangoRaillard.cpp 
	MytilusGalloprovincialisIndividual.cpp 
	MytilusGalloprovincialisIndividualDEB.cpp 
	RDecussatus.cpp
	)
include_directories(
	${CURRENT_SOURCE_DIR} 
	../include 
	../Iodll 
	../ECDProtocol 
	../EcoClass 
	../Utilities
	)
add_library(ZoobObjt SHARED ${HDR} ${SRC})

install(TARGETS ZoobObjt
    DESTINATION lib
)

# vim:et:sw=4:ts=4:
