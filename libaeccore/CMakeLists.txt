file(GLOB sources "*.cpp" "*.h")

add_library(aeccore ${sources})

target_include_directories(aeccore PRIVATE "${UTILS_INCLUDE_DIR}")
target_link_libraries(aeccore PUBLIC devcrypto devcore)
