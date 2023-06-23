#include "menu_button.hpp"

namespace game_interface {
MenuButton::MenuButton(
    sf::Vector2f position,
    sf::Vector2f size,
    sf::Color color,
    sf::Color pressed_color,
    interface::Fonts font,
    unsigned int character_size,
    const std::string &label,
    ButtonType button_type
)
    : m_click_sound(ResourceManager::load_sound(interface::Sounds::Click)) {
    m_rect.setSize(size);
    m_rect.setFillColor(color);
    m_rect.setOrigin(size.x / 2.0f, size.y / 2.0f);
    m_rect.setPosition(position);

    m_button_color = color;
    m_rect.setOutlineColor(pressed_color);

    m_button = interface::Button(position, size);

    m_button_type = button_type;

    m_label.setFont(ResourceManager::load_font(font));
    m_label.setString(sf::String(label));
    set_label_size(character_size);

    m_label.setPosition(position);
}

ButtonType MenuButton::update(sf::Event event, Window *window) {
    auto result = m_button.handling_event(event, window->get_render_window());
    m_rect.setOutlineThickness(0);
    set_label_size(22);
    m_rect.setFillColor(m_button_color);
    if (result == EventType::FirstPress) {
        EventManager::update_game_menu(m_button_type, window);
        m_click_sound.play();
        return m_button_type;
    } else if (result == EventType::Targeting) {
        m_rect.setFillColor(m_rect.getOutlineColor());
        m_rect.setOutlineThickness(5);
        set_label_size(26);
    }
    return ButtonType::None;
}

void MenuButton::render(sf::RenderWindow *window) {
    window->draw(m_rect);
    window->draw(m_label);
}

void MenuButton::set_label_size(unsigned int character_size) {
    m_label.setCharacterSize(character_size);
    m_label.setOrigin(
        m_label.getLocalBounds().left + m_label.getLocalBounds().width / 2.0f,
        m_label.getLocalBounds().top + m_label.getLocalBounds().height / 2.0f
    );
}
}  // namespace game_interface