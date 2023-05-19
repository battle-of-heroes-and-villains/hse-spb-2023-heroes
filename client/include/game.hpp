#ifndef BATTLE_OF_HEROES_GAME_HPP
#define BATTLE_OF_HEROES_GAME_HPP

#include "game_menu_bar.hpp"
#include "resource_manager.hpp"

namespace game_interface {
class Game {
public:
    Game();
    ~Game() = default;

    void update();
    void render();
    [[nodiscard]] Window *get_window();
    [[nodiscard]] Board *get_board();
    [[nodiscard]] GameMenuBar *get_game_menu_bar();

private:
    Window m_window;
    sf::Sprite m_background;
    GameMenuBar m_game_menu_bar;
    Board m_board;
};

[[nodiscard]] Game *get_game_state();

[[nodiscard]] sf::Cursor &get_cursor();
}  // namespace game_interface

#endif  // BATTLE_OF_HEROES_GAME_HPP
