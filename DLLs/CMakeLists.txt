cmake_minimum_required(VERSION 2.8)

#
# first experience with cmake and EcoDynamo DLLs for NPI
#
# all what I am learning I will experiment here
#

# about the project - add all DLLs projects
# -----------------
project(DLLs)

set(LIBRARY_OUTPUT_PATH "~/bin/lib")

message ("** Project ${PROJECT_NAME} - Building Binary tree path: ${PROJECT_BINARY_DIR}")

add_subdirectory(Iodll)
add_subdirectory(Utilities)
add_subdirectory(params)
add_subdirectory(ECDProtocol)
add_subdirectory(EcoClass)
add_subdirectory(OutputResults)
add_subdirectory(Tidedll)
add_subdirectory(TWatObjt)
add_subdirectory(TideObjt)
add_subdirectory(WindObjt)
add_subdirectory(HydrObjt)
add_subdirectory(FlowObjt)
add_subdirectory(LiteObjt)
add_subdirectory(TAirObjt)
add_subdirectory(SpmObjt)
add_subdirectory(DissObjt)
add_subdirectory(PhytObjt)
add_subdirectory(ZoopObjt)
add_subdirectory(ZoobObjt)
add_subdirectory(MacrObjt)
add_subdirectory(Sediments)
add_subdirectory(IceObjt)
add_subdirectory(IceAlgaeObjt)
add_subdirectory(BacterObjt)
add_subdirectory(FishObjt)
add_subdirectory(PredatorObjt)
add_subdirectory(PreyObjt)
add_subdirectory(ManObjt)
add_subdirectory(EcoDynamo)

