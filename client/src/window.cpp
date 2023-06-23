#include "window.hpp"
#include <utility>

namespace game_interface {
Window::Window(std::string title, const sf::Vector2u &size) {
    setup(std::move(title), size);
}

Window::~Window() {
    destroy();
}

void Window::create() {
    auto style = (m_is_fullscreen ? sf::Style::Fullscreen : sf::Style::Default);
    m_window.create(
        {m_window_size.x, m_window_size.y, 32}, m_window_title, style
    );
}

void Window::setup(std::string title, sf::Vector2u size) {
    m_window_title = std::move(title);
    m_window_size = size;
    m_is_done = false;
    m_is_fullscreen = false;
    m_window.setFramerateLimit(60);
    create();
}

void Window::destroy() {
    m_window.close();
}

void Window::begin_draw() {
    m_window.clear(sf::Color::Black);
}

void Window::end_draw() {
    m_window.display();
}

[[nodiscard]] sf::RenderWindow *Window::get_render_window() {
    return &m_window;
}

[[nodiscard]] bool Window::is_done() const {
    return m_is_done;
}

void Window::set_is_done() {
    m_is_done = true;
    destroy();
}

void Window::toggle_fullscreen() {
    m_is_fullscreen = !m_is_fullscreen;
    destroy();
    create();
}

void Window::update(sf::Event event) {
    if (event.type == sf::Event::Closed) {
        m_is_done = true;
    } else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F5) {
        toggle_fullscreen();
    }
}

void Window::render(sf::Drawable &l_drawable) {
    m_window.draw(l_drawable);
}

void Window::reopen(std::string title, const sf::Vector2u &size) {
    setup(std::move(title), size);
}
}  // namespace game_interface