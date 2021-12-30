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

foreach(TARGET ${TARGETS})
    target_link_libraries(${TARGET} qqc_test qqc)
    if(LIBQQC_OPENMP) 
        target_link_libraries(qqc_test PUBLIC OpenMP::OpenMP_CXX)
    endif(LIBQQC_OPENMP)
    if(LIBQQC_MPI)
        target_link_libraries(qqc_test PUBLIC MPI::MPI_CXX)
    endif(LIBQQC_MPI)
endforeach(TARGET)
install(TARGETS libqqc_test DESTINATION "${CMAKE_SOURCE_DIR}/exe")


