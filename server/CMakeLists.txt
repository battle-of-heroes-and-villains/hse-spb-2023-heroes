set(SOURCES
        src/server.cpp
        src/game_session.cpp
        src/main.cpp
        model/src/board.cpp
        model/src/cell.cpp
        model/src/coordinates.cpp
        model/src/game.cpp
        model/src/player.cpp
        model/src/unit.cpp
        model/src/bfs_state.cpp
        model/src/spell.cpp
        model/src/hero.cpp
        interactors/src/move_interactor.cpp
        interactors/src/move_select_interactor.cpp
        interactors/src/attack_interactor.cpp
        interactors/src/attack_select_interactor.cpp
        interactors/src/spell_interactor.cpp
        interactors/src/spell_select_interactor.cpp
        game_bot/game_bot.cpp)

add_executable(server ${SOURCES})

find_package(SQLite3)
find_package(Boost)

target_include_directories(server PRIVATE
        "${PROJECT_BINARY_DIR}"
        "${CMAKE_CURRENT_SOURCE_DIR}/model/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/model/const_game_info"
        "${CMAKE_CURRENT_SOURCE_DIR}/interactors/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/network/include")

set_target_properties(server
        PROPERTIES
        INSTALL_RPATH
        "${CMAKE_INSTALL_PREFIX}/lib"
        )

target_link_libraries(server
        all_protos
        sqlite3
        )

install(TARGETS server DESTINATION bin)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})

