cmake_minimum_required(VERSION 3.20)

project(Task1)

set (CMAKE_CXX_STANDARD 17)

set(INC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/inc")

file(GLOB INC_FILES "${INC_DIR}/*.h")

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

file(GLOB SRC_FILES "${SRC_DIR}/*.cpp")

set(WT_INC_DIR "C:/Program Files/Wt 4.10.2 msvs2022 x64/include/")

file(GLOB WT_INC_FILES "${WT_INC_DIR}/*.h")

set(WT_STATIC_LIB_DIR "C:/Program Files/Wt 4.10.2 msvs2022 x64/lib")


if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    find_library(wtdbod_LIB wtdbod ${WT_STATIC_LIB_DIR})
    find_library(wtdbopostgresd_LIB wtdbopostgresd ${WT_STATIC_LIB_DIR})
elseif (${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo")
    find_library(wtdbo_LIB wtdbo ${WT_STATIC_LIB_DIR})
    find_library(wtdbopostgres_LIB wtdbopostgres ${WT_STATIC_LIB_DIR})
endif()

set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib")

set(EXECUTABLE ${PROJECT_NAME})

include_directories(${INC_DIR} ${WT_INC_DIR})

add_executable(${EXECUTABLE} ${SRC_FILES})

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    target_link_libraries( ${PROJECT_NAME} ${wtdbod_LIB} ${wtdbopostgresd_LIB})
elseif (${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo")
    target_link_libraries( ${PROJECT_NAME} ${wtdbo_LIB} ${wtdbopostgres_LIB})
endif()

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${LIB_DIR}"
    ${CMAKE_BINARY_DIR}
)
