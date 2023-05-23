#include "popup_window.hpp"

namespace interface {
PopUpWindow::PopUpWindow(
    sf::Vector2f position,
    sf::Vector2f size,
    interface::Fonts font,
    unsigned int character_size,
    const std::string &text
) {
    m_table.setSize(size);
    m_table.setFillColor(sf::Color(71, 78, 50));
    m_table.setPosition(position);

    m_data.setFont(game_interface::ResourceManager::load_font(font));
    m_data.setString(sf::String(text));
    m_data.setFillColor(sf::Color::White);
    m_data.setCharacterSize(character_size);

    m_data.setPosition(position);
}

void PopUpWindow::update(
    const std::string &text,
    game_interface::EventType event_type,
    const sf::Window *window
) {
    if (event_type == game_interface::EventType::Targeting) {
        m_is_active = true;
        auto mouse_position = sf::Mouse::getPosition(*window);
        m_table.setPosition(mouse_position.x + 20.0f, mouse_position.y + 20.0f);
        m_data.setPosition(mouse_position.x + 30.0f, mouse_position.y + 22.0f);
        m_data.setString(text);
    } else {
        m_is_active = false;
    }
}

void PopUpWindow::render(sf::RenderWindow *window) {
    if (m_is_active) {
        window->draw(m_table);
        window->draw(m_data);
    }
}
}  // namespace interface
