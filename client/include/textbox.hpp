#ifndef BATTLE_OF_HEROES_TEXTBOX_HPP
#define BATTLE_OF_HEROES_TEXTBOX_HPP

#include <SFML/Graphics.hpp>
#include "button.hpp"
#include "event_manager.hpp"

namespace menu_interface {
class TextBox {
public:
    TextBox() = default;
    ~TextBox() = default;
    TextBox(
        sf::Vector2f position,
        sf::Vector2f size,
        interface::Fonts font,
        unsigned character_size,
        bool is_active
    );

    std::string get_data() const;
    bool is_active() const;
    bool is_showed() const;
    void activate();
    void hide_data();
    void show_data();
    void clear();
    bool update(sf::Event event, game_interface::Window *window);
    void render(sf::RenderWindow *window) const;

private:
    sf::RectangleShape m_table;
    sf::String m_data;
    sf::String m_hidden_data;
    bool m_is_active{false};
    bool m_is_showed{true};
    sf::Text m_label;
    interface::Button m_button;

    sf::Sound m_type_sound;
};
}  // namespace menu_interface

#endif  // BATTLE_OF_HEROES_TEXTBOX_HPP
