#ifndef BATTLE_OF_HEROES_GAME_MENU_BAR_HPP
#define BATTLE_OF_HEROES_GAME_MENU_BAR_HPP

#include "spell.hpp"

namespace game_interface {

class GameMenuBar {
public:
    GameMenuBar(sf::Vector2f wind_size, float menu_height);
    ~GameMenuBar() = default;

    void apply_spell();
    void update(sf::Event event, Window *window);
    void render(sf::RenderWindow *window);

private:
    std::string compress_name(const std::string &name);

    sf::RectangleShape m_turn_label;
    sf::Text m_data;
    sf::Text m_mana;
    sf::Text m_opponent_mana;
    sf::Text m_username;
    sf::Text m_opponent_username;
    sf::RectangleShape m_user_icon;
    sf::RectangleShape m_opponent_icon;
    std::vector<MenuButton> m_buttons;
    int m_spells_amount;
    std::vector<Spell> m_spells;
    int m_opponents_spells_amount;
    std::vector<Spell> m_opponents_spells;

    static void align_text_origin(sf::Text &data);
    void update_turn();
    void update_mana();
};
}  // namespace game_interface

#endif  // BATTLE_OF_HEROES_GAME_MENU_BAR_HPP
