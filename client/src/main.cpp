#include <future>
#include "client.hpp"
#include "game.hpp"
#include "menu.hpp"

int main() {
    const std::shared_ptr<::grpc::ChannelInterface> &channel =
        grpc::CreateChannel(
            "localhost:50051", grpc::InsecureChannelCredentials()
        );
    get_client_state()->m_stub =
        std::make_unique<namespace_proto::Server::Stub>(channel);
    get_client_state()->m_user.mutable_user()->set_id(-1);

    while (!menu_interface::get_menu_state()->get_window()->is_done()) {
        menu_interface::get_menu_state()->update();
        menu_interface::get_menu_state()->render();
    }

    Client::get_hero();
    get_client_state()->m_opponent.set_type(-1);
    std::thread receiver(&Client::run_receiver);
    while (!game_interface::get_game_state()->get_window()->is_done()) {
        {
            std::unique_lock lock{get_client_state()->m_mutex};
            game_interface::get_game_state()->update();
        }
        game_interface::get_game_state()->render();
    }
    receiver.join();
    return 0;
}