cmake_minimum_required(VERSION 3.15)
project(bnfc)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCE_FILES ${CMAKE_SOURCE_DIR}/bnfc/*.cpp ${CMAKE_SOURCE_DIR}/parser/*.cpp ${CMAKE_SOURCE_DIR}/editor/*.cpp)

add_executable(bnfc main.cpp LRParser.cpp Dud.cpp LR0.cpp ${SOURCE_FILES} parser/h/Token.h parser/h/Rule.h parser/h/State.h)

##########    GLFW    ##########
set(GLFW_DIR "${CMAKE_SOURCE_DIR}/external/glfw")
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")
add_subdirectory("${GLFW_DIR}")
target_link_libraries(${PROJECT_NAME} "glfw" "${GLFW_LIBRARIES}")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLFW_DIR}/include")
target_compile_definitions(${PROJECT_NAME} PRIVATE "GLFW_INCLUDE_NONE")

##########    GLAD    ##########
set(GLAD_DIR "${CMAKE_SOURCE_DIR}/external/glad")
add_library("glad" "${GLAD_DIR}/src/glad.c")
target_include_directories("glad" PRIVATE "${GLAD_DIR}/include")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLAD_DIR}/include")
target_link_libraries(${PROJECT_NAME} "glad" "${CMAKE_DL_LIBS}")