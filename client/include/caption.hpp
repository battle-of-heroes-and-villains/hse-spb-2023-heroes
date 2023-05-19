#ifndef BATTLE_OF_HEROES_CAPTION_HPP
#define BATTLE_OF_HEROES_CAPTION_HPP

#include <SFML/Graphics.hpp>
#include "event_manager.hpp"

namespace menu_interface {
class Caption {
public:
    Caption() = default;
    ~Caption() = default;
    Caption(
        sf::Vector2f position,
        sf::Vector2f size,
        interface::Fonts font,
        unsigned character_size,
        const std::string &text,
        PageType m_current_page
    );

    [[nodiscard]] PageType get_current_page() const;
    void update_text(const std::string &text);
    void render(sf::RenderWindow *window) const;

private:
    sf::RectangleShape m_table;
    sf::Text m_data;
    PageType m_current_page{PageType::SinglePlayer};
};
}  // namespace menu_interface

#endif  // BATTLE_OF_HEROES_CAPTION_HPP
