#ifndef SERVER_HPP_
#define SERVER_HPP_

#include <grpc/grpc.h>
#include <grpcpp/server_builder.h>
#include <atomic>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <random>
#include <string>
#include <thread>
#include <utility>
#include <vector>
#include "db_interface.hpp"
#include "game_session.hpp"
#include "heroes_list.hpp"

class ServerState {
public:
    db data_base;
    std::vector<GameSession> game_sessions;
    TSQueue<Player> pairing_wait_list;
    TSQueue<Player> wait_list;
    mutable std::mutex server_mutex;
    mutable std::mutex db_mutex;
};

ServerState *get_server_state();

int my_rand();

class ServerServices final : public ::namespace_proto::Server::Service {
    ::grpc::Status SignUp(
        ::grpc::ServerContext *context,
        const namespace_proto::LogInData *request,
        namespace_proto::User *response
    ) override {
        int id = rand();
        get_server_state()->data_base.insert(
            request->name().c_str(), request->password().c_str(), 1000
        );
        response->set_id(id);
        response->set_rate(1000);
        response->set_name(request->name());
        return grpc::Status::OK;
    }

    ::grpc::Status LogIn(
        ::grpc::ServerContext *context,
        const namespace_proto::LogInData *request,
        namespace_proto::User *response
    ) override {
        int id = rand();
        std::unique_lock lock{get_server_state()->db_mutex};
        get_server_state()->data_base.get_user(request->name());
        if (callback_data.password == request->password()) {
            response->set_id(id);
            response->set_rate(callback_data.rating);
            response->set_name(request->name());
        } else {
            response->set_id(-1);
        }
        return grpc::Status::OK;
    }

    static void set_move(
        namespace_proto::OpponentMove *move,
        namespace_proto::Enum enum_,
        namespace_proto::Cell *from,
        namespace_proto::Cell *to = nullptr
    ) {
        move->set_enum_(enum_);
        auto cell_from = new namespace_proto::Cell(*from);
        move->set_allocated_from(cell_from);
        if (to != nullptr) {
            auto cell_to = new namespace_proto::Cell(*to);
            move->set_allocated_to(cell_to);
        }
    }

    static namespace_proto::GameState *handle_diff(
        GameSession *game_session_ref,
        const namespace_proto::User &user
    ) {
        if (!(game_session_ref->get_type())) {
            if (user.id() != game_session_ref->get_first_player().get_id()) {
                (*(game_session_ref->get_response_queues())
                )[game_session_ref->get_first_player().get_id()]
                    .push(*(game_session_ref->get_game_state()));
            } else {
                (*(game_session_ref->get_response_queues())
                )[game_session_ref->get_second_player().get_id()]
                    .push(*(game_session_ref->get_game_state()));
            }
        } else {
            switch_turn(game_session_ref);
            auto item = (*game_session_ref->get_game_bot())();
            namespace_proto::GameState *game_state_ref =
                game_session_ref->get_game_state();
            namespace_proto::OpponentMove *move =
                game_state_ref->mutable_opponent_move();
            move->set_opponent_id(-2);
            if (item.get_type() == bot::bot_response_type::SINGLE_CELL) {
                game_model::cell cell = item.get_from_cell();
                int index = cell.get_coordinates().get_row() * 10 +
                            cell.get_coordinates().get_column();
                namespace_proto::Cell *proto_cell =
                    game_session_ref->get_game_state()->mutable_game_cells(index
                    );
                update_cell(
                    proto_cell, cell.get_coordinates(), game_session_ref
                );

                if (proto_cell->is_unit()) {
                    namespace_proto::Unit *unit = proto_cell->mutable_unit();
                    update_unit(unit, cell.get_coordinates(), game_session_ref);
                    int cell_to_index =
                        item.get_to_cell().get_coordinates().get_row() * 10 +
                        item.get_to_cell().get_coordinates().get_column();
                    namespace_proto::Cell *cell_to =
                        game_state_ref->mutable_game_cells(cell_to_index);
                    set_move(
                        move, namespace_proto::attack, cell_to, proto_cell
                    );
                } else {
                    set_move(move, namespace_proto::spell, proto_cell);
                }
            } else if (item.get_type() == bot::bot_response_type::PAIR_OF_CELLS) {
                game_model::cell cell_from = item.get_from_cell();
                int index_from = cell_from.get_coordinates().get_row() * 10 +
                                 cell_from.get_coordinates().get_column();
                namespace_proto::Cell *proto_cell_from =
                    game_session_ref->get_game_state()->mutable_game_cells(
                        index_from
                    );
                game_model::cell cell_to = item.get_to_cell();
                int index_to = cell_to.get_coordinates().get_row() * 10 +
                               cell_to.get_coordinates().get_column();
                namespace_proto::Cell *proto_cell_to =
                    game_session_ref->get_game_state()->mutable_game_cells(
                        index_to
                    );
                swapUnits(proto_cell_from, proto_cell_to);
                update_cell(
                    proto_cell_to, cell_to.get_coordinates(), game_session_ref
                );
                namespace_proto::Unit *unit = proto_cell_to->mutable_unit();
                update_unit(unit, cell_to.get_coordinates(), game_session_ref);
                update_cell(
                    proto_cell_from, cell_from.get_coordinates(),
                    game_session_ref
                );
                set_move(
                    move, namespace_proto::move, proto_cell_from, proto_cell_to
                );

            } else if (item.get_type() == bot::bot_response_type::GIVE_UP) {
                game_session_ref->get_second_player().get_context()->TryCancel(
                );
            }
        }
        (*(game_session_ref->get_response_queues())
        )[game_session_ref->get_first_player().get_id()]
            .push(*(game_session_ref->get_game_state()));

        return game_session_ref->get_game_state();
    }

    static void dump_hero(int hero_id, namespace_proto::Hero *hero) {
        const game_model::hero *model_hero =
            &const_game_info::HEROES_LIST[hero_id];
        hero->set_type(model_hero->get_id());
        const std::vector<game_model::spell> spells = model_hero->get_spells();
        for (const auto &item : spells) {
            auto spell = hero->add_spells();
            spell->set_id(item.get_id());
            spell->set_name(item.get_name());
            spell->set_description(item.get_description());
            spell->set_mana(item.get_mana_cost());
        }
    }

    ::grpc::Status GetHero(
        ::grpc::ServerContext *context,
        const google::protobuf::Empty *request,
        namespace_proto::Hero *response
    ) override {
        int rand_id = my_rand();
        dump_hero(rand_id, response);
        return ::grpc::Status::OK;
    }

    ::grpc::Status GetOpponent(
        grpc::ServerContext *context,
        const namespace_proto::UserState *request,
        namespace_proto::Hero *response
    ) override {
        GameSession *game_session_ref =
            &(get_server_state()->game_sessions[request->game_id()]);
        namespace_proto::GameState *game_state_ref =
            game_session_ref->get_game_state();
        if (request->user().id() ==
            game_session_ref->get_first_player().get_id()) {
            dump_hero(
                game_session_ref->get_second_player().get_hero_id(), response
            );
            response->set_name(game_session_ref->get_second_player().get_name()
            );
        } else {
            dump_hero(
                game_session_ref->get_first_player().get_hero_id(), response
            );
            response->set_name(game_session_ref->get_first_player().get_name());
        }
        return grpc::Status::OK;
    }

    ::grpc::Status SwitchTurn(
        grpc::ServerContext *context,
        const namespace_proto::UserState *request,
        namespace_proto::GameState *response
    ) override {
        GameSession *game_session_ref =
            &(get_server_state()->game_sessions[request->game_id()]);
        switch_turn(game_session_ref);
        *response = *handle_diff(game_session_ref, request->user());
        return grpc::Status::OK;
    }

    ::grpc::Status EndSession(
        grpc::ServerContext *context,
        const namespace_proto::UserState *request,
        google::protobuf::Empty *response
    ) override {
        GameSession *game_session_ref =
            &(get_server_state()->game_sessions[request->game_id()]);
        namespace_proto::GameState *game_state_ref =
            game_session_ref->get_game_state();
        game_state_ref->set_is_active(false);
        if (request->user().id() ==
            game_session_ref->get_first_player().get_id()) {
            game_state_ref->set_winner(
                game_session_ref->get_second_player().get_id()
            );
        } else {
            game_state_ref->set_winner(
                game_session_ref->get_first_player().get_id()
            );
        }
        (*(game_session_ref->get_response_queues())
        )[game_session_ref->get_first_player().get_id()]
            .push(*(game_session_ref->get_game_state()));
        (*(game_session_ref->get_response_queues())
        )[game_session_ref->get_second_player().get_id()]
            .push(*(game_session_ref->get_game_state()));
        while((*(game_session_ref->get_response_queues())
                )[game_session_ref->get_first_player().get_id()].size() > 0) {}
        if (!(game_session_ref->get_type())) {
            while ((*(game_session_ref->get_response_queues())
                   )[game_session_ref->get_second_player().get_id()]
                       .size() > 0) {
            }
        }
        auto move = std::make_unique<namespace_proto::OpponentMove>();
        move->set_enum_(namespace_proto::none);
        game_state_ref->set_allocated_opponent_move(move.get());
        game_session_ref->get_first_player().get_context()->TryCancel();
        game_session_ref->get_second_player().get_context()->TryCancel();
        return grpc::Status::OK;
    }

    ::grpc::Status CallServer(
        ::grpc::ServerContext *context,
        const namespace_proto::UserState *request,
        ::grpc::ServerWriter<::namespace_proto::GameState> *response
    ) override {
        if (!(request->is_single())) {
            get_server_state()->pairing_wait_list.push(Player{
                request->user().id(), request->hero_id(),
                request->user().name(), response, context});
        } else {
            get_server_state()->wait_list.push(Player{
                request->user().id(), request->hero_id(),
                request->user().name(), response, context});
        }
        while (!context->IsCancelled()) {
        }
        return ::grpc::Status::OK;
    }

    static void
    swapUnits(namespace_proto::Cell *cell1, namespace_proto::Cell *cell2) {
        cell1->Swap(cell2);
        int temp_row = cell1->row();
        int temp_column = cell1->column();
        int temp_strength = cell1->durability();
        cell1->set_durability(cell2->durability());
        cell2->set_durability(temp_strength);
        cell1->set_row(cell2->row());
        cell1->set_column(cell2->column());
        cell2->set_row(temp_row);
        cell2->set_column(temp_column);
    }

    static void update_mana(GameSession *game) {
        int first_player_mana =
            game->get_model_game()->get_player(0)->get_mana();
        int second_player_mana =
            game->get_model_game()->get_player(1)->get_mana();
        game->get_game_state()->set_first_user_mana(first_player_mana);
        game->get_game_state()->set_second_user_mana(second_player_mana);
    }

    static void switch_turn(GameSession *game) {
        if (game->get_game_state()->move_turn() ==
            game->get_game_state()->first_user()) {
            game->get_model_game()->get_player(0)->decrease_mana(-1);
            game->get_game_state()->set_move_turn(
                game->get_game_state()->second_user()
            );
        } else {
            game->get_model_game()->get_player(1)->decrease_mana(-1);
            game->get_game_state()->set_move_turn(
                game->get_game_state()->first_user()
            );
        }
        update_mana(game);
    }

    static void update_unit(
        namespace_proto::Unit *unit,
        const game_model::coordinates &coordinates,
        GameSession *game_session_res
    ) {
        game_model::game *model_game = game_session_res->get_model_game();
        game_model::cell cell = model_game->get_cell(coordinates);
        if (cell.get_unit_index() != -1 && cell.get_player_index() != -1) {
            game_model::unit model_unit =
                model_game->get_player(cell.get_player_index())
                    ->get_unit(cell.get_unit_index());
            unit->set_sum_of_health(model_unit.get_health());
            unit->set_amount_unit(model_unit.get_number());
        }
    }

    static void update_cell(
        namespace_proto::Cell *cell,
        const game_model::coordinates &coordinates,
        GameSession *game_session_res
    ) {
        game_model::game *model_game = game_session_res->get_model_game();
        game_model::cell model_cell = model_game->get_cell(coordinates);
        if (model_cell.get_unit_index() != -1) {
            cell->set_is_unit(true);
        } else {
            cell->set_is_unit(false);
        }
        cell->set_durability(model_cell.get_durability());
    }

    ::grpc::Status MoveUnit(
        ::grpc::ServerContext *context,
        const ::namespace_proto::MoveFromTo *request,
        ::namespace_proto::GameState *response
    ) override {
        GameSession *game_session_ref =
            &(get_server_state()->game_sessions[request->user().game_id()]);
        namespace_proto::GameState *game_state_ref =
            game_session_ref->get_game_state();
        game_model::coordinates from(request->start());
        namespace_proto::Cell *cell_from = game_state_ref->mutable_game_cells(
            request->start().row() * 10 + request->start().column()
        );
        game_model::coordinates to(request->finish());
        namespace_proto::Cell *cell_to = game_state_ref->mutable_game_cells(
            request->finish().row() * 10 + request->finish().column()
        );
        namespace_proto::OpponentMove *move =
            game_state_ref->mutable_opponent_move();
        move->set_opponent_id(request->user().user().id());

        if (game_session_ref->get_model_game()->get_cell(to).get_unit_index() ==
            -1) {
            (*game_session_ref->get_mover())(from, to);
            swapUnits(cell_from, cell_to);
            set_move(move, namespace_proto::move, cell_from, cell_to);
        } else {
            (*game_session_ref->get_attacker())(from, to);
            if (game_session_ref->get_model_game()->get_cell(to).get_unit_index(
                ) == -1) {
                cell_to->set_allocated_unit(nullptr);
                cell_to->set_is_unit(false);
                set_move(move, namespace_proto::kill, cell_from, cell_to);
            } else {
                update_unit(cell_to->mutable_unit(), to, game_session_ref);
                set_move(move, namespace_proto::attack, cell_from, cell_to);
            }
            if (game_session_ref->get_model_game()
                    ->get_cell(from)
                    .get_unit_index() == -1) {
                cell_from->set_allocated_unit(nullptr);
                cell_from->set_is_unit(false);
            } else {
                update_unit(cell_from->mutable_unit(), from, game_session_ref);
            }
        }
        update_cell(cell_from, from, game_session_ref);
        update_cell(cell_to, to, game_session_ref);

        switch_turn(game_session_ref);

        *response = *game_state_ref;
        handle_diff(game_session_ref, request->user().user());

        return ::grpc::Status::OK;
    }

    ::grpc::Status SelectUnit(
        ::grpc::ServerContext *context,
        const ::namespace_proto::MoveSelectUnit *request,
        ::namespace_proto::EnableCell *response
    ) override {
        GameSession *game_session_ref =
            &(get_server_state()->game_sessions[request->user().game_id()]);
        game_model::coordinates selected(request->unit());
        auto enable_cells = (*game_session_ref->get_move_selecter()
        )(selected, request->user().user().id());
        for (auto cell : enable_cells) {
            namespace_proto::Cell *new_cell = response->add_cells();
            new_cell->set_row(cell.get().get_coordinates().get_row());
            new_cell->set_column(cell.get().get_coordinates().get_column());
        }
        enable_cells = (*game_session_ref->get_attack_selecter()
        )(selected, request->user().user().id());
        for (auto cell : enable_cells) {
            namespace_proto::Cell *new_cell = response->add_cells();
            new_cell->set_row(cell.get().get_coordinates().get_row());
            new_cell->set_column(cell.get().get_coordinates().get_column());
            new_cell->set_is_attack(true);
        }
        return ::grpc::Status::OK;
    }

    ::grpc::Status SelectSpell(
        ::grpc::ServerContext *context,
        const ::namespace_proto::SelectSpellRequest *request,
        ::namespace_proto::EnableCell *response
    ) override {
        GameSession *game_session_ref =
            &(get_server_state()->game_sessions[request->user().game_id()]);
        auto enable_cells = (*game_session_ref->get_spell_selecter()
        )(request->user().user().id(), request->spell_id());
        for (auto cell : enable_cells) {
            namespace_proto::Cell *new_cell = response->add_cells();
            new_cell->set_row(cell.get().get_coordinates().get_row());
            new_cell->set_column(cell.get().get_coordinates().get_column());
            new_cell->set_is_attack(true);
        }
        return ::grpc::Status::OK;
    }

    ::grpc::Status DoSpell(
        ::grpc::ServerContext *context,
        const ::namespace_proto::DoSpellRequest *request,
        ::namespace_proto::GameState *response
    ) override {
        GameSession *game_session_ref =
            &(get_server_state()->game_sessions[request->user().game_id()]);
        (*game_session_ref->get_speller()
        )(game_model::coordinates{request->cell()}, request->user().user().id(),
          request->spell_id());
        int index = 10 * request->cell().row() + request->cell().column();
        namespace_proto::Cell *cell =
            game_session_ref->get_game_state()->mutable_game_cells(index);
        namespace_proto::Unit *unit = cell->mutable_unit();
        namespace_proto::OpponentMove *move =
            game_session_ref->get_game_state()->mutable_opponent_move();
        move->set_opponent_id(request->user().user().id());
        set_move(move, namespace_proto::spell, cell);
        update_cell(cell, game_model::coordinates{*cell}, game_session_ref);
        update_unit(unit, game_model::coordinates{*cell}, game_session_ref);
        update_mana(game_session_ref);
        *response = *handle_diff(game_session_ref, request->user().user());
        return ::grpc::Status::OK;
    }
};

void RunServer(const std::string &address, ServerServices *services);

#endif  // SERVER_HPP_
