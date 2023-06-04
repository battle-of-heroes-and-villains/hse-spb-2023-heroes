#include "../network/include/server.hpp"

void dump_game(GameSession *game_session) {
    namespace_proto::GameState *game_state_ref = game_session->get_game_state();
    game_state_ref->set_is_active(true);
    game_model::game *model_game = game_session->get_model_game();
    int unit_count = 0;
    for (int i = 0; i < 100; i++) {
        namespace_proto::Cell *new_cell = game_state_ref->add_game_cells();
        new_cell->set_allocated_unit(nullptr);
        int column = i % 10;
        int row = i / 10;
        new_cell->set_column(column);
        new_cell->set_row(row);
        game_model::coordinates cell_coordinates{row, column};
        game_model::cell cell = model_game->get_cell(cell_coordinates);
        new_cell->set_durability(cell.get_durability());
        if (cell.get_unit_index() != -1) {
            game_model::unit model_unit =
                model_game->get_player(cell.get_player_index())
                    ->get_unit(cell.get_unit_index());
            auto *unit = new namespace_proto::Unit;
            unit->set_id_unit(unit_count++);
            if (cell.get_player_index() == 0) {
                unit->set_id_hero(game_session->get_first_player().get_id());
            } else {
                unit->set_id_hero(game_session->get_second_player().get_id());
            }
            unit->set_type_unit(model_unit.get_type());
            unit->set_amount_unit(model_unit.get_number());
            unit->set_sum_of_health(model_unit.get_health());
            unit->set_damage(model_unit.get_damage());
            unit->set_attack_range(model_unit.get_attack_range());
            unit->set_movement_range(model_unit.get_movement_range());
            unit->set_weight(model_unit.get_weight());
            new_cell->set_allocated_unit(unit);
            new_cell->set_is_unit(true);
        }
    }
}

void start_game_session(int game_id) {
    GameSession *game_session = &(get_server_state()->game_sessions[game_id]);

    Player first_player = game_session->get_first_player();
    Player second_player = game_session->get_second_player();
    std::unordered_map<int, TSQueue<namespace_proto::GameState>>
        *response_queues_ref = game_session->get_response_queues();
    namespace_proto::GameState *game_state_ref = game_session->get_game_state();
    game_state_ref->set_first_user(first_player.get_id());
    game_state_ref->set_second_user(second_player.get_id());

    dump_game(game_session);

    game_state_ref->set_first_user_mana(10);
    game_state_ref->set_second_user_mana(10);
    game_state_ref->set_move_turn(first_player.get_id());

    game_state_ref->set_game_id(game_id);
    (*response_queues_ref)[first_player.get_id()].push(*game_state_ref);
    (*response_queues_ref)[second_player.get_id()].push(*game_state_ref);

    std::thread first_writer([response_queues_ref, &first_player]() {
        while (true) {
            if ((*response_queues_ref)[first_player.get_id()].size() > 0) {
                namespace_proto::GameState response =
                    (*response_queues_ref)[first_player.get_id()].pop();
                first_player.get_stream()->Write(response);
            }
        }
    });

    if (second_player.get_id() == -2) {
        game_session->set_type(true);
        first_writer.join();
        return;
    }

    std::thread second_writer([response_queues_ref, &second_player]() {
        while (true) {
            if ((*response_queues_ref)[second_player.get_id()].size() > 0) {
                namespace_proto::GameState response =
                    (*response_queues_ref)[second_player.get_id()].pop();
                second_player.get_stream()->Write(response);
            }
        }
    });
    first_writer.join();
    second_writer.join();
}