cmake_minimum_required(VERSION 3.0.0)
project(Messager VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set(CMAKE_AUTOMOC ON)

set(HEADERS 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Connection/Connection.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Message/IMessage.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Message/Message.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/NetModel/IConsumer.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/NetModel/IDataProvider.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/NetModel/NetModel.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/types/MessageTypes.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/UI/UI.hpp"
    # "${CMAKE_CURRENT_SOURCE_DIR}/src/QTstuff/mainwindow.hpp"
)

set(SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Connection/Connection.cpp"
    # "src/Message/Message.cpp"
    # "src/QTstuff/mainwindow.cpp"
)

find_package (Boost REQUIRED system QUIET)
# find_package (Qt5 COMPONENTS Widgets REQUIRED)
# find_package(Qt5 COMPONENTS Widgets DBus REQUIRED) //this is working node
find_package (nlohmann_json 3.2.0 REQUIRED)

# include_directories("src")
# include_directories("src/Connection")
# include_directories("src/Message")
# include_directories("src/NetModel")
# include_directories("src/UI")
# include_directories("src/types")
# include_directories("src/QTstuff")

add_executable(${PROJECT_NAME} 
    src/main.cpp 
    ${HEADERS} 
    ${SOURCES}
    ${Boost_INCLUDE_DIR})

target_include_directories(${PROJECT_NAME}
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/Connection>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/Message>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/NetModel>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/UI>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/types>
    # $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/src/src/QTstuff>
)    


target_link_libraries(${PROJECT_NAME}
    # Qt5::DBus   
    # Qt5::Widgets
    nlohmann_json::nlohmann_json
    pthread)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)