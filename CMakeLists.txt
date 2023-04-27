cmake_minimum_required(VERSION 3.0)
project(lliurex-screen-mirroring)

add_subdirectory(src)

set(langs es ca@valencia)
find_package(Intl)
find_package(Gettext)

foreach(lang ${langs})
    gettext_process_po_files(${lang} ALL PO_FILES locale/${lang}.po)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${lang}.gmo DESTINATION
            share/locale/${lang}/LC_MESSAGES/ RENAME lliurex-screen-mirroring.mo)
endforeach()

install(FILES rsrc/lliurex-screen-mirroring.desktop
    DESTINATION share/applications/
)
