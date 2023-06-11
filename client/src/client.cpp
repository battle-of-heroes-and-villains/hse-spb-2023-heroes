#include "client.hpp"
#include <utility>
#include "game.hpp"

[[nodiscard]] ClientState *get_client_state() {
    static ClientState clientState;
    return &clientState;
}

namespace_proto::UserState *dump_user_state(
    namespace_proto::UserState *request_user
) {
    *request_user = (get_client_state()->m_user);
    request_user->set_hero_id(get_client_state()->m_hero.type());
    return request_user;
}

bool Client::is_need_log_in() {
    return get_client_state()->m_user.user().id() == -1;
}

void Client::log_in(std::string nickname, std::string password) {
    namespace_proto::LogInData request;
    request.set_name(nickname);
    request.set_password(password);
    grpc::ClientContext context{};
    get_client_state()->m_stub->LogIn(
        &context, request, get_client_state()->m_user.mutable_user()
    );
}

void Client::sign_up(std::string nickname, std::string password) {
    namespace_proto::LogInData request;
    request.set_name(nickname);
    request.set_password(password);
    grpc::ClientContext context{};
    get_client_state()->m_stub->SignUp(
        &context, request, get_client_state()->m_user.mutable_user()
    );
}

void Client::run_receiver() {
    namespace_proto::GameState response;
    grpc::ClientContext context{};
    get_client_state()->reader = (get_client_state()->m_stub->CallServer(
        &context, get_client_state()->m_user
    ));
    while (get_client_state()->reader->Read(&response)) {
        {
            std::unique_lock lock{get_client_state()->m_mutex};
            while (!get_client_state()->active_animation) {
            }
            get_client_state()->active_animation = false;
            get_client_state()->m_game_state = response;
            if (!(get_client_state()->m_game_state.is_active())) {
                get_client_state()->active_game = false;
                break;
            }
            game_interface::get_game_state()->get_board()->update_board(
                get_client_state()->m_game_state
            );
        }
        get_client_state()->m_user.set_game_id(
            get_client_state()->m_game_state.game_id()
        );
    }
}

void Client::move_unit(namespace_proto::Cell from, namespace_proto::Cell to) {
    grpc::ClientContext context;
    namespace_proto::MoveFromTo request;
    auto *request_from = new namespace_proto::Cell;
    auto *request_to = new namespace_proto::Cell;
    *request_from = from;
    *request_to = to;
    request.set_allocated_start(request_from);
    request.set_allocated_finish(request_to);
    auto *request_user = new namespace_proto::UserState;
    request.set_allocated_user(dump_user_state(request_user));
    namespace_proto::GameState response;
    get_client_state()->m_stub->MoveUnit(&context, request, &response);
    get_client_state()->m_game_state = response;
}

std::vector<std::pair<int, int>> Client::select_unit(
    namespace_proto::Cell selected
) {
    grpc::ClientContext context;
    namespace_proto::MoveSelectUnit request;
    auto *request_cell = new namespace_proto::Cell;
    *request_cell = selected;
    request.set_allocated_unit(request_cell);
    auto *request_user = new namespace_proto::UserState;
    request.set_allocated_user(dump_user_state(request_user));
    namespace_proto::EnableCell response;
    get_client_state()->m_stub->SelectUnit(&context, request, &response);
    std::vector<std::pair<int, int>> enable_set;
    for (const auto &cell : response.cells()) {
        enable_set.emplace_back(cell.row(), cell.column());
    }
    return enable_set;
}

std::vector<std::pair<int, int>> Client::select_spell(int spell_id) {
    grpc::ClientContext context;
    namespace_proto::SelectSpellRequest request;
    auto *request_user = new namespace_proto::UserState;
    request.set_allocated_user(dump_user_state(request_user));
    request.set_spell_id(spell_id);
    namespace_proto::EnableCell response;
    get_client_state()->m_stub->SelectSpell(&context, request, &response);
    std::vector<std::pair<int, int>> enable_set;
    for (const auto &cell : response.cells()) {
        enable_set.emplace_back(cell.row(), cell.column());
    }
    return enable_set;
}

void Client::do_spell(int spell_id, namespace_proto::Cell cell) {
    grpc::ClientContext context;
    namespace_proto::DoSpellRequest request;
    //    request.set_game_id(get_client_state()->m_user.game_id());
    //    request.set_player_id(get_client_state()->m_user.user().id());

    request.set_spell_id(spell_id);
    auto request_cell = new namespace_proto::Cell(std::move(cell));
    request.set_allocated_cell(request_cell);
    auto *request_user = new namespace_proto::UserState;
    request.set_allocated_user(dump_user_state(request_user));
    namespace_proto::GameState response;
    get_client_state()->m_stub->DoSpell(&context, request, &response);
    get_client_state()->m_game_state = response;
}

void Client::get_hero() {
    grpc::ClientContext context;
    namespace_proto::Hero response;
    get_client_state()->m_stub->GetHero(
        &context, google::protobuf::Empty{}, &response
    );
    get_client_state()->m_hero = response;

    get_client_state()->m_hero.set_name(get_client_state()->m_user.user().name()
    );
}

void Client::get_opponent() {
    grpc::ClientContext context;

    auto *request_user = new namespace_proto::UserState;
    dump_user_state(request_user);
    namespace_proto::Hero response;
    get_client_state()->m_stub->GetOpponent(&context, *request_user, &response);
    get_client_state()->m_opponent = response;
}

void Client::skip_turn() {
    grpc::ClientContext context;
    auto *request_user = new namespace_proto::UserState;
    dump_user_state(request_user);
    namespace_proto::GameState response;
    get_client_state()->m_stub->SwitchTurn(&context, *request_user, &response);
    get_client_state()->m_game_state = response;
}

void Client::end_session() {
    grpc::ClientContext context;
    auto *request_user = new namespace_proto::UserState;
    dump_user_state(request_user);
    google::protobuf::Empty response;
    get_client_state()->m_stub->EndSession(&context, *request_user, &response);
}

int Client::get_mana() {
    if (get_client_state()->m_game_state.first_user() ==
        get_client_state()->m_user.user().id()) {
        return get_client_state()->m_game_state.first_user_mana();
    } else {
        return get_client_state()->m_game_state.second_user_mana();
    }
}

int Client::get_opponent_mana() {
    if (get_client_state()->m_game_state.first_user() ==
        get_client_state()->m_user.user().id()) {
        return get_client_state()->m_game_state.second_user_mana();
    } else {
        return get_client_state()->m_game_state.first_user_mana();
    }
}