#ifndef MODEL_GAME_HPP
#define MODEL_GAME_HPP

#include <functional>
#include <memory>
#include <utility>
#include <vector>
#include "board.hpp"
#include "cell.hpp"
#include "player.hpp"

namespace game_model {

class game {
private:
    std::vector<std::unique_ptr<player>> m_players_list;
    board m_board{};

    static void clear_cell(cell &current_cell);
    void attack_cell(const cell &attacking, cell &attacked);

    static std::vector<std::vector<int>> troops;

public:
    explicit game(
        int account_id_first,
        int first_troop,
        int account_id_second,
        int second_troop
    ) {
        m_players_list.emplace_back(std::make_unique<player>(account_id_first));
        m_players_list.emplace_back(std::make_unique<player>(account_id_second)
        );
        m_players_list[0]->set_start_units(0, m_board, troops[first_troop]);
        m_players_list[1]->set_start_units(1, m_board, troops[second_troop]);
    }

    static int get_troop();

    [[nodiscard]] cell &get_cell(const coordinates &cell_coordinates);
    [[nodiscard]] std::vector<std::reference_wrapper<cell>>
    get_reachable_cells(const coordinates &cell_coordinates, int user_id);
    [[nodiscard]] std::vector<std::reference_wrapper<cell>>
    get_attackable_cells(const coordinates &cell_coordinates, int user_id);
    [[nodiscard]] std::vector<std::reference_wrapper<cell>>
    get_spellable_cells(int user_id, int spell_id);

    [[nodiscard]] player *get_player(int index);

    void move(
        const coordinates &current_cell_coordinates,
        const coordinates &new_cell_coordinates
    );
    void attack(const coordinates &attacking, const coordinates &attacked);
    void spell(const coordinates &cell_coordinates, int user_id, int spell_id);
};

}  // namespace game_model

#endif  // MODEL_GAME_HPP
