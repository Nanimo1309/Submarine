project(Client
        VERSION 1.0
        LANGUAGES CXX)

find_package(Qt5 COMPONENTS Quick Multimedia REQUIRED)
add_executable(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PRIVATE Base Qt5::Quick Qt5::Multimedia)

target_include_directories(${PROJECT_NAME} PRIVATE include)
add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(rcc)