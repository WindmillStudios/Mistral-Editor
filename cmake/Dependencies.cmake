include(FetchContent)
include(cmake/Colors.cmake)
message_color(${BoldCyan} "---------------- Downloading ${PROJECT_NAME} dependencies -----------------")

# Mistral
if (NOT TARGET mistral-core)
	message_color(${BoldYellow} "Downlaoding Mistral")
	FetchContent_Declare(
			mistral-core
			GIT_REPOSITORY https://github.com/WindmillStudios/Mistral-Core.git
			GIT_TAG main
	)
	FetchContent_MakeAvailable(mistral-core)
endif ()

# Get .clang-format from Coding-Style repo
set(STYLE_URL "https://raw.githubusercontent.com/WindmillStudios/Coding-Style/refs/heads/main/.clang-format")
set(LOCAL_STYLE_FILE "${PROJECT_ROOT_DIRECTORY}/.clang-format")
file(DOWNLOAD "${STYLE_URL}" "${LOCAL_STYLE_FILE}")
message_color(${BoldYellow} "Downloading .clang-format")

# Linking libraries into the project
message_color(${BoldYellow} "Linking libraries intro ${PROJECT_NAME}")
target_link_libraries(${PROJECT_NAME} PUBLIC mistral-core)