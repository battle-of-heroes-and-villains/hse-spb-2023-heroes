#include "menu_button.hpp"

namespace game_interface {
MenuButton::MenuButton(
    sf::Vector2f position,
    sf::Vector2f size,
    sf::Color color,
    interface::Fonts font,
    unsigned int character_size,
    const std::string &label,
    ButtonType button_type
) {
    m_rect.setSize(size);
    m_rect.setFillColor(color);
    m_rect.setOrigin(size.x / 2.0f, size.y / 2.0f);
    m_rect.setPosition(position);

    m_button = interface::Button(position, size);

    m_button_type = button_type;

    m_label.setFont(ResourceManager::load_font(font));
    m_label.setString(sf::String(label));
    m_label.setCharacterSize(character_size);

    sf::FloatRect rect = m_label.getLocalBounds();
    m_label.setOrigin(
        rect.left + rect.width / 2.0f, rect.top + rect.height / 2.0f
    );

    m_label.setPosition(position);
}

ButtonType MenuButton::update(sf::Event event, Window *window) {
    if (m_button.handling_event(event, window->get_render_window()) ==
        EventType::FirstPress) {
        EventManager::update_game_menu(m_button_type, window);
        return m_button_type;
    }
    return ButtonType::None;
}

void MenuButton::render(sf::RenderWindow *window) {
    window->draw(m_rect);
    window->draw(m_label);
}
}  // namespace game_interface