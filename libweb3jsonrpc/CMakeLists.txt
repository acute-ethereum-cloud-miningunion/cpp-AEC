set(sources
    AccountHolder.cpp
    AccountHolder.h
    Adminaec.cpp
    Adminaec.h
    AdminaecFace.h
    AdminNet.cpp
    AdminNet.h
    AdminNetFace.h
    Debug.cpp
    Debug.h
    DebugFace.h
    aec.cpp
    aec.h
    aecFace.h
    IpcServer.h
    IpcServerBase.cpp
    IpcServerBase.h
    JsonHelper.cpp
    JsonHelper.h
    ModularServer.h
    Net.cpp
    Net.h
    NetFace.h
    Personal.cpp
    Personal.h
    PersonalFace.h
    SessionManager.cpp
    SessionManager.h
    Test.cpp
    Test.h
    TestFace.h
    Web3.cpp
    Web3.h
    Web3Face.h
    WhisperFace.h
)

if(WIN32)
    list(APPEND sources WinPipeServer.cpp WinPipeServer.h)
else()
    list(APPEND sources UnixSocketServer.cpp UnixSocketServer.h)
endif()

add_library(web3jsonrpc ${sources})

jsonrpcstub_create(
    web3jsonrpc aec.json
    dev::rpc::aecFace ${CMAKE_CURRENT_SOURCE_DIR} aecFace
    aecClient ${CMAKE_CURRENT_BINARY_DIR} aecClient
)

jsonrpcstub_create(web3jsonrpc debug.json
    dev::rpc::DebugFace ${CMAKE_CURRENT_SOURCE_DIR} DebugFace
    DebugClient ${CMAKE_CURRENT_BINARY_DIR} DebugClient
)

jsonrpcstub_create(web3jsonrpc net.json
    dev::rpc::NetFace ${CMAKE_CURRENT_SOURCE_DIR} NetFace
    NetClient ${CMAKE_CURRENT_BINARY_DIR} NetClient
)

jsonrpcstub_create(web3jsonrpc web3.json
    dev::rpc::Web3Face ${CMAKE_CURRENT_SOURCE_DIR} Web3Face
    Web3Client ${CMAKE_CURRENT_BINARY_DIR} Web3Client
)

jsonrpcstub_create(web3jsonrpc admin_net.json
    dev::rpc::AdminNetFace ${CMAKE_CURRENT_SOURCE_DIR} AdminNetFace
    AdminNetClient ${CMAKE_CURRENT_BINARY_DIR} AdminNetClient
)

jsonrpcstub_create(web3jsonrpc admin_aec.json
    dev::rpc::AdminaecFace ${CMAKE_CURRENT_SOURCE_DIR} AdminaecFace
    AdminaecClient ${CMAKE_CURRENT_BINARY_DIR} AdminaecClient
)

jsonrpcstub_create(web3jsonrpc personal.json
    dev::rpc::PersonalFace ${CMAKE_CURRENT_SOURCE_DIR} PersonalFace
    PersonalClient ${CMAKE_CURRENT_BINARY_DIR} PersonalClient
)

jsonrpcstub_create(web3jsonrpc test.json
    dev::rpc::TestFace ${CMAKE_CURRENT_SOURCE_DIR} TestFace
    TestClient ${CMAKE_CURRENT_BINARY_DIR} TestClient
)

target_link_libraries(web3jsonrpc aecashseal webthree libjson-rpc-cpp::server)
