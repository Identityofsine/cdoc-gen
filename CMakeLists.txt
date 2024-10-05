CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

# set the project name
PROJECT(cgdoc)

SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

FILE(GLOB SOURCES "src/*.c")

# add the executable
ADD_EXECUTABLE(cgdoc ${SOURCES})

# install script (on make install)
INSTALL(TARGETS cgdoc DESTINATION bin)


