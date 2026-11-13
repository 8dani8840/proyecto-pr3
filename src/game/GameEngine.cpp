#include "game/GameEngine.hpp"
#include <iostream>

// ============================================================
// GameEngine — Implementación
// Orquesta todas las estructuras de datos del juego.
// ============================================================

GameEngine::GameEngine()
    : proximoIdPista(1), nombreJugadora("Detective"), juegoTerminado(false) {
    inicializarNarrativa();
    inicializarInventarioInicial();
}

// ---- Inicialización del Mundo ----

void GameEngine::inicializarNarrativa() {
    // =========================================================
    // NARRATIVA: "Cold Case — La Mansión Blackwood"
    // La investigadora Sofía Ramos reabre un caso de hace 20
    // años: la desaparición de Eleanor Blackwood.
    // =========================================================

    // --- Nodos (Escenas) ---
    narrativa.agregarNodo({0, "Entrada de la Mansión",
        "Te paras frente a la imponente mansión Blackwood. Las rejas oxidadas "
        "crujen. Tu expediente dice que Eleanor desapareció aquí en 2004. "
        "Una nota cae a tus pies.",
        true, false, ""});

    narrativa.agregarNodo({1, "Vestíbulo Principal",
        "El polvo cubre cada superficie. Un retrato de Eleanor te observa desde "
        "lo alto de la escalinata. Ves tres puertas: la biblioteca, el estudio "
        "y el sótano.",
        false, false, ""});

    narrativa.agregarNodo({2, "Biblioteca",
        "Estantes repletos de libros oscuros. Un diario con páginas arrancadas "
        "descansa sobre la mesa. Entre las páginas, una llave oxidada con el "
        "sello 'B'.",
        false, false, ""});

    narrativa.agregarNodo({3, "Estudio del Sr. Blackwood",
        "Papeles financieros dispersos. Un cajón cerrado. Los registros muestran "
        "transferencias millonarias el día de la desaparición.",
        false, false, "llave_b"});

    narrativa.agregarNodo({4, "Sótano",
        "El frío es insoportable. Una grabadora antigua reposa en un estante. "
        "Al reproducirla, escuchas la voz de Eleanor: 'No pueden saberlo...'",
        false, false, ""});

    narrativa.agregarNodo({5, "Confrontación — La Verdad",
        "Con todas las evidencias sobre la mesa, confrontas al heredero Blackwood. "
        "El caso está resuelto. Eleanor no desapareció: fue encubierta por la familia.",
        false, true, "grabacion_eleanor"});

    narrativa.agregarNodo({6, "Final Alternativo — Expediente Sellado",
        "Sin la grabación, el caso queda sin resolver. El expediente vuelve a "
        "archivarse. La verdad permanece oculta otros 20 años.",
        false, true, ""});

    // --- Aristas (Decisiones) ---
    narrativa.agregarArista(0, {1, "Entrar a la mansión"});

    narrativa.agregarArista(1, {2, "Ir a la biblioteca"});
    narrativa.agregarArista(1, {4, "Bajar al sótano"});
    narrativa.agregarArista(1, {3, "Intentar abrir el estudio", "llave_b"});

    narrativa.agregarArista(2, {1, "Volver al vestíbulo"});
    narrativa.agregarArista(2, {3, "Usar la llave en el estudio", "llave_b"});

    narrativa.agregarArista(3, {5, "Presentar todas las evidencias", "grabacion_eleanor"});
    narrativa.agregarArista(3, {6, "Cerrar el caso sin prueba definitiva"});

    narrativa.agregarArista(4, {1, "Volver al vestíbulo"});
    narrativa.agregarArista(4, {5, "Ir al estudio con la grabación", "grabacion_eleanor"});

    registrarAccion(TipoEvento::SISTEMA, "Motor de juego inicializado.");
    registrarAccion(TipoEvento::ALERTA,
        "Sofía Ramos abre el expediente N° 2004-078. La mansión Blackwood "
        "guarda secretos que nadie quiso revelar.");
}

void GameEngine::inicializarInventarioInicial() {
    // El jugador comienza con su expediente y una linterna
    inventario.insertar("linterna", "Linterna Táctica",
                        "Ilumina zonas oscuras. Indispensable en el sótano.");
    inventario.insertar("expediente_2004", "Expediente #2004-078",
                        "El caso original. Contiene fotos y declaraciones contradictorias.");
}

// ---- Helpers ----

void GameEngine::registrarAccion(TipoEvento tipo, const std::string& descripcion) {
    Evento e(tipo, descripcion);
    historial.push(e);

    // Las alertas y descubrimientos también van a la cola
    if (tipo == TipoEvento::ALERTA || tipo == TipoEvento::DESCUBRIMIENTO) {
        alertas.enqueue(e);
    }
}

std::vector<std::string> GameEngine::obtenerClavesInventario() const {
    std::vector<std::string> claves;
    // Necesitamos const_cast porque obtenerTodos() es en un objeto const
    for (const auto& obj : const_cast<InventarioHash&>(inventario).obtenerTodos()) {
        claves.push_back(obj.clave);
    }
    return claves;
}

// ---- Gestión de Inventario ----

bool GameEngine::recogerObjeto(const std::string& clave, const std::string& nombre,
                                const std::string& desc) {
    if (inventario.buscar(clave) != nullptr) {
        return false; // Ya en el inventario
    }
    inventario.insertar(clave, nombre, desc);
    registrarAccion(TipoEvento::ACCION, "Recogido: " + nombre);
    registrarAccion(TipoEvento::ALERTA, "Nuevo objeto en tu mochila: " + nombre + ". " + desc);
    return true;
}

bool GameEngine::usarObjeto(const std::string& clave) {
    ObjetoInventario* obj = inventario.buscar(clave);
    if (obj == nullptr) return false;
    inventario.marcarUsado(clave);
    registrarAccion(TipoEvento::ACCION, "Usado: " + obj->nombre);
    return true;
}

ObjetoInventario* GameEngine::consultarObjeto(const std::string& clave) {
    return inventario.buscar(clave);
}

std::vector<ObjetoInventario> GameEngine::obtenerInventario() const {
    return const_cast<InventarioHash&>(inventario).obtenerTodos();
}

// ---- Gestión de Pistas (Árbol AVL) ----

bool GameEngine::agregarPista(const std::string& titulo, const std::string& desc,
                               CategoriaPista cat) {
    Pista nueva(proximoIdPista++, titulo, desc, cat);
    expediente.insertar(nueva);
    registrarAccion(TipoEvento::DESCUBRIMIENTO,
                    "Nueva pista añadida al expediente: [" + titulo + "]");
    return true;
}

Pista* GameEngine::buscarPista(int id) {
    return expediente.buscar(id);
}

std::vector<Pista> GameEngine::obtenerExpediente() const {
    return expediente.obtenerOrdenado();
}

std::vector<Pista> GameEngine::filtrarPistasPorCategoria(const std::string& categoria) const {
    return expediente.filtrarPorCategoria(categoria);
}

// ---- Narrativa y Decisiones ----

NodoNarrativo* GameEngine::getEscenaActual() {
    return narrativa.getNodo(narrativa.getNodoActual());
}

std::vector<AristaDecision> GameEngine::getDecisionesDisponibles() const {
    return narrativa.getDecisiones(narrativa.getNodoActual(), obtenerClavesInventario());
}

bool GameEngine::tomarDecision(int idDecision) {
    bool ok = narrativa.moverA(idDecision, obtenerClavesInventario());
    if (ok) {
        NodoNarrativo* nodo = narrativa.getNodo(idDecision);
        if (nodo) {
            registrarAccion(TipoEvento::DECISION, "Avanzando a: " + nodo->titulo);
            if (nodo->esFinale) {
                juegoTerminado = true;
                registrarAccion(TipoEvento::ALERTA, "FIN: " + nodo->titulo);
            }
        }
    }
    return ok;
}

// ---- Alertas y Historial ----

bool GameEngine::hayAlertasPendientes() const {
    return !alertas.estaVacia();
}

Evento GameEngine::siguienteAlerta() {
    return alertas.dequeue();
}

std::vector<Evento> GameEngine::obtenerHistorialReciente(int n) const {
    return historial.obtenerUltimos(n);
}

// ---- Estado General ----

int GameEngine::getTotalPistas() const {
    return expediente.getTotalPistas();
}

int GameEngine::getTotalObjetos() const {
    return const_cast<InventarioHash&>(inventario).contarObjetos();
}

void GameEngine::mostrarEstadoCompleto() const {
    std::cout << "\n========================================\n";
    std::cout << "  COLD CASE — Estado de la Partida\n";
    std::cout << "  Investigadora: " << nombreJugadora << "\n";
    std::cout << "========================================\n";

    const_cast<InventarioHash&>(inventario).mostrarInventario();
    expediente.mostrarExpediente();
    historial.mostrarHistorial();
    alertas.mostrarAlertas();
    narrativa.mostrarGrafo();
}
