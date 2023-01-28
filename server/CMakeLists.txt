cmake_minimum_required(VERSION 3.10)
project(Messager VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(HEADERS 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Message/IMessage.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Message/Message.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/NetModel/IConsumer.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/NetModel/IDataProvider.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/NetModel/NetModel.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Server/Server.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Server/TcpConnection.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/types/MessageTypes.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Server/MessageProcessor.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Server/ConnectionsManager.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Server/ISender.hpp"
)

set(SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Message/Message.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Server/TcpConnection.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Server/Server.cpp"
)

find_package (Boost REQUIRED system QUIET)
find_package (nlohmann_json 3.2.0 REQUIRED)

add_executable(${PROJECT_NAME} 
    src/main.cpp 
    ${HEADERS} 
    ${SOURCES}
    ${Boost_INCLUDE_DIR})
   

target_include_directories(${PROJECT_NAME}
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/NetModel>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/types>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/Message>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/Server>
)    

target_link_libraries(${PROJECT_NAME}
    nlohmann_json::nlohmann_json
    pthread)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)