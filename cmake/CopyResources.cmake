# Logs
include(cmake/Colors.cmake)
message_color(${BoldCyan} "---------------- Copying resources --------------------------------")

# Copy resources and generate a header for easy access
include(cmake/Strings.cmake)
capitalize_string("Editor" CAPITALIZED_PROJECT_NAME)

set(RESOURCES_INPUT_DIR "${PROJECT_ROOT_DIRECTORY}/resources")
set(RESOURCES_OUTPUT_DIR "${CMAKE_BINARY_DIR}/${PROJECT_NAME}/editor_resources")
set(RESOURCES_OUTPUT_FILE "${CMAKE_BINARY_DIR}/generated/${CAPITALIZED_PROJECT_NAME}Resources.h")

# Copy
message_color(${BoldYellow} "Copying resources into the build directory")
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/generated")
execute_process(
		COMMAND ${CMAKE_COMMAND}
		-E copy_directory
		${RESOURCES_INPUT_DIR}
		${RESOURCES_OUTPUT_DIR}
)
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_BINARY_DIR}/generated")

# Header
message_color(${BoldYellow} "Generating header file for linking resources path to c++ symbols")
execute_process(
		COMMAND ${CMAKE_COMMAND}
		-DPROJECT_NAME=${CAPITALIZED_PROJECT_NAME}
		-DRESOURCES_DIR=${RESOURCES_INPUT_DIR}
		-DOUTPUT_FILE=${RESOURCES_OUTPUT_FILE}
		-P "${PROJECT_ROOT_DIRECTORY}/cmake/GenerateResourcesHeader.cmake"
)