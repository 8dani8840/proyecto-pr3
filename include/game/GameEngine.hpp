#ifndef GAME_ENGINE_HPP
#define GAME_ENGINE_HPP

#include "structures/InventarioHash.hpp"
#include "structures/ArbolAVL.hpp"
#include "structures/PilaEventos.hpp"
#include "structures/ColaAlertas.hpp"
#include "structures/GrafoNarrativo.hpp"
#include "game/Pista.hpp"
#include "game/Evento.hpp"
#include <string>
#include <vector>

// ============================================================
// GameEngine: Orquestador central del juego.
// Integra todas las estructuras de datos y coordina el estado
// de la partida: narrativa, inventario, pistas y eventos.
// ============================================================
class GameEngine {
private:
    InventarioHash inventario;
    ArbolAVL expediente;
    PilaEventos historial;
    ColaAlertas alertas;
    GrafoNarrativo narrativa;

    int proximoIdPista;
    std::string nombreJugadora;
    bool juegoTerminado;

    // Inicializa el mundo del juego con datos predefinidos
    void inicializarNarrativa();
    void inicializarInventarioInicial();

    // Helper: obtener claves del inventario para el grafo
    std::vector<std::string> obtenerClavesInventario() const;

    // Helper: registrar acción y agregar alerta
    void registrarAccion(TipoEvento tipo, const std::string& descripcion);

public:
    GameEngine();
    ~GameEngine() = default;

    // No permitir copia
    GameEngine(const GameEngine&) = delete;
    GameEngine& operator=(const GameEngine&) = delete;

    // ---- Gestión de Inventario ----
    bool recogerObjeto(const std::string& clave, const std::string& nombre, const std::string& desc);
    bool usarObjeto(const std::string& clave);
    ObjetoInventario* consultarObjeto(const std::string& clave);
    std::vector<ObjetoInventario> obtenerInventario() const;

    // ---- Gestión de Pistas (Árbol AVL) ----
    bool agregarPista(const std::string& titulo, const std::string& desc, CategoriaPista cat);
    Pista* buscarPista(int id);
    std::vector<Pista> obtenerExpediente() const;
    std::vector<Pista> filtrarPistasPorCategoria(const std::string& categoria) const;

    // ---- Narrativa y Decisiones ----
    NodoNarrativo* getEscenaActual();
    std::vector<AristaDecision> getDecisionesDisponibles() const;
    bool tomarDecision(int idDecision);

    // ---- Alertas y Historial ----
    bool hayAlertasPendientes() const;
    Evento siguienteAlerta();
    std::vector<Evento> obtenerHistorialReciente(int n = 10) const;

    // ---- Estado General ----
    bool estaTerminado() const { return juegoTerminado; }
    std::string getNombreJugadora() const { return nombreJugadora; }
    void setNombreJugadora(const std::string& nombre) { nombreJugadora = nombre; }
    int getTotalPistas() const;
    int getTotalObjetos() const;

    // ---- Debug / Test ----
    void mostrarEstadoCompleto() const;
};

#endif // GAME_ENGINE_HPP
