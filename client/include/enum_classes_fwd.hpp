#ifndef BATTLE_OF_HEROES_ENUM_CLASSES_FWD_HPP
#define BATTLE_OF_HEROES_ENUM_CLASSES_FWD_HPP

#include <string>
#include "config.hpp"

namespace game_interface {
enum class UnitType { Empty, Type1, Type2, Type3, Type4, Type5 };

enum class AnimationType { Attack };

enum class CellType { Type1,
    Broken,
    Move,
    Enemy,
    Attack,
    Spell,
    AttackSpell
};

enum class TextureType { MenuBackground };

enum class EventType { FirstPress, SecondPress, Move, Targeting, Nothing };

enum class ButtonType { None, Play, Pass, Menu, Exit, Skip, GiveUp, Spell };

enum class CursorType { Attack };
}  // namespace game_interface

namespace menu_interface {
enum class PageType { Game, Exit, Entry, SignUp, Registration, GameChoose };
}  // namespace menu_interface

namespace interface {
static std::string source_dir = RESOURCE_PATH;

enum class Fonts { TittleFont, CaptionFont };
}  // namespace interface

#endif  // BATTLE_OF_HEROES_ENUM_CLASSES_FWD_HPP
