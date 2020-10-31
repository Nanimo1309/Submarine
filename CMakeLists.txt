cmake_minimum_required(VERSION 3.18)

project(Submarine
        VERSION 1.0
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

if(NOT BUILD_SHARED_LIBS)
    add_link_options(-static-libstdc++)
endif()

add_subdirectory(Base)
add_subdirectory(Server)
add_subdirectory(Client)

# cmake -B bin -DBUILD_SHARED_LIBS=ON -DCMAKE_PREFIX_PATH="/opt/Qt/5.15.1/gcc_64" && cmake --build bin -t Client Server && gnome-terminal -- bin/Client/Client && gnome-terminal -- bin/Server/Server
