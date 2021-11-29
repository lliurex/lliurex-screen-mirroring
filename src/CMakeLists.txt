
find_package(ECM 1.0.0 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt5 REQUIRED COMPONENTS Core Widgets Quick)

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fexceptions")
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -foperator-names")

find_package(EdupalsBase REQUIRED)

include(KDEInstallDirs)

add_executable(llx-screen-mirroring main.cpp resources.qrc)
target_link_libraries(llx-screen-mirroring Qt5::Core Qt5::Widgets Qt5::Quick Edupals::Base)

install(TARGETS llx-screen-mirroring RUNTIME DESTINATION bin)

add_custom_target(
    po
    COMMAND xgettext --keyword=T --language=C++ ${CMAKE_CURRENT_SOURCE_DIR}/harvesterstudentplugin.cpp ${CMAKE_CURRENT_SOURCE_DIR}/harvesterteacherplugin.cpp -o ${CMAKE_CURRENT_SOURCE_DIR}/messages.po
    COMMENT "extracting po..." 
)
