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

    void remove();
    void set_name();
    void update(sf::Event event, Window *window);
    void render(sf::RenderWindow *window);

private:
    void set_label_size(unsigned int character_size);

    sf::RectangleShape m_table;
    sf::Text m_data;
    sf::Color m_button_color;
    interface::Button m_button;
    int m_id;
    std::string m_name;
    std::string m_description;
    bool m_is_name_showed;

    void update_data();
};
}  // namespace game_interface

#endif  // BATTLE_OF_HEROES_SPELL_HPP
