#ifndef BATTLE_OF_HEROES_MANAGER_HPP
#define BATTLE_OF_HEROES_MANAGER_HPP

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include "enum_classes_fwd.hpp"

namespace game_interface {
class ResourceManager {
public:
    [[nodiscard]] const static sf::Texture &load_cell_texture(CellType texture);

    [[nodiscard]] const static sf::Texture &load_cell_property_texture(
        CellType texture
    );

    [[nodiscard]] const static sf::Texture &load_unit_texture(UnitType texture);

    [[nodiscard]] const static sf::Texture &load_selected_unit_texture(
        UnitType texture
    );

    [[nodiscard]] const static sf::Texture &load_attack_animation_sprite_sheet(
        UnitType texture
    );

    [[nodiscard]] const static sf::Texture &load_hurt_animation_sprite_sheet(
        UnitType texture
    );

    [[nodiscard]] const static sf::Texture &load_move_animation_sprite_sheet(
        UnitType texture
    );

    [[nodiscard]] const static sf::Texture &load_dead_animation_sprite_sheet(
        UnitType texture
    );

    [[nodiscard]] static int amount_of_frames_in_attack_animation(UnitType type
    );

    [[nodiscard]] static int amount_of_frames_in_hurt_animation(UnitType type);

    [[nodiscard]] static int amount_of_frames_in_move_animation(UnitType type);

    [[nodiscard]] static int amount_of_frames_in_dead_animation(UnitType type);

    [[nodiscard]] const static sf::Texture &load_texture(TextureType texture);

    [[nodiscard]] const static sf::Font &load_font(interface::Fonts font);

    [[nodiscard]] const static sf::SoundBuffer &load_sound(interface::Sounds sound);

    [[nodiscard]] const static sf::Image &load_cursor(CursorType cursor);

    static void load_resources();

private:
    static std::unordered_map<interface::Sounds, sf::SoundBuffer> m_sounds;

    static std::unordered_map<TextureType, sf::Texture> m_textures;
    static std::unordered_map<interface::Fonts, sf::Font> m_fonts;
    static std::unordered_map<CursorType, sf::Image> m_cursors;

    static std::unordered_map<CellType, sf::Texture> m_cell_textures;
    static std::unordered_map<CellType, sf::Texture> m_cell_property_textures;
    static std::unordered_map<UnitType, sf::Texture> m_unit_textures;

    static std::unordered_map<UnitType, sf::Texture> m_selected_unit_textures;

    static std::unordered_map<UnitType, sf::Texture> m_attack_animation;
    static std::unordered_map<UnitType, sf::Texture> m_hurt_animation;
    static std::unordered_map<UnitType, sf::Texture> m_move_animation;
    static std::unordered_map<UnitType, sf::Texture> m_dead_animation;

    static std::unordered_map<UnitType, int>
        m_amount_of_sprites_in_attack_animation;
    static std::unordered_map<UnitType, int>
        m_amount_of_sprites_in_hurt_animation;
    static std::unordered_map<UnitType, int>
        m_amount_of_sprites_in_move_animation;
    static std::unordered_map<UnitType, int>
        m_amount_of_sprites_in_dead_animation;
};
}  // namespace game_interface

#endif  // BATTLE_OF_HEROES_MANAGER_HPP
