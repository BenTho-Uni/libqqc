
#[[
    \brief CMake file for the general libqqc folder. 
    \authors Benjamin Thomitzni
]]

# setting variables according to folder structure
# additional
set(SRC_ADDITIONAL
    folder/file1.cpp
)

set(HDR_ADDITIONAL
    folder/file1.h
)

# for compiling with MPI
set(SRC_MPI
    folder/file2_mpi.cpp
)

set(HDR_MPI
    folder/file2_mpi.h
)

#SRC_ is a variable name
set(SRC
    ${SRC_ADDITIONAL}
    main.cpp
)

set(HDR
    ${SRC_ADDITIONAL}
)

#[[
    add_library(qqc ${SRC} ${HDR})
    target_include_directories(qqc PUBLIC "${LIBQQC_SOURCE_DIR}" "${CMAKE_SOURCE_DIR}/eigen")
    target_compile_definitions(qqc PUBLIC EIGEN_DONT_PARALLELIZE)
    target_link_libraries(jqqc ${ARMADILLO_LIBRARIES} OpenMP::OpenMP_CXX)
    install(TARGETS jove DESTINATION "${CMAKE_SOURCE_DIR}/lib")
]]

if(LIBQQC_WITH_MPI)
    add_library(qqc ${SRC} ${HDR} ${SRC_MPI} ${HDR_MPI})
    target_include_directories(qqc PUBLIC "${LIBQQC_SOURCE_DIR}")
    target_link_libraries(qqc OpenMP::OpenMP_CXX)
    install(TARGETS libqqc DESTINATION "${CMAKE_SOURCE_DIR}/lib")
else(LIBQQC_WITH_MPI)
    add_library(qqc ${SRC} ${HDR})
    target_include_directories(qqc PUBLIC "${LIBQQC_SOURCE_DIR}")
    target_link_libraries(qqc OpenMP::OpenMP_CXX)
    install(TARGETS qqc DESTINATION "${CMAKE_SOURCE_DIR}/lib")
endif(LIBQQC_WITH_MPI)



