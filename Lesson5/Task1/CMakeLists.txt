cmake_minimum_required(VERSION 3.20)

set(PostgreSQL_ADDITIONAL_VERSIONS 16)

set (CMAKE_CXX_STANDARD 17)

set(INC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/inc")

file(GLOB INC_FILES "${INC_DIR}/*.h")

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

file(GLOB SRC_FILES "${SRC_DIR}/*.cpp")

project(Task1)

set(EXECUTABLE ${PROJECT_NAME})

include_directories(${INC_DIR})

add_subdirectory("../libpqxx-7.8.1" libpqxx_build)

add_executable(${EXECUTABLE} ${SRC_FILES})

target_link_libraries(Task1 pqxx)
