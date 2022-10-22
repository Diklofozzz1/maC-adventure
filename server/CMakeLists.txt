cmake_minimum_required(VERSION 3.0.0)
project(Messager VERSION 0.1.0)

set(HEADERS 
    "src/Message/IMessage.hpp"
    "src/Message/Message.hpp"
    "src/NetModel/IConsumer.hpp"
    "src/NetModel/IDataProvider.hpp"
    "src/NetModel/NetModel.hpp"
    "src/Server/Server.hpp"
    "src/Server/TcpConnection.hpp"
)

set(SOURCES 
    "src/Message/Message.cpp"
    "src/Server/TcpConnection.cpp"
    "src/Server/Server.cpp"
)

find_package (Boost REQUIRED system QUIET)

include_directories("src")
include_directories("src/Server")
include_directories("src/Message")
include_directories("src/NetModel")
# include_directories("UI")

add_executable(${PROJECT_NAME} 
    src/main.cpp 
    ${HEADERS} 
    ${SOURCES}
    ${Boost_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME}
    pthread)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)