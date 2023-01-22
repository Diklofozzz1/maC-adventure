cmake_minimum_required(VERSION 3.0.0)
project(Messager VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set(CMAKE_AUTOMOC ON)

set(HEADERS 
    "src/Connection/Connection.hpp"
    "src/Message/IMessage.hpp"
    "src/Message/Message.hpp"
    "src/NetModel/IConsumer.hpp"
    "src/NetModel/IDataProvider.hpp"
    "src/NetModel/NetModel.hpp"
    "src/types/MessageTypes.hpp"
    "src/UI/UI.hpp"
    # "src/QTstuff/mainwindow.hpp"
)

set(SOURCES 
    "src/Connection/Connection.cpp"
    # "src/Message/Message.cpp"
    # "src/QTstuff/mainwindow.cpp"
)

find_package (Boost REQUIRED system QUIET)
# find_package (Qt5 COMPONENTS Widgets REQUIRED)
# find_package(Qt5 COMPONENTS Widgets DBus REQUIRED) //this is working node
find_package (nlohmann_json 3.2.0 REQUIRED)

include_directories("src")
include_directories("src/Connection")
include_directories("src/Message")
include_directories("src/NetModel")
include_directories("src/UI")
include_directories("src/types")
# include_directories("src/QTstuff")

add_executable(${PROJECT_NAME} 
    src/main.cpp 
    ${HEADERS} 
    ${SOURCES}
    ${Boost_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME}
    # Qt5::DBus   
    # Qt5::Widgets
    nlohmann_json::nlohmann_json
    pthread)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)