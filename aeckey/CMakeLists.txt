add_executable(aeckey KeyAux.h main.cpp)
target_link_libraries(aeckey PRIVATE aeccore devcore Boost::program_options)
