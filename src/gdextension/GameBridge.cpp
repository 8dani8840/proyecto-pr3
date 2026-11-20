// GameBridge.cpp — Implementacion del bridge entre GameEngine y Godot GDExtension
#include "GameBridge.hpp"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void GameBridge::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_scene_title"), &GameBridge::get_scene_title);
    ClassDB::bind_method(D_METHOD("get_scene_description"), &GameBridge::get_scene_description);
    ClassDB::bind_method(D_METHOD("take_decision", "id"), &GameBridge::take_decision);
    ClassDB::bind_method(D_METHOD("collect_item", "key", "name", "desc"), &GameBridge::collect_item);
    ClassDB::bind_method(D_METHOD("get_inventory"), &GameBridge::get_inventory);
    ClassDB::bind_method(D_METHOD("get_clues"), &GameBridge::get_clues);
    ClassDB::bind_method(D_METHOD("is_game_over"), &GameBridge::is_game_over);
}

void GameBridge::_ready() {
    engine.iniciar();
}

godot::String GameBridge::get_scene_title() const {
    return godot::String(engine.getEscenaActual().getTitulo().c_str());
}

godot::String GameBridge::get_scene_description() const {
    return godot::String(engine.getEscenaActual().getDescripcion().c_str());
}

bool GameBridge::take_decision(int id) {
    return engine.tomarDecision(id);
}

bool GameBridge::collect_item(godot::String key, godot::String name, godot::String desc) {
    std::string k = key.utf8().get_data();
    std::string n = name.utf8().get_data();
    std::string d = desc.utf8().get_data();
    return engine.getInventario().agregar(k, {n, d});
}

godot::Array GameBridge::get_inventory() const {
    godot::Array result;
    for (const auto& item : engine.getInventario().getItems()) {
        godot::Dictionary d;
        d["key"]  = godot::String(item.first.c_str());
        d["name"] = godot::String(item.second.nombre.c_str());
        d["desc"] = godot::String(item.second.descripcion.c_str());
        result.push_back(d);
    }
    return result;
}

godot::Array GameBridge::get_clues() const {
    godot::Array result;
    for (const auto& pista : engine.getExpediente().getAll()) {
        godot::Dictionary d;
        d["id"]          = pista.id;
        d["descripcion"] = godot::String(pista.descripcion.c_str());
        result.push_back(d);
    }
    return result;
}

bool GameBridge::is_game_over() const {
    return engine.juegoTerminado();
}
