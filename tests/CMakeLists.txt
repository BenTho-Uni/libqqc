#[[
    \brief CMake file for the tests folder of libqqc.
    \authors Benjamin Thomitzni
]]

set(SRC_FOLDER
    folder/test_file1.cpp
)

set(HDR_FOLDER
    folder/test_file1.h
)


set(SRC
        ${SRC_FOLDER}
    libqqc_tests.C
)

set(HDR
        ${HDR_FOLDER}
)

set(TARGETS
    libqqc_tests
)

add_library(qqc_tests ${SRC} ${HDR})
add_executable(libqqc_tests ${SRC} ${HDR})

if(LIBQQC_WITH_MPI)
else(LIBQQC_WITH_MPI)
        foreach(TARGET ${TARGETS})
                target_link_libraries(${TARGET} qqc_tests qqc)
        endforeach(TARGET)
        install(TARGETS libqqc_tests DESTINATION "${CMAKE_SOURCE_DIR}/exe")
endif(LIBQQC_WITH_MPI)


