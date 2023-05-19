#ifndef BATTLE_OF_HEROES_BOARD_HPP
#define BATTLE_OF_HEROES_BOARD_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "cell.hpp"
#include "spell.hpp"

namespace game_interface {
class Board {
public:
    explicit Board(sf::Vector2i window_size);
    ~Board() = default;

    [[nodiscard]] sf::Vector2f get_cell_position(Coords coords) const;

    void add_available_for_moving_cells(
        std::vector<std::pair<int, int>> selected_cells
    );
    void remove_available_for_moving_cells();
    void add_enable_for_spelling_cells(
        std::vector<std::pair<int, int>> selected_cells,
        int spell_id
    );

    void play_animation(Coords source_cell, Coords destination_cell);

    void remove_enable_for_spelling_cells();
    void handling_event(sf::Event event, sf::Window *window);
    void update_board(const namespace_proto::GameState &game_state);
    void render(sf::RenderWindow *window);

private:
    std::vector<std::vector<Cell>> m_board;
    sf::Vector2i m_window_size;
    sf::Vector2i m_cell_size;
    sf::Vector2i m_boarder_size;
    int m_cell_amount;
    std::vector<Unit> m_units;
    std::vector<bool> m_unit_is_updated;
    std::vector<bool> m_unit_is_alive;
    Unit *selected_unit;
    std::vector<std::pair<int, int>> m_available_for_moving_cells;
    std::vector<std::pair<int, int>> m_enable_for_spelling_cells;
};
}  // namespace game_interface
#endif  // BATTLE_OF_HEROES_BOARD_HPP
