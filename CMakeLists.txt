CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

# set the project name
PROJECT(cgdoc)

set(CMAKE_BUILD_TYPE Debug)
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0") # Add debug symbols and disable optimizations

FILE(GLOB SOURCES "src/*.c", "src/*.h", "src/**/*.c", "src/**/*.h")

# add the executable
ADD_EXECUTABLE(cgdoc ${SOURCES})

# install script (on make install)
INSTALL(TARGETS cgdoc DESTINATION bin)


