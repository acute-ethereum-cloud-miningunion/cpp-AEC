file(GLOB sources "*.cpp" "*.h")

add_library(aecmio ${sources})

target_include_directories(aecmio PRIVATE "${UTILS_INCLUDE_DIR}")
target_link_libraries(aecmio PUBLIC evm aeccore p2p devcrypto devcore PRIVATE jsoncpp_lib_static Snappy::snappy Boost::fiber Boost::context)
