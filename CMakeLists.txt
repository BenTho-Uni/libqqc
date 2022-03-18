cmake_minimum_required(VERSION 3.12)
project(LIBQQC CXX)

# Going through the configure options
if(LIBQQC_OPENMP)
    message("+++ Compiling with OpenMP.")
    # required packages: OpenMP
    find_package(OpenMP REQUIRED)
    add_compile_definitions(LIBQQC_WITH_OPENMP)
else(LIBQQC_OPENMP)
    message("+++ Compiling without OpenMP.")
endif(LIBQQC_OPENMP)

if(LIBQQC_EIGEN)
    message("+++ Compiling with Eigen.")
    add_compile_definitions(LIBQQC_WITH_EIGEN)
    SET( EIGEN3_INCLUDE_DIR "$ENV{EIGEN3_INCLUDE_DIR}" )
    IF( NOT EIGEN3_INCLUDE_DIR )
        MESSAGE( FATAL_ERROR "Please point the environment variable 
        EIGEN3_INCLUDE_DIR to the include directory of your Eigen3 installation.")
    ENDIF()
    INCLUDE_DIRECTORIES ( "${EIGEN3_INCLUDE_DIR}" )
else(LIBQQC_EIGEN)
    message("+++ Compiling without Eigen.")
endif(LIBQQC_EIGEN)

if(LIBQQC_GPU)
    message("+++ Compiling with OpenMP Offloading to GPU.")
    add_compile_definitions(LIBQQC_WITH_GPU)
else(LIBQQC_GPU)
    message("+++ Compiling without OpenMP Offloading to GPU.")
endif(LIBQQC_GPU)

if(LIBQQC_MPI)
    message("+++ Compiling with MPI.")
    find_package(MPI REQUIRED)
# next line only needed when using OpenMPI
    add_definitions(-DOMPI_SKIP_MPICXX)
    add_compile_definitions(LIBQQC_WITH_MPI)
else(LIBQQC_MPI)
    message("+++ Compiling without MPI")
endif(LIBQQC_MPI)

if(CMAKE_BUILD_TYPE STREQUAL "RELEASE")
    add_definitions(-O3)
    add_definitions(-ffast-math)
    add_definitions(-march=native)
    message("+++ Compiling with RELEASE flags")
endif(CMAKE_BUILD_TYPE STREQUAL "RELEASE")

# additional packages: Doxygen
find_package(Doxygen)

# subdirectories for compiling
add_subdirectory(libqqc)

# generating variable if test should be compiled, if, add directory
option(LIBQQC_TESTS "building libqqc tests" OFF) 
if(LIBQQC_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif(LIBQQC_TESTS)

# Adding Cmpiler flags
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -ffast-math")

# find_package sets DOXYGEN_FOUND to true if found, compile doxygen
if(DOXYGEN_FOUND)
    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile"
        "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile" @ONLY)
    add_custom_target(libqqc_docs
        "${DOXYGEN_EXECUTABLE}" "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile"
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
        COMMENT "Doxygen documentation" VERBATIM)
endif(DOXYGEN_FOUND)
