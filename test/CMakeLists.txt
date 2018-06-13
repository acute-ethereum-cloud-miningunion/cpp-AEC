file(GLOB_RECURSE sources "*.cpp" "*.h" "*.sol")

# search for test names and create ctest tests
set(excludeSuites jsonrpc \"customTestSuite\" BlockQueueSuite)
set(allSuites jsonrpc)
foreach(file ${sources})
	file(STRINGS ${file} test_list_raw REGEX "BOOST_.*TEST_(SUITE|CASE|SUITE_END)")
	set(TestSuite "DEFAULT")
	set(TestSuitePath "")
	foreach(test_raw ${test_list_raw})
		string(REGEX REPLACE ".*TEST_(SUITE|CASE)\\(([^ ,\\)]*).*" "\\1 \\2" test ${test_raw})

		#skip disabled
		if (";${excludeSuites};" MATCHES ";${TestSuite};")
			continue()
		endif()

		if(test MATCHES "^SUITE .*")

			string(SUBSTRING ${test} 6 -1 TestSuite)
			set(TestSuitePath "${TestSuitePath}/${TestSuite}")

			if(FASTCTEST)
				if (";${excludeSuites};" MATCHES ";${TestSuite};")
					continue()
				endif()
				if (NOT ";${allSuites};" MATCHES ";${TestSuite};")
					string(SUBSTRING ${TestSuitePath} 1 -1 TestSuitePathFixed)
					list(APPEND allSuites ${TestSuite})
					separate_arguments(TESTaec_ARGS)
					set(TestaecArgs -t ${TestSuitePathFixed} -- ${TESTaec_ARGS})
					add_test(NAME ${TestSuitePathFixed} WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/test COMMAND testaec ${TestaecArgs})
				endif()
			endif()
		elseif(test MATCHES "^CASE .*")
			if(NOT FASTCTEST)
				if(NOT test MATCHES "^CASE &createRandom.*")
					string(SUBSTRING ${test} 5 -1 TestCase)
					string(SUBSTRING ${TestSuitePath} 1 -1 TestSuitePathFixed)
					separate_arguments(TESTaec_ARGS)
					set(TestaecArgs -t ${TestSuitePathFixed}/${TestCase} -- ${TESTaec_ARGS})
					add_test(NAME ${TestSuitePathFixed}/${TestCase} WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/test COMMAND testaec ${TestaecArgs})
				endif()
			endif()
		elseif (";${test_raw};" MATCHES "BOOST_AUTO_TEST_SUITE_END()")
			#encountered SUITE_END block. remove one test suite from the suite path.
			string(FIND ${TestSuitePath} "/" Position REVERSE)
			string(SUBSTRING ${TestSuitePath} 0 ${Position} TestSuitePath)
		endif()
	endforeach(test_raw)
endforeach(file)

hunter_add_package(yaml-cpp)
find_package(yaml-cpp CONFIG REQUIRED)

add_executable(testaec ${sources})
target_include_directories(testaec PRIVATE ${UTILS_INCLUDE_DIR})
target_link_libraries(testaec PRIVATE aecmio aecashseal web3jsonrpc devcrypto devcore alaec-buildinfo cryptopp-static yaml-cpp::yaml-cpp binaryen::binaryen)
install(TARGETS testaec DESTINATION ${CMAKE_INSTALL_BINDIR})


enable_testing()
set(CTEST_OUTPUT_ON_FAILURE TRUE)

include(aecUtils)

aec_add_test(ClientBase
	ARGS --aec_testfile=BlockTests/bcJS_API_Test --aec_threads=1
	ARGS --aec_testfile=BlockTests/bcJS_API_Test --aec_threads=3
	ARGS --aec_testfile=BlockTests/bcJS_API_Test --aec_threads=10
	ARGS --aec_testfile=BlockTests/bcValidBlockTest --aec_threads=1
	ARGS --aec_testfile=BlockTests/bcValidBlockTest --aec_threads=3
	ARGS --aec_testfile=BlockTests/bcValidBlockTest --aec_threads=10
)

#Does not work
#aec_add_test(JsonRpc
#	ARGS --aec_testfile=BlockTests/bcJS_API_Test
#	ARGS --aec_testfile=BlockTests/bcValidBlockTest
#)

