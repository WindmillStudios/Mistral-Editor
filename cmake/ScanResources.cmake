if (NOT DEFINED RESOURCE_DIR OR NOT DEFINED OUTPUT_FILE)
	message(FATAL_ERROR "Script usage: cmake -DRESOURCE_DIR=... -DOUTPUT_FILE=... -P ScanResources.cmake")
endif ()

if (NOT DEFINED BASE_FUNC)
	set(BASE_FUNC "Mistral::GetResourcesPath()")
endif ()

file(GLOB_RECURSE FOUND_FILES RELATIVE "${RESOURCE_DIR}" "${RESOURCE_DIR}/*")
list(SORT FOUND_FILES)

set(CONTENT "#pragma once\n\n")
set(CONTENT "${CONTENT}#include \"Resources.h\"\n\n")
set(CONTENT "${CONTENT}namespace RESOURCES\n{\n")

set(CURRENT_NAMESPACE_STACK "")
set(CURRENT_INDENT_LEVEL 1)

macro(GET_INDENT OUTPUT_VAR LEVEL)
	if (${LEVEL} GREATER 0)
		string(REPEAT "    " ${LEVEL} ${OUTPUT_VAR})
	else ()
		set(${OUTPUT_VAR} "")
	endif ()
endmacro()

macro(SANITIZE_NAME INPUT OUTPUT)
	set(${OUTPUT} "${INPUT}")
	string(REPLACE " " "_" ${OUTPUT} "${${OUTPUT}}")
	string(REPLACE "." "_" ${OUTPUT} "${${OUTPUT}}")
	string(REPLACE "-" "_" ${OUTPUT} "${${OUTPUT}}")
	string(TOUPPER "${${OUTPUT}}" ${OUTPUT})
endmacro()

foreach (FILE_PATH ${FOUND_FILES})
	if (NOT FILE_PATH MATCHES "(^|/)\\.")
		get_filename_component(DIR_PATH "${FILE_PATH}" DIRECTORY)
		get_filename_component(FILE_NAME_NO_EXT "${FILE_PATH}" NAME_WE)

		string(REPLACE "/" ";" TARGET_NAMESPACE_STACK "${DIR_PATH}")

		set(COMMON_DEPTH 0)
		list(LENGTH CURRENT_NAMESPACE_STACK CURR_LEN)
		list(LENGTH TARGET_NAMESPACE_STACK TARG_LEN)

		set(MAX_ITER ${CURR_LEN})
		if (TARG_LEN LESS CURR_LEN)
			set(MAX_ITER ${TARG_LEN})
		endif ()

		if (MAX_ITER GREATER 0)
			foreach (I RANGE 1 ${MAX_ITER})
				math(EXPR IDX "${I} - 1")
				list(GET CURRENT_NAMESPACE_STACK ${IDX} CURR_DIR)
				list(GET TARGET_NAMESPACE_STACK ${IDX} TARG_DIR)
				if (CURR_DIR STREQUAL TARG_DIR)
					math(EXPR COMMON_DEPTH "${COMMON_DEPTH} + 1")
				else ()
					break()
				endif ()
			endforeach ()
		endif ()

		if (CURR_LEN GREATER COMMON_DEPTH)
			math(EXPR CLOSE_COUNT "${CURR_LEN} - ${COMMON_DEPTH}")
			foreach (I RANGE 1 ${CLOSE_COUNT})
				math(EXPR CURRENT_INDENT_LEVEL "${CURRENT_INDENT_LEVEL} - 1")
				GET_INDENT(INDENT_STR ${CURRENT_INDENT_LEVEL})
				set(CONTENT "${CONTENT}${INDENT_STR}}\n")
			endforeach ()
		endif ()

		if (TARG_LEN GREATER COMMON_DEPTH)
			math(EXPR START_IDX "${COMMON_DEPTH}")
			math(EXPR END_IDX "${TARG_LEN} - 1")
			foreach (I RANGE ${START_IDX} ${END_IDX})
				list(GET TARGET_NAMESPACE_STACK ${I} NEW_DIR)
				SANITIZE_NAME("${NEW_DIR}" SANITIZED_DIR)

				GET_INDENT(INDENT_STR ${CURRENT_INDENT_LEVEL})
				set(CONTENT "${CONTENT}${INDENT_STR}namespace ${SANITIZED_DIR}\n${INDENT_STR}{\n")

				math(EXPR CURRENT_INDENT_LEVEL "${CURRENT_INDENT_LEVEL} + 1")
			endforeach ()
		endif ()

		set(CURRENT_NAMESPACE_STACK ${TARGET_NAMESPACE_STACK})

		SANITIZE_NAME("${FILE_NAME_NO_EXT}" VAR_NAME)

		GET_INDENT(INDENT_STR ${CURRENT_INDENT_LEVEL})
		set(CONTENT "${CONTENT}${INDENT_STR}static const auto ${VAR_NAME} = ${BASE_FUNC} / \"${FILE_PATH}\";\n")

	endif ()
endforeach ()

list(LENGTH CURRENT_NAMESPACE_STACK FINAL_LEN)
if (FINAL_LEN GREATER 0)
	foreach (I RANGE 1 ${FINAL_LEN})
		math(EXPR CURRENT_INDENT_LEVEL "${CURRENT_INDENT_LEVEL} - 1")
		GET_INDENT(INDENT_STR ${CURRENT_INDENT_LEVEL})
		set(CONTENT "${CONTENT}${INDENT_STR}}\n")
	endforeach ()
endif ()

set(CONTENT "${CONTENT}}\n")

if (EXISTS "${OUTPUT_FILE}")
	file(READ "${OUTPUT_FILE}" OLD_CONTENT)
else ()
	set(OLD_CONTENT "")
endif ()

if (NOT "${OLD_CONTENT}" STREQUAL "${CONTENT}")
	file(WRITE "${OUTPUT_FILE}" "${CONTENT}")
	message(STATUS "[Resources] Updated header: ${OUTPUT_FILE}")
else ()
	message(STATUS "[Resources] Up to date.")
endif ()