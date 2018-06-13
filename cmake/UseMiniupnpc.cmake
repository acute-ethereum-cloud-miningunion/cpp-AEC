function(aec_apply TARGET REQUIRED)
	find_package (Miniupnpc 1.8.2013)
	if (MINIUPNPC AND MINIUPNPC_FOUND)
		target_include_directories(${TARGET} SYSTEM PRIVATE ${MINIUPNPC_INCLUDE_DIRS})
		target_link_libraries(${TARGET} ${MINIUPNPC_LIBRARIES})
		target_compile_definitions(${TARGET} PUBLIC aec_MINIUPNPC)
	elseif (NOT ${REQUIRED} STREQUAL "OPTIONAL")
		message(FATAL_ERROR "Miniupnpc library not found")
	endif()
endfunction()