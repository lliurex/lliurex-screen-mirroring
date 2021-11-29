cmake_minimum_required(VERSION 3.0)
project(lliurex-screen-mirroring)

add_subdirectory(src)

add_custom_target(
    poqml
    COMMAND xgettext -kde -ki18nd:2 ${CMAKE_CURRENT_SOURCE_DIR}/rsrc/*.qml  -o ${CMAKE_CURRENT_SOURCE_DIR}/messages.po
    COMMENT "extracting po..." 
)
