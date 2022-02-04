cmake_minimum_required(VERSION 3.0.0)
project(nqueens VERSION 1.0.0) #specify the executable name and version #

#specify the C++ standard 
set(CMAKE_CXX_STANDARD 17)

add_executable(nqueens main.cpp board/board.cpp)

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}/board"
    "${CMAKE_CURRENT_SOURCE_DIR}/queen"
    )

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
