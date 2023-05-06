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
    explicit Cell(
        Coords coords,
        CellType type,
        sf::Vector2f position,
        sf::Vector2f size,
        int strength = 0,
        Unit *unit = nullptr
    );

    bool is_have_unit() const;
    [[nodiscard]] Unit *get_unit();
    void set_unit(Unit *unit);
    void add_selection();
    void remove_selection();
    void handling_event(
        Unit **selected_unit,
        Board *board,
        sf::Event event,
        sf::Window *window
    );
    CellEventType is_mouse_target(sf::Window *window);
    void update_cell(const namespace_proto::Cell &cell);
    void render(sf::RenderWindow *window);

private:
    sf::Sprite m_cell;
    Coords m_coords;
    CellType m_cell_type{CellType::Default};
    Unit *m_unit{nullptr};
    int m_strength{};
    bool m_is_available_for_moving{};
    interface::Button m_button;
    sf::Text m_label;
};

namespace_proto::Cell reverse_cell(namespace_proto::Cell cell);
}  // namespace game_interface

#endif  // BATTLE_OF_HEROES_CELL_HPP
