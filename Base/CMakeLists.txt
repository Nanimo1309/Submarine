project(Base
        VERSION 1.0
        LANGUAGES CXX)

find_package(Qt5 COMPONENTS Network REQUIRED)
add_library(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PUBLIC Qt5::Network)

target_include_directories(${PROJECT_NAME} PUBLIC include)
add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(rcc)