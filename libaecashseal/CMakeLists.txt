set(sources
    aecash.cpp
    aecash.h
    aecashClient.cpp
    aecashClient.h
    aecashCPUMiner.cpp
    aecashCPUMiner.h
    aecashProofOfWork.cpp
    aecashProofOfWork.h
    GenesisInfo.cpp
    GenesisInfo.h
)

add_library(aecashseal ${sources})
target_include_directories(aecashseal PRIVATE ../utils)
target_link_libraries(aecashseal PUBLIC aecmio aeccore)
