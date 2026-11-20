#pragma once
// GameBridge.hpp — conecta GameEngine con nodos Godot via GDExtension
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/array.hpp>
#include "game/GameEngine.hpp"

class GameBridge : public godot::Node {
    GDCLASS(GameBridge, godot::Node)

private:
    GameEngine engine;

public:
    static void _bind_methods();
    void _ready() override;

    // Narrativa
    godot::String get_scene_title() const;
    godot::String get_scene_description() const;

    // Decisiones y progresión
    bool take_decision(int id);

    // Inventario (HashMap via GDExtension)
    bool collect_item(godot::String key, godot::String name, godot::String desc);
    godot::Array get_inventory() const;

    // Pistas (AVL via GDExtension)
    godot::Array get_clues() const;

    // Estado del juego
    bool is_game_over() const;
};
