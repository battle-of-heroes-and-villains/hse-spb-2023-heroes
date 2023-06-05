#ifndef MODEL_BOARD_HPP
#define MODEL_BOARD_HPP

#include <deque>
#include <vector>
#include "cell.hpp"
#include "coordinates.hpp"

namespace game_model {

class board {
private:
    std::vector<std::vector<cell>> m_cells_matrix =
        std::vector<std::vector<cell>>(10, std::vector<cell>(10));
    const coordinates m_size = coordinates(10, 10);

    [[nodiscard]] std::vector<std::reference_wrapper<cell>>
    coordinates_to_cells(const std::vector<coordinates> &coordinates_array);
    void set_cell_coordinates(int row, int column);
    bool is_cell_empty(
        coordinates current_cell_coordinates,
        coordinates start_cell_coordinates
    );
    bool is_cell_attackable(
        const coordinates &attacking,
        const coordinates &attacked,
        int attack_range
    );

public:
    board() {
        int row_amount = m_size.get_row();
        int column_amount = m_size.get_column();
        for (int row = 0; row < row_amount; ++row) {
            for (int column = 0; column < column_amount; ++column)
                set_cell_coordinates(row, column);
            m_cells_matrix[row][0].set_player_index(0);
            m_cells_matrix[row][column_amount - 1].set_player_index(1);
            m_cells_matrix[row][0].set_unit_index(row);
            m_cells_matrix[row][column_amount - 1].set_unit_index(row);
        }
    }

    [[nodiscard]] const coordinates &get_size() const;
    [[nodiscard]] cell &get_cell(const coordinates &cell_coordinates);
    [[nodiscard]] cell get_cell_copy(const coordinates &cell_coordinates) const;
    [[nodiscard]] std::vector<std::reference_wrapper<cell>>
    get_reachable_cells(coordinates cell_coordinates, int max_distance);
    [[nodiscard]] std::vector<std::reference_wrapper<cell>>
    get_attackable_cells(coordinates cell_coordinates, int max_distance);
    [[nodiscard]] std::vector<std::reference_wrapper<cell>>
    get_spellable_cells(int player_id, int spell_id);
};

}  // namespace game_model

#endif  // MODEL_BOARD_HPP
