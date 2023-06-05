#include "game.hpp"
#include "board.hpp"

namespace game_interface {
Game::Game()
    : m_window("Battle of Heroes and Villains", sf::Vector2u(1920, 1080)),
      m_game_menu_bar(sf::Vector2f(1920, 1080), 100),
      m_board(sf::Vector2i(1920, 980)),
      m_soundtrack(
          ResourceManager::load_sound(interface::Sounds::GameBackgroundSound)
      ) {
    m_background.setTexture(game_interface::ResourceManager::load_cell_texture(
        game_interface::CellType::Type1
    ));
    m_background.setPosition(0, 0);
}

void Game::update() {
    sf::Event event{};
    while (m_window.get_render_window()->pollEvent(event)) {
        m_window.update(event);
        m_game_menu_bar.update(event, &m_window);
        m_board.handling_event(event, m_window.get_render_window());
    }
}

void Game::render() {
    m_window.begin_draw();
    if (m_soundtrack.getStatus() == sf::SoundSource::Paused ||
        m_soundtrack.getStatus() == sf::SoundSource::Stopped) {
        m_soundtrack.play();
    }
    m_window.get_render_window()->clear(sf::Color(164, 176, 126));
    m_board.render(m_window.get_render_window());
    m_game_menu_bar.render(m_window.get_render_window());
    m_window.end_draw();
}

[[nodiscard]] Window *Game::get_window() {
    return &m_window;
}

[[nodiscard]] Board *Game::get_board() {
    return &m_board;
}

[[nodiscard]] GameMenuBar *Game::get_game_menu_bar() {
    return &m_game_menu_bar;
}

void Game::music_stop() {
    m_soundtrack.pause();
}

[[nodiscard]] Game *get_game_state() {
    static Game game_state;
    return &game_state;
}
}  // namespace game_interface