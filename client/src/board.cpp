#include "board.hpp"
#include <utility>
#include "game.hpp"
#include "resource_manager.hpp"

namespace game_interface {
Board::Board(sf::Vector2i window_size) {
    m_cell_amount = 10;
    m_window_size = window_size;
    m_boarder_size.x = static_cast<int>(m_window_size.x * 0.07);
    m_boarder_size.y = static_cast<int>(m_window_size.y * 0.07);
    m_cell_size.x = (m_window_size.x - 2 * m_boarder_size.x) / m_cell_amount;
    m_cell_size.y = (m_window_size.y - m_boarder_size.y) / m_cell_amount;
    m_board.resize(m_cell_amount, std::vector<Cell>(m_cell_amount));
    m_units.resize(20);
    m_unit_is_updated.resize(20, false);
    m_unit_is_alive.resize(20, true);
    selected_unit = nullptr;
    for (int row = 0; row < m_cell_amount; row++) {
        for (int column = 0; column < m_cell_amount; column++) {
            m_board[row][column] = Cell(
                Coords(row, column),
                sf::Vector2f(
                    m_boarder_size.x + m_cell_size.x * column,
                    m_boarder_size.y + m_cell_size.y * row
                ),
                sf::Vector2f(m_cell_size)
            );
        }
    }
}

[[nodiscard]] sf::Vector2f Board::get_cell_position(Coords coords) const {
    return {
        static_cast<float>(
            m_boarder_size.x + m_cell_size.x * coords.get_column()
        ),
        static_cast<float>(
            m_boarder_size.y + m_cell_size.y * coords.get_row()
        )};
}

void Board::play_animation(Coords source_cell, Coords destination_cell) {
    if (m_board[destination_cell.get_row()][destination_cell.get_column()]
            .is_have_unit() &&
        m_board[destination_cell.get_row()][destination_cell.get_column()]
                .get_unit()
                ->get_hero_id() != get_client_state()->m_user.user().id()) {
        m_board[source_cell.get_row()][source_cell.get_column()]
            .get_unit()
            ->play_animation(AnimationType::Attack);
        if (m_board[destination_cell.get_row()][destination_cell.get_column()].get_unit()->get_health() <=
            m_board[source_cell.get_row()][source_cell.get_column()]
                .get_unit()
                ->get_damage()) {
            m_board[destination_cell.get_row()][destination_cell.get_column()]
                .get_unit()
                ->play_animation(AnimationType::Dead);
        } else {
            m_board[destination_cell.get_row()][destination_cell.get_column()]
                .get_unit()
                ->play_animation(AnimationType::GetAttacked);
        }

    } else {
        m_board[source_cell.get_row()][source_cell.get_column()]
            .get_unit()
            ->play_animation(AnimationType::Move, destination_cell);
    }
}

void Board::add_available_for_moving_cells(
    std::vector<std::pair<int, int>> selected_cells
) {
    remove_available_for_moving_cells();
    m_available_for_moving_cells = std::move(selected_cells);
    for (auto [row, column] : m_available_for_moving_cells) {
        bool is_second =
            (get_client_state()->m_game_state.second_user() ==
             get_client_state()->m_user.user().id());
        if (is_second) {
            column = 9 - column;
        }
        m_board[row][column].add_selection();
    }
}

void Board::remove_available_for_moving_cells() {
    for (auto [row, column] : m_available_for_moving_cells) {
        bool is_second =
            (get_client_state()->m_game_state.second_user() ==
             get_client_state()->m_user.user().id());
        if (is_second) {
            column = 9 - column;
        }
        m_board[row][column].remove_selection();
    }
    m_available_for_moving_cells.clear();
}

void Board::add_enable_for_spelling_cells(
    std::vector<std::pair<int, int>> enable_cells,
    int spell_id
) {
    remove_enable_for_spelling_cells();
    m_enable_for_spelling_cells = std::move(enable_cells);
    for (auto [row, column] : m_enable_for_spelling_cells) {
        bool is_second =
            (get_client_state()->m_game_state.second_user() ==
             get_client_state()->m_user.user().id());
        if (is_second) {
            column = 9 - column;
        }
        m_board[row][column].add_spell(spell_id);
    }
}

void Board::remove_enable_for_spelling_cells() {
    for (auto [row, column] : m_enable_for_spelling_cells) {
        bool is_second =
            (get_client_state()->m_game_state.second_user() ==
             get_client_state()->m_user.user().id());
        if (is_second) {
            column = 9 - column;
        }
        m_board[row][column].remove_spell();
    }
    m_enable_for_spelling_cells.clear();
}

void Board::handling_event(sf::Event event, sf::Window *window) {
    int row =
        (sf::Mouse::getPosition(*window).y - m_boarder_size.y) / m_cell_size.y;
    int column =
        (sf::Mouse::getPosition(*window).x - m_boarder_size.x) / m_cell_size.x;
    if (column >= 0 && row >= 0 && row < m_cell_amount &&
        column < m_cell_amount) {
        m_board[row][column].handling_event(event, &selected_unit);
    }
}

void Board::render(sf::RenderWindow *window) {
    bool is_cursor_changed = false;
    for (auto &row : m_board) {
        for (auto &cell : row) {
            if (cell.is_have_unit()) {
                cell.get_unit()->update_statistic(
                    cell.is_mouse_target(window), window
                );
            }
            is_cursor_changed = is_cursor_changed || cell.change_cursor(window);
            cell.render(window);
        }
    }
    if (!is_cursor_changed) {
        get_cursor().loadFromSystem(sf::Cursor::Arrow);
        window->setMouseCursor(get_cursor());
    }
    for (int unit_id = 0; unit_id < m_units.size(); unit_id++) {
        if (m_unit_is_alive[unit_id]) {
            m_units[unit_id].render(window);
        } else {
            m_units[unit_id].render_animation(window);
        }
    }

    for (int unit_id = 0; unit_id < m_units.size(); unit_id++) {
        if (m_unit_is_alive[unit_id]) {
            m_units[unit_id].render_statistic(window);
        }
    }
}

void Board::update_board(const namespace_proto::GameState &game_state) {
    bool is_second =
        (game_state.second_user() == get_client_state()->m_user.user().id());
    std::fill(m_unit_is_updated.begin(), m_unit_is_updated.end(), false);
    for (int cell_index = 0; cell_index < 100; cell_index++) {
        namespace_proto::Cell server_cell = game_state.game_cells(cell_index);
        int row = server_cell.row();
        int column = server_cell.column();
        if (is_second) {
            column = 9 - column;
            server_cell = reverse_cell(server_cell);
        }
        m_board[row][column].update_cell();
        if (server_cell.is_unit()) {
            int unit_id = server_cell.unit().id_unit();
            auto server_unit = game_state.game_cells(cell_index).unit();
            m_units[unit_id].update_unit(
                server_cell, server_unit,
                m_board[row][column].get_cell_position(),
                static_cast<sf::Vector2f>(m_cell_size)
            );
            m_board[row][column].set_unit(&m_units[unit_id]);
            m_unit_is_updated[unit_id] = true;
        } else {
            m_board[row][column].set_unit(nullptr);
        }
    }
    for (int unit_id = 0; unit_id < m_units.size(); unit_id++) {
        if (!m_unit_is_updated[unit_id]) {
            m_unit_is_alive[unit_id] = false;
        }
    }
}
}  // namespace game_interface