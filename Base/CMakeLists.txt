project(Base
        VERSION ${CMAKE_PROJECT_VERSION}
        LANGUAGES CXX)

find_package(Qt6 COMPONENTS Core REQUIRED)
add_library(${PROJECT_NAME})

configure_file(Info.hpp.in Info.hpp)

file(GLOB hpp *.hpp "${CMAKE_CURRENT_BINARY_DIR}/Info.hpp")
file(GLOB cpp *.cpp)
file(GLOB rcc "")

target_sources(${PROJECT_NAME} PUBLIC ${hpp} ${cpp} ${rcc})