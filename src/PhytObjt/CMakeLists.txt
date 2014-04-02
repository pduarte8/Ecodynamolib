set(HDR 
	prodobjt.h 
	phytobjt.h 
	../include/globals.h 
	../Iodll/iodll.h 
	../EcoClass/EcoDynClass.h
	)
set(SRC 
	Phytobjt.cpp 
	Phytobjt2.cpp 
	AdaptativePhyto.cpp 
	AdaptativeSlopePhyto.cpp 
	CalaPhyto.cpp 
	CarlPhyto.cpp 
	CrestumaLeverPhyto2DV.cpp 
	CrestumaLeverPhyto2DVIntLim.cpp 
	CrestumaLeverPhyto.cpp 
	DynamicAdaptativePhyto.cpp 
	DynamicPhyto.cpp 
	EilerPhyto.cpp 
	GenericPhytoplankton.cpp 
	PhytobjtTimeSeries.cpp 
	PhytobjtBasic.cpp 
	RiaF2DPhytoPlankton.cpp 
	RiaFPhyto.cpp 
	Sangoobjt.cpp 
	SangoPhytoplanktonIntLim.cpp 
	Stoaobjt.cpp 
	TejoNPsiPhyto.cpp 
	TejoPhyto.cpp 
	TPhytoplankton3D.cpp
	)
include_directories(
	${CURRENT_SOURCE_DIR} 
	../include 
	../Iodll 
	../ECDProtocol 
	../Utilities 
	../EcoClass
	)
add_library(PhytObjt SHARED ${HDR} ${SRC})
# vim:et:sw=4:ts=4:

install(TARGETS PhytObjt
    DESTINATION lib
)
