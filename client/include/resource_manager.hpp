#ifndef BATTLE_OF_HEROES_MANAGER_HPP
#define BATTLE_OF_HEROES_MANAGER_HPP

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include "enum_classes_fwd.hpp"

namespace game_interface {
class ResourceManager {
public:
    const static sf::Texture &load_cell_texture(CellType texture);

    const static sf::Texture &load_cell_property_texture(CellType texture);

    const static sf::Texture &load_unit_texture(UnitType texture);

    const static sf::Texture &load_selected_unit_texture(UnitType texture);

    const static sf::Texture &load_animation_sprite_sheet(UnitType type);

    static int amount_of_frames_in_animation(UnitType type);

    const static sf::Texture &load_texture(TextureType texture);

    const static sf::Font &load_font(interface::Fonts font);

    const static sf::Image &load_cursor(CursorType cursor);

    static void load_resources();

private:
    static std::unordered_map<CellType, sf::Texture> m_cell_textures;
    static std::unordered_map<CellType, sf::Texture> m_cell_property_textures;
    static std::unordered_map<UnitType, sf::Texture> m_unit_textures;
    static std::unordered_map<UnitType, sf::Texture> m_selected_unit_textures;
    static std::unordered_map<UnitType, sf::Texture> m_animation_sprite_sheets;
    static std::unordered_map<UnitType, int> m_amount_of_sprites_in_animation;
    static std::unordered_map<TextureType, sf::Texture> m_textures;
    static std::unordered_map<interface::Fonts, sf::Font> m_fonts;
    static std::unordered_map<CursorType, sf::Image> m_cursors;
};
}  // namespace game_interface

#endif  // BATTLE_OF_HEROES_MANAGER_HPP
