#include "event_manager.hpp"
#include "client.hpp"
#include "game.hpp"

namespace game_interface {
void EventManager::update_cell(
    EventType event_type,
    Unit **selected_unit,
    Unit **unit,
    Coords clicked_position
) {
    if (get_client_state()->m_user.user().id() !=
        get_client_state()->m_game_state.move_turn()) {
        return;
    }
    if (event_type == EventType::FirstPress) {
        namespace_proto::Cell selected_cell;
        selected_cell.set_row(clicked_position.get_row());
        selected_cell.set_column(clicked_position.get_column());
        bool is_second =
            (get_client_state()->m_game_state.second_user() ==
             get_client_state()->m_user.user().id());
        if (is_second) {
            selected_cell = reverse_cell(selected_cell);
        }
        get_game_state()->get_board()->add_available_for_moving_cells(
            Client::select_unit(selected_cell)
        );
        if (*selected_unit != nullptr) {
            (*selected_unit)->disable_selection();
        }
        *selected_unit = *unit;
        (*unit)->set_selection();
    } else if (event_type == EventType::SecondPress) {
        get_game_state()->get_board()->remove_available_for_moving_cells();
        *selected_unit = nullptr;
        (*unit)->disable_selection();
    } else if (event_type == EventType::Move) {
        namespace_proto::Cell to;
        to.set_row(clicked_position.get_row());
        to.set_column(clicked_position.get_column());
        namespace_proto::Cell from;
        from.set_row((*selected_unit)->get_coords().get_row());
        from.set_column((*selected_unit)->get_coords().get_column());
        bool is_second =
            (get_client_state()->m_game_state.second_user() ==
             get_client_state()->m_user.user().id());
        if (is_second) {
            from = reverse_cell(from);
            to = reverse_cell(to);
        }
        get_game_state()->get_board()->play_animation(
            {(*selected_unit)->get_coords().get_row(),
             (*selected_unit)->get_coords().get_column()}
        );
        Client::move_unit(from, to);

        get_game_state()->get_board()->remove_available_for_moving_cells();
        get_game_state()->get_board()->update_board(
            get_client_state()->m_game_state
        );
        (*selected_unit)->disable_selection();
        *selected_unit = nullptr;
    }
}

void EventManager::update_game_menu(ButtonType button_type, Window *window) {
    if (button_type == ButtonType::Exit) {
        window->set_is_done();
    }
}

void EventManager::apply_spell(int spell_id, int row, int column) {
    if (get_client_state()->m_user.user().id() !=
        get_client_state()->m_game_state.move_turn()) {
        return;
    }
    if (Client::get_mana() >=
        get_client_state()->m_hero.spells(spell_id).mana()) {
        namespace_proto::Cell cell;
        cell.set_row(row);
        cell.set_column(column);
        bool is_second =
            (get_client_state()->m_game_state.second_user() ==
             get_client_state()->m_user.user().id());
        if (is_second) {
            cell = reverse_cell(cell);
        }
        Client::do_spell(spell_id, cell);
    }
}
}  // namespace game_interface