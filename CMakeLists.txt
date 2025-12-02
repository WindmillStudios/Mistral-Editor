project(vendaval)

# Executable
add_executable(${PROJECT_NAME}
		main.cpp
		VendavalRenderPipeline.h
		VendavalRenderPipeline.cpp
		Editor.h
		Editor.cpp
)

add_subdirectory(editors)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC floresta_lib)

# Generate resources
set(GEN_SCRIPT "${CMAKE_SOURCE_DIR}/cmake/ScanResources.cmake")
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

add_subdirectory(src)