#ifndef BATTLE_OF_HEROES_SPELL_HPP
#define BATTLE_OF_HEROES_SPELL_HPP

#include <SFML/Graphics.hpp>
#include "menu_button.hpp"
#include "window.hpp"

namespace game_interface {
class Spell {
public:
    Spell() = default;
    Spell(
        sf::Vector2f position,
        sf::Vector2f size,
        sf::Color color,
        sf::Color pressed_color,
        const namespace_proto::Spell &spell
    );
    ~Spell() = default;

    void set_default_state();
    void update(sf::Event event, Window *window);
    void render(sf::RenderWindow *window);

private:
    sf::RectangleShape m_table;
    sf::RectangleShape m_icon;
    sf::Text m_name;
    sf::Text m_description;
    sf::Color m_button_color;
    interface::Button m_button;
    int m_id;
    bool m_is_name_showed;
    sf::Sound m_click_sound;
};
}  // namespace game_interface

#endif  // BATTLE_OF_HEROES_SPELL_HPP
