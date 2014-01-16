include_directories (
    ${NETCDF_INCLUDES}
    )

if (MPI_FOUND)
    set(MPI_NUMPROC 1)
    set(MPIEXEC_PREFLAGS "--hostfile hostfile")
    set (MPIRUN
        "${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} ${MPI_NUMPROC} ${MPIEXEC_PREFLAGS}")
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/hostfile
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif()

include(UseLocalBuildTree)
#add_test(NAME DynamoTest
#    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/dynamo_test.x
#    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
#    )


# vim:et:sw=4:ts=4:
