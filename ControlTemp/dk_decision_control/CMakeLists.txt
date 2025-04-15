cmake_minimum_required(VERSION 3.25.0)
project(Decision VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 20)

set(SOURCES src/dk_decision_control.cpp
            )

find_package(GTest REQUIRED)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/tests)


set(INCL_DIRS   ${CMAKE_CURRENT_LIST_DIR}/include
                ${CMAKE_CURRENT_LIST_DIR}/mocks
                ${CMAKE_CURRENT_LIST_DIR}/../common/include)
             
message("--2--${CMAKE_CURRENT_LIST_DIR} ")
include_directories(${INCL_DIRS})
add_library(${PROJECT_NAME})
message("--2--${PROJECT_NAME} ")

target_include_directories(${PROJECT_NAME} PRIVATE ${INCL_DIRS})

target_sources(${PROJECT_NAME} PRIVATE ${SOURCES})

set(${PROJECT_NAME}_INCLUDE_DIRS ${INCL_DIRS} CACHE INTERNAL "")

