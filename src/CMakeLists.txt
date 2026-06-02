
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt6 "6.7.0" REQUIRED COMPONENTS Core Widgets Quick DBus)

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fexceptions")
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -foperator-names")

find_package(EdupalsBase REQUIRED)
find_package(EdupalsN4D REQUIRED)

#include(KDEInstallDirs)

add_executable(llx-screen-mirroring proxy.cpp main.cpp resources.qrc)
target_link_libraries(llx-screen-mirroring Qt6::Core Qt6::Widgets Qt6::Quick Qt6::DBus Edupals::Base Edupals::N4D)

install(TARGETS llx-screen-mirroring RUNTIME DESTINATION bin)

add_custom_target(
    po
    COMMAND xgettext -kde -ki18nd:2 ${CMAKE_CURRENT_SOURCE_DIR}/ui.qml -o ${CMAKE_CURRENT_SOURCE_DIR}/messages.po
    COMMENT "extracting po..." 
)
