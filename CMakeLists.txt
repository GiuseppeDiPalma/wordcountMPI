cmake_minimum_required(VERSION 3.16)

project(mpi-word-count)

enable_language(C)

#### MPI
find_package(MPI REQUIRED)
if (MPI_FOUND)
    include_directories(SYSTEM ${MPI_INCLUDE_PATH})
else (MPI_FOUND)
    message(SEND_ERROR "This application cannot compile without MPI")
endif (MPI_FOUND)

add_subdirectory(src)