# based on: http://stackoverflow.com/questions/11813271/embed-resources-eg-shader-code-images-into-executable-library-with-cmake
#
# example:
# cmake -Daec_RES_FILE=test.cmake -P resources.cmake
#
# where test.cmake is:
# 
# # BEGIN OF cmake.test
# 
# set(copydlls "copydlls.cmake")
# set(conf "configure.cmake")
# 
# # this three properties must be set!
#
# set(aec_RESOURCE_NAME "aecResources")
# set(aec_RESOURCE_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}")
# set(aec_RESOURCES "copydlls" "conf")
#
# # END of cmake.test
#

# should define aec_RESOURCES
include(${aec_RES_FILE})

set(aec_RESULT_DATA "")
set(aec_RESULT_INIT "")

# resource is a name visible for cpp application 
foreach(resource ${aec_RESOURCES})

	# filename is the name of file which will be used in app
	set(filename ${${resource}})

	# filedata is a file content
	file(READ ${filename} filedata HEX)

	# read full name of the file
	file(GLOB filename ${filename})

	# Convert hex data for C compatibility
	string(REGEX REPLACE "([0-9a-f][0-9a-f])" "0x\\1," filedata ${filedata})

	# append static variables to result variable
	set(aec_RESULT_DATA "${aec_RESULT_DATA}	static const unsigned char aec_${resource}[] = {\n	// ${filename}\n	${filedata}\n};\n")

	# append init resources
	set(aec_RESULT_INIT "${aec_RESULT_INIT}	m_resources[\"${resource}\"] = (char const*)aec_${resource};\n")
	set(aec_RESULT_INIT "${aec_RESULT_INIT}	m_sizes[\"${resource}\"]     = sizeof(aec_${resource});\n")

endforeach(resource)

set(aec_DST_NAME "${aec_RESOURCE_LOCATION}/${aec_RESOURCE_NAME}")

configure_file("${CMAKE_CURRENT_LIST_DIR}/resource.hpp.in" "${aec_DST_NAME}.hpp.tmp")

include("${CMAKE_CURRENT_LIST_DIR}/../aecUtils.cmake")
replace_if_different("${aec_DST_NAME}.hpp.tmp" "${aec_DST_NAME}.hpp")
