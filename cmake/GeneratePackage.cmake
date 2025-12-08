# Logs
include(cmake/Colors.cmake)
message_color(${BoldCyan} "---------------- Generating package target ------------------------")

# Get the operative system suffix
if (WIN32)
	set(OS_SUFFIX "windows")
elseif (UNIX)
	set(OS_SUFFIX "linux")
endif ()

# Generate the file name for the distributable and the directory where it will be placed
string(TOLOWER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE_LOWER)
set(PROJECT_FILE_NAME "${PROJECT_NAME}-${CMAKE_BUILD_TYPE_LOWER}-${OS_SUFFIX}-${PROJECT_VERSION}")
set(PROJECT_RELEASE_DIR "${CMAKE_SOURCE_DIR}/build/package/${PROJECT_FILE_NAME}")

# Generate the Package
message_color(${BoldYellow} "Packaging executable")
add_custom_command(
		TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_RELEASE_DIR}
		COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${PROJECT_RELEASE_DIR}
		COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_BINARY_DIR}/${PROJECT_NAME}/editor_resources ${PROJECT_RELEASE_DIR}/editor_resources
		COMMAND ${CMAKE_COMMAND} -E chdir ${PROJECT_RELEASE_DIR} ${CMAKE_COMMAND} -E tar "cf" "${PROJECT_RELEASE_DIR}/../${PROJECT_FILE_NAME}.zip" --format=zip -- "."
)