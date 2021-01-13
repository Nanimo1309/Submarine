project(Client
        VERSION ${CMAKE_PROJECT_VERSION}
        LANGUAGES CXX)

find_package(Qt5 COMPONENTS Network REQUIRED)
add_executable(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PRIVATE Base Qt5::Network)

add_custom_target(${PROJECT_NAME}Run
                  DEPENDS ${PROJECT_NAME}
                  COMMAND $<IF:$<BOOL:${WIN32}>,start,gnome-terminal> ARGS $<$<NOT:$<BOOL:${WIN32}>>:--> "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}")

file(GLOB hpp *.hpp)
file(GLOB cpp *.cpp)
file(GLOB rcc "")

target_sources(${PROJECT_NAME} PRIVATE ${hpp} ${cpp} ${rcc})