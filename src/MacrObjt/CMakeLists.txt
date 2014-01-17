
#
#   Macrophytes
#

# project name and files 
# -----------------
project(Macrophytes)
set(PROJ_HDR 
	macrobjt.h 
	../include/globals.h 
	../Iodll/iodll.h 
	../EcoClass/EcoDynClass.h 
	../PhytObjt/prodobjt.h
	)
set(PROJ_SRC 
	Macrobjt.cpp 
	EnteromorphaSp.cpp 
	GelidiumSesquipedale.cpp 
	LaminariaJaponica.cpp 
	LaminariaJaponicaExponencialGrowth.cpp 
	RuppiaCirrhosa.cpp 
	RuppiaCirrhosaMesocosmos.cpp 
	SaltMarshForcing.cpp 
	UlvaSp.cpp 
	ZosteraMarina.cpp 
	ZosteraNolti.cpp
	)


# add includes and libraries
# --------------------------
set(LIBRARY_OUTPUT_PATH "~/bin/lib")
set(LIB_DIR "~/bin/lib")
include_directories(
	${PROJECT_SOURCE_DIR} 
	../include 
	../Iodll 
	../ECDProtocol 
	../Utilities 
	../EcoClass 
	../PhytObjt
	)

# create the library
# ---------------
add_library(${PROJECT_NAME} SHARED ${PROJ_HDR} ${PROJ_SRC})

# linker
# ---------------
find_library(IODLL IO ${LIB_DIR})
find_library(ECLASS EcoClass ${LIB_DIR})

target_link_libraries(${PROJECT_NAME} ${ECLASS} ${IODLL})

