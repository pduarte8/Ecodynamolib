if (NOT DEFINED DEFAULT_CXX_FLAGS_SET)
if (CMAKE_COMPILER_IS_GNUCXX)
    set (CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wno-unknown-pragmas -Wno-sign-compare -Woverloaded-virtual -Wno-write-strings -Wno-unused")
    set (CMAKE_CXX_FLAGS_DEBUG "-O0 -g3 -DDEBUG")
    set (CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
    if (ENABLE_CODE_COVERAGE)
        set (CMAKE_CXX_FLAGS
            "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
        set (CMAKE_CXX_LINK_FLAGS "-fprofile-arcs -ftest-coverage")
    endif()
elseif (CMAKE_CXX_COMPILER_ID MATCHES Intel)
    set (CMAKE_CXX_FLAGS "-std=c++0x -Wno-unknown-pragmas")
    set (CMAKE_CXX_FLAGS_DEBUG "-O0 -debug -DDEBUG")
    set (CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
endif ()
save_compiler_flags(CXX)
endif ()

