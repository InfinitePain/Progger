cmake_minimum_required(VERSION 3.25)

# Set the Language Standards
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)

# Set the compiler flags for the Release
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -s -O3")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s -O3")

# Set the compiler flags for the Debug
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -O0 -Wall -Wpedantic")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0 -Wall -Wpedantic")

project(Progger VERSION 1.0.2)

# Set the output directory for the binary file
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${PROJECT_NAME}/)

# Set up source directories and files
file(GLOB SOURCES "src/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_BINARY_DIR})
target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)