#
# this function requires the following variables to be specified:
# aec_VERSION
# PROJECT_NAME
# PROJECT_VERSION
# PROJECT_COPYRIGHT_YEAR
# PROJECT_VENDOR
# PROJECT_DOMAIN_SECOND
# PROJECT_DOMAIN_FIRST
# SRC_LIST
# HEADERS
#
# params:
# ICON
#

macro(aec_copy_dll EXECUTABLE DLL)
	# dlls must be unsubstitud list variable (without ${}) in format
	# optimized;path_to_dll.dll;debug;path_to_dlld.dll
	if(DEFINED MSVC)
		list(GET ${DLL} 1 DLL_RELEASE)
		list(GET ${DLL} 3 DLL_DEBUG)
		add_custom_command(TARGET ${EXECUTABLE}
			PRE_BUILD
			COMMAND ${CMAKE_COMMAND} ARGS
			-DDLL_RELEASE="${DLL_RELEASE}"
			-DDLL_DEBUG="${DLL_DEBUG}"
			-DCONF="$<CONFIGURATION>"
			-DDESTINATION="${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}"
			-P "${aec_SCRIPTS_DIR}/copydlls.cmake"
		)
	endif()
endmacro()

macro(aec_copy_dlls EXECUTABLE)
	foreach(dll ${ARGN})
		aec_copy_dll(${EXECUTABLE} ${dll})
	endforeach(dll)
endmacro()

macro(jsonrpcstub_create EXECUTABLE SPEC SERVERNAME SERVERDIR SERVERFILENAME CLIENTNAME CLIENTDIR CLIENTFILENAME)
	if (aec_JSON_RPC_STUB)
		add_custom_target(${SPEC}stub)
		add_custom_command(
		TARGET ${SPEC}stub
		POST_BUILD
		DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${SPEC}"
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		COMMAND ${CMAKE_COMMAND} -Daec_SPEC_PATH="${CMAKE_CURRENT_SOURCE_DIR}/${SPEC}" -Daec_SOURCE_DIR="${CMAKE_SOURCE_DIR}" -Daec_CMAKE_DIR="${aec_CMAKE_DIR}"
			-Daec_CLIENT_DIR="${CLIENTDIR}"
			-Daec_CLIENT_NAME=${CLIENTNAME}
			-Daec_CLIENT_FILENAME=${CLIENTFILENAME}
			-Daec_SERVER_DIR="${SERVERDIR}"
			-Daec_SERVER_NAME=${SERVERNAME}
			-Daec_SERVER_FILENAME=${SERVERFILENAME}
			-Daec_JSON_RPC_STUB="${aec_JSON_RPC_STUB}"
			-P "${aec_SCRIPTS_DIR}/jsonrpcstub.cmake"
			)
		add_dependencies(${EXECUTABLE} ${SPEC}stub)
	endif ()
endmacro()

