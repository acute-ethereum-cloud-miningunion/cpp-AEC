add_library(webthree WebThree.cpp WebThree.h)
target_include_directories(webthree PRIVATE ../utils)
target_link_libraries(webthree PUBLIC aecmio p2p devcore)
