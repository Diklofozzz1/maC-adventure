cmake_minimum_required(VERSION 3.0.0)
project(Messager VERSION 0.1.0)

# set(HEADERS 
#     src/Ex.h
# )

set(SOURCES 
    src/Example.cpp
)

find_package (Boost REQUIRED system QUIET)

add_executable(${PROJECT_NAME} 
    src/main.cpp 
    # ${HEADERS} 
    ${SOURCES}
    ${Boost_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME}
    pthread)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)