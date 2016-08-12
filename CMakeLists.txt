cmake_minimum_required(VERSION 3.2)
project(TargetParser)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pedantic -Wall -Wextra -Wno-char-subscripts -Wno-unused-result -g -fsanitize=thread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pedantic -Wall -Wextra -Wno-char-subscripts -Wno-unused-result -g")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pedantic -Wall -Wextra -Wno-char-subscripts -Wno-unused-result -Wno-unused-parameter -Wno-ignored-qualifiers -Wno-unused-value -g")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2 -ggdb")

add_definitions(-D_LINUX)
#add_definitions(-DNDEBUG)
#add_definitions(-D_GLIBCXX_DEBUG)

set(BUILD_DIR "/home/nemchenko/work/builds/PSC_NEW")

include_directories("${BUILD_DIR}" "${BUILD_DIR}/psc/include" include)

file(GLOB_RECURSE SOURCE_FILES src/*.cpp test/*.cpp)
file(GLOB_RECURSE HEADER_FILES include/*.h)

IF ("$ENV{GTEST}" STREQUAL "y")
    MESSAGE("!Gtests're enabled!")

    find_package(GTest REQUIRED)

    include_directories(${GTEST_INCLUDE_DIRS})
    add_definitions(-D_GTEST)
ENDIF ()

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})

IF ("$ENV{GTEST}" STREQUAL "y")
    target_link_libraries(${PROJECT_NAME} ${GTEST_BOTH_LIBRARIES} )
ENDIF ()

target_link_libraries(${PROJECT_NAME} pthread)

# generate documentation
FIND_PACKAGE(Doxygen)

IF (DOXYGEN_FOUND)
#    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doxygen)

    set(doxyfile ${CMAKE_CURRENT_SOURCE_DIR}/doxygen/config.txt)

    add_custom_target(doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/doxygen
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)

    add_custom_command(TARGET doc POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_CURRENT_SOURCE_DIR}/doxygen ${CMAKE_CURRENT_BINARY_DIR}/doxygen)
ENDIF (DOXYGEN_FOUND)
