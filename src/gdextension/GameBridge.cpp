// GameBridge.cpp — Implementacion del bridge entre GameEngine y Godot GDExtension
#include "GameBridge.hpp"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void GameBridge::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_scene_title"), &GameBridge::get_scene_title);
    ClassDB::bind_method(D_METHOD("get_scene_description"), &GameBridge::get_scene_description);
    ClassDB::bind_method(D_METHOD("get_decisions"), &GameBridge::get_decisions);
    ClassDB::bind_method(D_METHOD("take_decision", "id"), &GameBridge::take_decision);
    ClassDB::bind_method(D_METHOD("collect_item", "key", "name", "desc"), &GameBridge::collect_item);
    ClassDB::bind_method(D_METHOD("get_inventory"), &GameBridge::get_inventory);
    ClassDB::bind_method(D_METHOD("get_clues"), &GameBridge::get_clues);
    ClassDB::bind_method(D_METHOD("get_recent_history", "n"), &GameBridge::get_recent_history, DEFVAL(10));
    ClassDB::bind_method(D_METHOD("is_game_over"), &GameBridge::is_game_over);
}

void GameBridge::_ready() {
    // GameEngine se auto-inicializa en su constructor por defecto
}

godot::String GameBridge::get_scene_title() const {
    const NodoNarrativo* escena = const_cast<GameEngine&>(engine).getEscenaActual();
    if (escena != nullptr) {
        return godot::String(escena->titulo.c_str());
    }
    return godot::String("");
}

godot::String GameBridge::get_scene_description() const {
    const NodoNarrativo* escena = const_cast<GameEngine&>(engine).getEscenaActual();
    if (escena != nullptr) {
        return godot::String(escena->descripcion.c_str());
    }
    return godot::String("");
}

godot::Array GameBridge::get_decisions() const {
    godot::Array result;
    auto decisiones = engine.getDecisionesDisponibles();
    for (size_t i = 0; i < decisiones.size(); ++i) {
        godot::Dictionary d;
        d["index"] = static_cast<int>(i);
        d["id_destino"] = decisiones[i].idDestino;
        d["texto"] = godot::String(decisiones[i].textoDecision.c_str());
        d["condicion"] = godot::String(decisiones[i].condicion.c_str());
        result.push_back(d);
    }
    return result;
}

bool GameBridge::take_decision(int id) {
    return engine.tomarDecision(id);
}

bool GameBridge::collect_item(godot::String key, godot::String name, godot::String desc) {
    std::string k = key.utf8().get_data();
    std::string n = name.utf8().get_data();
    std::string d = desc.utf8().get_data();
    return engine.recogerObjeto(k, n, d);
}

godot::Array GameBridge::get_inventory() const {
    godot::Array result;
    for (const auto& item : engine.obtenerInventario()) {
        godot::Dictionary d;
        d["key"]  = godot::String(item.clave.c_str());
        d["name"] = godot::String(item.nombre.c_str());
        d["desc"] = godot::String(item.descripcion.c_str());
        d["usado"] = item.usado;
        result.push_back(d);
    }
    return result;
}

godot::Array GameBridge::get_clues() const {
    godot::Array result;
    for (const auto& pista : engine.obtenerExpediente()) {
        godot::Dictionary d;
        d["id"]          = pista.id;
        d["titulo"]      = godot::String(pista.titulo.c_str());
        d["descripcion"] = godot::String(pista.descripcion.c_str());
        d["categoria"]   = godot::String(categoriaAString(pista.categoria).c_str());
        d["analizada"]   = pista.analizada;
        result.push_back(d);
    }
    return result;
}

godot::Array GameBridge::get_recent_history(int n) const {
    godot::Array result;
    for (const auto& ev : engine.obtenerHistorialReciente(n)) {
        godot::Dictionary d;
        d["tipo"] = godot::String(tipoEventoAString(ev.tipo).c_str());
        d["descripcion"] = godot::String(ev.descripcion.c_str());
        d["timestamp"] = godot::String(ev.timestamp.c_str());
        result.push_back(d);
    }
    return result;
}

bool GameBridge::is_game_over() const {
    return engine.estaTerminado();
}
