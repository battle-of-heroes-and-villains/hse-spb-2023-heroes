#ifndef BATTLE_OF_HEROES_MENU_BUTTON_HPP
#define BATTLE_OF_HEROES_MENU_BUTTON_HPP
#include <SFML/Graphics.hpp>
#include "button.hpp"
#include "event_manager.hpp"
#include "window.hpp"

namespace game_interface {
class MenuButton {
public:
    MenuButton() = default;
    ~MenuButton() = default;

    MenuButton(
        sf::Vector2f position,
        sf::Vector2f size,
        sf::Color color,
        sf::Color pressed_color,
        interface::Fonts font,
        unsigned character_size,
        const std::string &label,
        ButtonType button_type
    );

    ButtonType update(sf::Event event, Window *window);

    void render(sf::RenderWindow *window);

private:
    void set_label_size(unsigned int character_size);

    sf::RectangleShape m_rect;
    sf::Text m_label;
    sf::Color m_button_color;
    interface::Button m_button;
    ButtonType m_button_type{ButtonType::None};
};
}  // namespace game_interface

#endif  // BATTLE_OF_HEROES_MENU_BUTTON_HPP
