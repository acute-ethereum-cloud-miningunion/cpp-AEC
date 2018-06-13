# generates JSONRPC Stub Server && Client
#
# this script expects
# aec_SOURCE_DIR - main CMAKE_SOURCE_DIR
# aec_SPEC_PATH
# aec_SERVER_DIR
# aec_CLIENT_DIR
# aec_SERVER_NAME
# aec_CLIENT_NAME
# aec_JSON_RPC_STUB
#
# example usage:
# cmake -Daec_SPEC_PATH=spec.json -Daec_SERVER_DIR=libweb3jsonrpc -Daec_CLIENT_DIR=test
# -Daec_SERVER_NAME=AbstractWebThreeStubServer -Daec_CLIENT_NAME=WebThreeStubClient -Daec_JSON_RPC_STUB=/usr/local/bin/jsonrpcstub

# setup names, and allow different filename from classname for namespaced classes.
# For an example call look at libaecmio/CMakeLists.txt for aec::dev::Sentinel
if (aec_SERVER_FILENAME)
	set(SERVER_TMPFILE "${aec_SERVER_DIR}/${aec_SERVER_FILENAME}.h.tmp")
	set(SERVER_TMPFILE2 "${aec_SERVER_DIR}/${aec_SERVER_FILENAME}.h.tmp2")
	set(SERVER_OUTFILE "${aec_SERVER_DIR}/${aec_SERVER_FILENAME}.h")
else ()
	set(SERVER_TMPFILE "${aec_SERVER_DIR}/${aec_SERVER_NAME}.h.tmp")
	set(SERVER_TMPFILE2 "${aec_SERVER_DIR}/${aec_SERVER_NAME}.h.tmp2")
	set(SERVER_OUTFILE "${aec_SERVER_DIR}/${aec_SERVER_NAME}.h")
endif()
if (aec_CLIENT_FILENAME)
	set(CLIENT_TMPFILE "${aec_CLIENT_DIR}/${aec_CLIENT_FILENAME}.h.tmp")
	set(CLIENT_OUTFILE "${aec_CLIENT_DIR}/${aec_CLIENT_FILENAME}.h")
else ()
	set(CLIENT_TMPFILE "${aec_CLIENT_DIR}/${aec_CLIENT_NAME}.h.tmp")
	set(CLIENT_OUTFILE "${aec_CLIENT_DIR}/${aec_CLIENT_NAME}.h")
endif ()

# create tmp files
if (NOT aec_SERVER_DIR)
	execute_process(
		COMMAND ${aec_JSON_RPC_STUB} ${aec_SPEC_PATH}
			--cpp-client=${aec_CLIENT_NAME} --cpp-client-file=${CLIENT_TMPFILE}
			OUTPUT_VARIABLE ERR ERROR_QUIET
	)
else ()
	execute_process(
		COMMAND ${aec_JSON_RPC_STUB} ${aec_SPEC_PATH}
			--cpp-server=${aec_SERVER_NAME} --cpp-server-file=${SERVER_TMPFILE}
			--cpp-client=${aec_CLIENT_NAME} --cpp-client-file=${CLIENT_TMPFILE}
			OUTPUT_VARIABLE ERR ERROR_QUIET
	)

	# get name without namespace
	string(REPLACE "::" ";" SERVER_NAME_LIST "${aec_SERVER_NAME}")
	list(LENGTH SERVER_NAME_LIST SERVER_NAME_LENGTH)
	math(EXPR SERVER_NAME_POS "${SERVER_NAME_LENGTH} - 1")
	list(GET SERVER_NAME_LIST ${SERVER_NAME_POS} SERVER_NAME)

	file(READ ${SERVER_TMPFILE} SERVER_CONTENT)

	# The following cmake regexps are equal to this sed command
	# 	sed -e s/include\ \<jsonrpccpp\\/server\.h\>/include\ ${INCLUDE_NAME}/g \
	#		-e s/public\ jsonrpc::AbstractServer\<${NAME}\>/public\ ServerInterface\<${NAME}\>/g \
	#		-e s/${NAME}\(jsonrpc::AbstractServerConnector\ \&conn,\ jsonrpc::serverVersion_t\ type\ =\ jsonrpc::JSONRPC_SERVER_V2\)\ :\ jsonrpc::AbstractServer\<${NAME}\>\(conn,\ type\)/${NAME}\(\)/g \
	string(REGEX REPLACE "include\ <jsonrpccpp/server.h>" "include\ \"ModularServer.h\"" SERVER_CONTENT "${SERVER_CONTENT}")
	string(REGEX REPLACE "public\ jsonrpc::AbstractServer<${SERVER_NAME}>" "public ServerInterface<${SERVER_NAME}>" SERVER_CONTENT "${SERVER_CONTENT}")
	string(REGEX REPLACE "${SERVER_NAME}\\(jsonrpc::AbstractServerConnector\ &conn,\ jsonrpc::serverVersion_t\ type\ =\ jsonrpc::JSONRPC_SERVER_V2\\)\ :\ jsonrpc::AbstractServer<${SERVER_NAME}>\\(conn, type\\)" "${SERVER_NAME}()" SERVER_CONTENT "${SERVER_CONTENT}")

	file(WRITE ${SERVER_TMPFILE2} "${SERVER_CONTENT}")
endif()

# don't throw fatal error on jsonrpcstub error, someone might have old version of jsonrpcstub,
# he does not need to upgrade it if he is not working on JSON RPC
# show him warning instead
if (ERR)
	message(WARNING "Your version of jsonrcpstub tool is not supported. Please upgrade it.")
	message(WARNING "${ERR}")
else()
	include("${aec_CMAKE_DIR}/aecUtils.cmake")
	if (aec_SERVER_DIR)
		file(REMOVE "${SERVER_TMPFILE}")
		replace_if_different("${SERVER_TMPFILE2}" "${SERVER_OUTFILE}")
	endif()
	replace_if_different("${CLIENT_TMPFILE}" "${CLIENT_OUTFILE}")
endif()
