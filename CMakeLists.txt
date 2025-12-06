cmake_minimum_required(VERSION 3.16)
project(editor)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Executable
add_executable(${PROJECT_NAME} src/main.cpp)
add_subdirectory(src)
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")

# Dependencies
if (NOT TARGET mistral)
	FetchContent_Declare(
			mistral
			GIT_REPOSITORY https://github.com/WindmillStudios/Mistral-Core.git
			GIT_TAG main
	)
	FetchContent_MakeAvailable(mistral)
endif ()
target_link_libraries(${PROJECT_NAME} PUBLIC mistral)

# Copy resources directory into build path
add_custom_target(copy_resources_editor ALL
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		${CMAKE_SOURCE_DIR}/mistral-editor/resources
		${CMAKE_BINARY_DIR}/mistral-editor/editor_resources
)
add_dependencies(${PROJECT_NAME} copy_resources_editor)

# Generate resources
set(GEN_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/cmake/ScanResources.cmake")
set(RES_INPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/resources")
set(RES_OUTPUT_FILE "${CMAKE_BINARY_DIR}/generated/EditorResources.h")
set(FUNC_NAME "Mistral::GetEditorResourcesPath()")

add_custom_target(scan_editor_resources
		COMMAND ${CMAKE_COMMAND}
		-DRESOURCE_DIR=${RES_INPUT_DIR}
		-DOUTPUT_FILE=${RES_OUTPUT_FILE}
		-DBASE_FUNC=${FUNC_NAME}
		-P "${GEN_SCRIPT}"
		BYPRODUCTS "${RES_OUTPUT_FILE}"
		COMMENT "Scanning Vendaval resources..."
		VERBATIM
)

file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/generated")
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_BINARY_DIR}/generated")
add_dependencies(${PROJECT_NAME} scan_editor_resources)

# Get .clang-format from Coding-Style repo
set(STYLE_URL "https://raw.githubusercontent.com/WindmillStudios/Coding-Style/refs/heads/main/.clang-format")
set(LOCAL_STYLE_FILE "${CMAKE_CURRENT_LIST_DIR}/.clang-format")
message(${LOCAL_STYLE_FILE})

file(DOWNLOAD "${STYLE_URL}" "${LOCAL_STYLE_FILE}" SHOW_PROGRESS)