cmake_minimum_required(VERSION 3.18)

if(NOT WIN32 AND NOT UNIX OR APPLE)
    message(FATAL_ERROR "Only Windows and Linux are supported")
endif()

project(Submarine
        VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_subdirectory(Base)
add_subdirectory(Client)
add_subdirectory(Server)

add_custom_target(${PROJECT_NAME}
                  DEPENDS Client Server)

add_custom_target(${PROJECT_NAME}Run
                  DEPENDS ${PROJECT_NAME}
                  COMMAND $<IF:$<BOOL:${WIN32}>,start,gnome-terminal> ARGS $<$<NOT:$<BOOL:${WIN32}>>:--> "${CMAKE_CURRENT_BINARY_DIR}/Server/Server"
                  COMMAND $<IF:$<BOOL:${WIN32}>,start,gnome-terminal> ARGS $<$<NOT:$<BOOL:${WIN32}>>:--> "${CMAKE_CURRENT_BINARY_DIR}/Client/Client")