#ifndef GAME_BRIDGE_HPP
#define GAME_BRIDGE_HPP

// GameBridge.hpp — conecta GameEngine con nodos Godot via GDExtension
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include "game/GameEngine.hpp"

class GameBridge : public godot::Node {
    GDCLASS(GameBridge, godot::Node)

private:
    GameEngine engine;

protected:
    static void _bind_methods();

public:
    GameBridge() = default;
    ~GameBridge() override = default;

    void _ready() override;

    // Narrativa y Escenas
    godot::String get_scene_title() const;
    godot::String get_scene_description() const;
    godot::Array get_decisions() const;

    // Decisiones y progresión
    bool take_decision(int id);

    // Inventario (Tabla Hash via GDExtension)
    bool collect_item(godot::String key, godot::String name, godot::String desc);
    godot::Array get_inventory() const;

    // Pistas (AVL via GDExtension)
    godot::Array get_clues() const;

    // Historial y Alertas
    godot::Array get_recent_history(int n = 10) const;

    // Estado del juego
    bool is_game_over() const;
};

#endif // GAME_BRIDGE_HPP
