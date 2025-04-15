cmake_minimum_required(VERSION 3.25.0)
message("C++ Compiler ID: ${CMAKE_CXX_COMPILER_ID}")
message("C++ Compiler Version: ${CMAKE_CXX_COMPILER_VERSION}")
message("C++ Standard: ${CMAKE_CXX_STANDARD}")

set(PROJECT_NAME "ControlTemp")
set(CMAKE_CXX_STANDARD 20)

project(${PROJECT_NAME})

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/dk_decision_control)

set(APP_SOURCES main.cpp)

# Specify include directories
set(APP_INCL_DIRS ${CMAKE_CURRENT_LIST_DIR}
                  ${CMAKE_CURRENT_LIST_DIR}/common/include)
message(" ------ ${CMAKE_CURRENT_LIST_DIR}")
# Create the executable

include_directories(${APP_INCL_DIRS})

add_executable(${PROJECT_NAME} ${APP_SOURCES})


target_link_libraries(${PROJECT_NAME} Decision )

# Specify include directories for the executable
target_include_directories(${PROJECT_NAME} PRIVATE ${APP_INCL_DIRS})
