cmake_minimum_required(VERSION 3.16)
project("mistral-editor" VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(PROJECT_ROOT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

# Logging
include(cmake/Colors.cmake)
message_color(${BoldCyan} "================ Configuring ${PROJECT_NAME} CMake project ================")
message_color(${BoldYellow} "Adding source files to ${PROJECT_NAME}")

# Editor Executable
add_executable(${PROJECT_NAME} src/main.cpp)
add_subdirectory(src)
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")

# Generate the Package (Actual zip file with the project content to release)
if (WIN32)
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++ -static")
elseif (UNIX)
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")
endif ()

include(cmake/Dependencies.cmake)
include(cmake/CopyResources.cmake)
include(cmake/GeneratePackage.cmake)