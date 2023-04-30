#ifndef BATTLE_OF_HEROES_COORDINATES_HPP
#define BATTLE_OF_HEROES_COORDINATES_HPP

namespace game_view {
    class Coords {
    public:
        Coords() : m_row(0), m_column(0){};
        Coords(int row, int column) : m_row(row), m_column(column){};

        [[nodiscard]] int get_row() const;

        [[nodiscard]] int get_column() const;

    private:
        int m_row;
        int m_column;
    };
}

#endif  // BATTLE_OF_HEROES_COORDINATES_HPP