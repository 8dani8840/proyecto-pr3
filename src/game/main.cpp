// ============================================================
// main.cpp — Punto de entrada del juego Cold Case
// Modo consola: ejecuta una sesión de demostración completa
// que prueba todas las estructuras de datos integradas.
// ============================================================

#include <iostream>
#include "game/GameEngine.hpp"

int main() {
    std::cout << "╔══════════════════════════════════════╗\n";
    std::cout << "║   COLD CASE — Enigma Byte Studio     ║\n";
    std::cout << "║   Thriller de Investigación 2D       ║\n";
    std::cout << "╚══════════════════════════════════════╝\n\n";

    GameEngine engine;
    engine.setNombreJugadora("Sofía Ramos");

    // ---- Mostrar escena inicial ----
    NodoNarrativo* escena = engine.getEscenaActual();
    if (escena) {
        std::cout << "[ " << escena->titulo << " ]\n";
        std::cout << escena->descripcion << "\n\n";
    }

    // ---- Consumir alertas iniciales ----
    std::cout << "--- Alertas ---\n";
    while (engine.hayAlertasPendientes()) {
        Evento alerta = engine.siguienteAlerta();
        std::cout << ">> " << alerta.descripcion << "\n";
    }

    // ---- Decisión 1: Entrar a la mansión ----
    std::cout << "\n--- Decisiones disponibles ---\n";
    auto decisiones = engine.getDecisionesDisponibles();
    for (size_t i = 0; i < decisiones.size(); ++i) {
        std::cout << "  [" << decisiones[i].idDestino << "] "
                  << decisiones[i].textoDecision << "\n";
    }

    engine.tomarDecision(1); // Entrar al vestíbulo

    // ---- Recoger la llave en la biblioteca ----
    engine.tomarDecision(2); // Ir a la biblioteca
    engine.recogerObjeto("llave_b", "Llave con sello 'B'",
                          "Encontrada entre las páginas del diario. Abre el estudio.");

    // ---- Agregar pista al AVL ----
    engine.agregarPista("Diario con páginas arrancadas",
                        "Las fechas borradas coinciden con la desaparición de Eleanor.",
                        CategoriaPista::DOCUMENTAL);
    engine.agregarPista("Llave con sello Blackwood",
                        "Solo el heredero debería tenerla. ¿Cómo llegó a la biblioteca?",
                        CategoriaPista::FISICA);

    // ---- Ir al sótano ----
    engine.tomarDecision(1); // Volver al vestíbulo
    engine.tomarDecision(4); // Bajar al sótano
    engine.recogerObjeto("grabacion_eleanor", "Grabación de Eleanor",
                          "Voz de Eleanor: 'No pueden saberlo... el acuerdo...'");

    engine.agregarPista("Grabación de voz de Eleanor",
                        "Menciona un acuerdo secreto. La voz está temblorosa, asustada.",
                        CategoriaPista::TESTIMONIAL);
    engine.agregarPista("Registros financieros del estudio",
                        "Transferencias el día de la desaparición. Suma: $2.4M.",
                        CategoriaPista::DOCUMENTAL);

    // ---- Final: Confrontación ----
    engine.tomarDecision(5); // Ir al estudio con la grabación (requiere "grabacion_eleanor")

    // ---- Mostrar estado completo ----
    engine.mostrarEstadoCompleto();

    std::cout << "\n========================================\n";
    std::cout << "  FIN DE LA DEMOSTRACIÓN\n";
    std::cout << "  Pistas recopiladas: " << engine.getTotalPistas() << "\n";
    std::cout << "  Objetos en inventario: " << engine.getTotalObjetos() << "\n";
    std::cout << "  Juego completado: " << (engine.estaTerminado() ? "SI" : "NO") << "\n";
    std::cout << "========================================\n";

    return 0;
}
