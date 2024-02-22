cmake_minimum_required(VERSION 3.25.0)
project(decision VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 20)

set(GCC_COVERAGE_COMPILE_FLAGS
    "-g -O0 -coverage -fprofile-arcs -ftest-coverage")
set(GCC_COVERAGE_LINK_FLAGS "-coverage -lgcov")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS
    "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

set(ROOT ${CMAKE_CURRENT_LIST_DIR}/..)

set(SOURCES ${ROOT}/src/dk_decision_control.cpp
   
            )

set(INCL_DIRS   ${ROOT}/include
                ${ROOT}/mocks
                ${ROOT}/../common/include)

find_package(GTest REQUIRED)


add_executable(${PROJECT_NAME}_test)

target_sources(${PROJECT_NAME}_test PUBLIC
                                            ${SOURCES}
                                            tests_dk_decision_control.cpp
                                  
                                            )

target_compile_definitions(${PROJECT_NAME}_test PRIVATE)



target_include_directories(${PROJECT_NAME}_test PRIVATE
                                                    ${GTEST_INCLUDE_DIRS}
                                                    ${GMOCK_INCLUDE_DIRS}
                                                    ${INCL_DIRS}
                                                    )

target_link_libraries(${PROJECT_NAME}_test ${GTEST_LIBRARIES} ${GMOCK_LIBRARIES} GTest::GTest GTest::Main GTest::gmock pthread)
