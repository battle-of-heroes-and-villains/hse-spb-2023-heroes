#ifndef BATTLE_OF_HEROES_WINDOW_HPP
#define BATTLE_OF_HEROES_WINDOW_HPP
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <string>
#include <utility>

namespace game_interface {
class Window {
public:
    Window() = default;
    Window(std::string title, const sf::Vector2u &size);
    ~Window();

    void begin_draw();
    void end_draw();
    [[nodiscard]] sf::RenderWindow *get_render_window();
    [[nodiscard]] bool is_done() const;
    void set_is_done();
    void toggle_fullscreen();
    void update(sf::Event event);
    void render(sf::Drawable &l_drawable);

    void reopen();

private:
    void create();
    void setup(std::string title, sf::Vector2u size);
    void destroy();

    sf::RenderWindow m_window;
    sf::Vector2u m_window_size;
    std::string m_window_title;
    bool m_is_done{};
    bool m_is_fullscreen{};
};
}  // namespace game_interface

#endif  // BATTLE_OF_HEROES_WINDOW_HPP
