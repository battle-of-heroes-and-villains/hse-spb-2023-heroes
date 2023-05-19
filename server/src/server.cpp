#include "server.hpp"

ServerState *get_server_state() {
    static ServerState server_state;
    return &server_state;
}

int my_rand() {
    int min = 0;
    auto max = static_cast<int>(const_game_info::HEROES_LIST.size());
    if (min == max)
        return min;
    return min + rand() % (max - min);
}

void RunServer(const std::string &address, ServerServices *service) {
    grpc::ServerBuilder builder;
    builder.AddListeningPort(address, grpc::InsecureServerCredentials());
    builder.RegisterService(service);
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    grpc::ServerContext context;
    std::thread pairing_worker{[&]() {
        while (true) {
            while (get_server_state()->pairing_wait_list.size() >= 2) {
                Player first_player =
                    get_server_state()->pairing_wait_list.pop();
                Player second_player =
                    get_server_state()->pairing_wait_list.pop();
                int game_id = 0;
                {
                    std::unique_lock lock{get_server_state()->server_mutex};
                    game_id = static_cast<int>(
                        get_server_state()->game_sessions.size()
                    );
                    get_server_state()->game_sessions.emplace_back(
                        first_player, second_player
                    );
                }
                std::thread(&start_game_session, game_id).detach();
            }
        }
    }};
    std::thread bot_worker{[&]() {
        while (true) {
            while (get_server_state()->wait_list.size() >= 1) {
                Player first_player = get_server_state()->wait_list.pop();
                Player second_player{my_rand()};
                int game_id = 0;
                {
                    std::unique_lock lock{get_server_state()->server_mutex};
                    game_id = static_cast<int>(
                        get_server_state()->game_sessions.size()
                    );
                    get_server_state()->game_sessions.emplace_back(
                        first_player, second_player
                    );
                    get_server_state()->game_sessions.back();
                }
                std::thread(&start_game_session, game_id).detach();
            }
        }
    }};
    pairing_worker.join();
    bot_worker.join();
}
