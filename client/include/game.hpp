#ifndef BATTLE_OF_HEROES_GAME_HPP
#define BATTLE_OF_HEROES_GAME_HPP

#include "game_menu_bar.hpp"
#include "resource_manager.hpp"

namespace game_interface {
class Game {
public:
    Game();
    ~Game() = default;

    void music_stop();
    void music_play();
    sf::Sound::Status get_music_status() const;

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
    sf::Sound m_soundtrack;
};

[[nodiscard]] Game *get_game_state();
}  // namespace game_interface

#endif  // BATTLE_OF_HEROES_GAME_HPP
