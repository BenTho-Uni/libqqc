#[[
    \brief CMake file for the test folder of libqqc.
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
    libqqc_test.cpp
)

set(HDR
        ${HDR_FOLDER}
)

set(TARGETS
    libqqc_test
)

add_library(qqc_test ${SRC} ${HDR})
add_executable(libqqc_test ${SRC} ${HDR})

if(LIBQQC_MPI)
else(LIBQQC_MPI)
        foreach(TARGET ${TARGETS})
                target_link_libraries(${TARGET} qqc_test qqc)
        endforeach(TARGET)
        install(TARGETS libqqc_test DESTINATION "${CMAKE_SOURCE_DIR}/exe")
endif(LIBQQC_MPI)


