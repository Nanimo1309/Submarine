cmake_minimum_required(VERSION 3.18)

if(NOT WIN32 AND NOT UNIX OR APPLE)
    message(FATAL_ERROR "Only Windows and Linux are supported")
endif()

project(Submarine
        VERSION 0.1
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_subdirectory(Base)
add_subdirectory(Client)
add_subdirectory(Server)

add_custom_target(${PROJECT_NAME}
                  DEPENDS Client Server)

add_custom_target(${PROJECT_NAME}Run
                  DEPENDS ${PROJECT_NAME} ClientRun ServerRun)
