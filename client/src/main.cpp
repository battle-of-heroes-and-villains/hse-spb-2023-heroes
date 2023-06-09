#include <future>
#include "client.hpp"
#include "state_machine.hpp"

int main() {
    get_client_state()->channel = grpc::CreateChannel(
        "localhost:50051", grpc::InsecureChannelCredentials()
    );
    get_client_state()->m_stub =
        std::make_unique<namespace_proto::Server::Stub>(
            get_client_state()->channel
        );
    auto *game_state = new GameState;
    auto *menu_state = new MenuState;
    menu_state->set_game_state(game_state);
    game_state->set_menu_state(menu_state);
    Context window_context{menu_state};

    while (true) {  // TODO: while (not interrupted)
        window_context.display();
        std::cout << "switch\n";
        if (window_context.is_exit()) {
            break;
        }
        window_context.switch_state();
    }
    return 0;
}
