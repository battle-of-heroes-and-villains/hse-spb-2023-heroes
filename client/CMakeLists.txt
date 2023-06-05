cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)

set(SOURCES
        src/animation.cpp
        src/board.cpp
        src/button.cpp
        src/caption.cpp
        src/cell.cpp
        src/client.cpp
        src/coordinates.cpp
        src/cursor.cpp
        src/event_manager.cpp
        src/game.cpp
        src/game_menu_bar.cpp
        src/main.cpp
        src/menu.cpp
        src/menu_button.cpp
        src/popup_window.cpp
        src/resource_manager.cpp
        src/spell.cpp
        src/textbox.cpp
        src/unit.cpp
        src/window.cpp
        )

add_executable(client ${SOURCES})


target_include_directories(client PRIVATE
        "${PROJECT_BINARY_DIR}"
        "${CMAKE_CURRENT_SOURCE_DIR}/include")

set_target_properties(client
        PROPERTIES
        INSTALL_RPATH
        "${CMAKE_INSTALL_PREFIX}/lib"
        )

target_link_libraries(client
        all_protos sfml-system sfml-window sfml-graphics sfml-audio sfml-network
        )

install(TARGETS client DESTINATION bin)

install(DIRECTORY content
        DESTINATION "${CMAKE_INSTALL_PREFIX}/share/${CMAKE_PROJECT_NAME}"
        )

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})
