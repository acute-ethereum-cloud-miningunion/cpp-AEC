add_executable(aecvm main.cpp)

target_link_libraries(aecvm PRIVATE aecmio evm aecashseal devcore Boost::program_options)

target_include_directories(aecvm PRIVATE ../utils)

install(TARGETS aecvm DESTINATION bin OPTIONAL)
