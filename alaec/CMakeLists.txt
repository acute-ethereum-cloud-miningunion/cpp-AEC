set(
    sources
    AccountManager.cpp AccountManager.h
    main.cpp
    MinerAux.h
)

add_executable(alaec ${sources})
target_link_libraries(
    alaec
    PRIVATE aecmio aecashseal evm web3jsonrpc webthree devcore Boost::program_options
)

target_include_directories(alaec PRIVATE ../utils)

if(MINIUPNPC)
    target_compile_definitions(alaec PRIVATE aec_MINIUPNPC)
endif()

install(TARGETS alaec DESTINATION bin)
