#ifndef CLIENT_HPP_
#define CLIENT_HPP_
#include <all_protos/demo.grpc.pb.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <memory>
#include <mutex>
#include <thread>

class ClientState final {
public:
    std::unique_ptr<namespace_proto::Server::Stub> m_stub = nullptr;
    namespace_proto::UserState m_user;
    namespace_proto::Hero m_hero;
    namespace_proto::Hero m_opponent;
    namespace_proto::GameState m_game_state;
    std::unique_ptr<grpc::ClientReader<namespace_proto::GameState>> reader;
    std::atomic<bool> active{true};
    std::condition_variable variable;
    mutable std::mutex m_mutex;
};

ClientState *get_client_state();

class Client final {
public:
    Client(const Client &) = delete;
    Client &operator=(const Client &) = delete;
    Client(Client &&) = delete;
    Client &operator=(Client &&) = delete;

public:
    static void run_receiver();
    static void move_unit(namespace_proto::Cell from, namespace_proto::Cell to);
    static std::vector<std::pair<int, int>> select_unit(
        namespace_proto::Cell selected
    );

    static bool is_need_log_in();

    static void log_in(std::string nickname, std::string password);
    static void sign_up(std::string nickname, std::string password);

    static void skip_turn();
    static void end_session();

    static int get_mana();

    static int get_opponent_mana();

    static void get_hero();
    static void get_opponent();

    static std::vector<std::pair<int, int>> select_spell(int spell_id);
    static void do_spell(int spell_id, namespace_proto::Cell cell);
};

#endif  // CLIENT_HPP_
