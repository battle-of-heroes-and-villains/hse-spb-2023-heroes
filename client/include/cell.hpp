#ifndef BATTLE_OF_HEROES_CELL_HPP
#define BATTLE_OF_HEROES_CELL_HPP

#include <SFML/Graphics.hpp>
#include "button.hpp"
#include "unit.hpp"

namespace game_interface {
class Board;

class Cell {
public:
    Cell() = default;
    explicit Cell(Coords coords, sf::Vector2f position, sf::Vector2f size);

    bool is_have_unit() const;
    bool is_available_for_moving() const;
    [[nodiscard]] Unit *get_unit();
    [[nodiscard]] sf::Vector2f get_cell_position();

    void set_unit(Unit *unit);

    void update_cell_durability();
    void update_cell_texture(CellType type);

    void add_selection();
    void remove_selection();
    void add_spell(int spell_id);
    void remove_spell();

    void handling_event(sf::Event event, Unit **selected_unit);
    EventType is_mouse_target(sf::Window *window);
    bool change_cursor(sf::Window *window);

    void update_cell();
    void render(sf::RenderWindow *window);

private:
    sf::Vector2f m_cell_size;
    Coords m_coords;
    CellType m_cell_type;
    CellType m_cell_property_type;
    int m_durability;
    int m_spell_id;

    sf::Sprite m_cell;
    sf::Sprite m_cell_property;
    interface::Button m_button;
    Unit *m_unit{nullptr};
    sf::Text m_label;
};

namespace_proto::Cell reverse_cell(namespace_proto::Cell cell);
}  // namespace game_interface

#endif  // BATTLE_OF_HEROES_CELL_HPP
