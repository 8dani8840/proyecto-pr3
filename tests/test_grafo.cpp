// ============================================================
// test_grafo.cpp — Tests del módulo GrafoNarrativo
// Verifica navegación, condiciones e inventario + backtracking.
// ============================================================

#include <iostream>
#include <cassert>
#include "structures/GrafoNarrativo.hpp"

static int passed = 0;
static int failed = 0;

#define ASSERT(cond, msg) \
    if (cond) { std::cout << "  [OK] " << msg << "\n"; ++passed; } \
    else { std::cout << "  [FAIL] " << msg << "\n"; ++failed; }

GrafoNarrativo construirGrafoPrueba() {
    GrafoNarrativo grafo;

    grafo.agregarNodo({0, "Inicio",    "Escena inicial.",              true,  false, ""});
    grafo.agregarNodo({1, "Sala A",    "Una sala con una puerta.",     false, false, ""});
    grafo.agregarNodo({2, "Sala B",    "Sala con un cofre cerrado.",   false, false, ""});
    grafo.agregarNodo({3, "Final Malo","Termina sin evidencia.",       false, true,  ""});
    grafo.agregarNodo({4, "Final Bien","Caso resuelto con llave.",     false, true,  "llave_maestra"});

    grafo.agregarArista(0, {1, "Ir a Sala A"});
    grafo.agregarArista(1, {2, "Explorar Sala B"});
    grafo.agregarArista(1, {3, "Cerrar el caso"});
    grafo.agregarArista(2, {4, "Usar la llave maestra", "llave_maestra"});

    return grafo;
}

void testNavegacionBasica() {
    std::cout << "\n-- Test: Navegacion basica --\n";
    GrafoNarrativo grafo = construirGrafoPrueba();

    ASSERT(grafo.getNodoActual() == 0, "Nodo inicial es 0");

    std::vector<std::string> inv = {};
    bool movio = grafo.moverA(1, inv);
    ASSERT(movio, "Mover a nodo 1 (sin condicion) retorna true");
    ASSERT(grafo.getNodoActual() == 1, "Nodo actual es ahora 1");
}

void testCondicionBloqueada() {
    std::cout << "\n-- Test: Condicion bloqueada sin item --\n";
    GrafoNarrativo grafo = construirGrafoPrueba();

    // El nodo 4 requiere "llave_maestra"
    grafo.moverA(1, {});
    grafo.moverA(2, {});

    std::vector<std::string> sinLlave = {};
    bool movio = grafo.moverA(4, sinLlave);
    ASSERT(!movio, "No puede moverse al final si no tiene la llave");
    ASSERT(grafo.getNodoActual() == 2, "Permanece en nodo 2");
}

void testCondicionDesbloqueada() {
    std::cout << "\n-- Test: Condicion desbloqueada con item --\n";
    GrafoNarrativo grafo = construirGrafoPrueba();

    grafo.moverA(1, {});
    grafo.moverA(2, {});

    std::vector<std::string> conLlave = {"llave_maestra"};
    bool movio = grafo.moverA(4, conLlave);
    ASSERT(movio, "Se puede mover al final con la llave");
    ASSERT(grafo.getNodoActual() == 4, "Nodo actual es 4 (final bueno)");
}

void testDecisionesDisponibles() {
    std::cout << "\n-- Test: Decisiones disponibles segun inventario --\n";
    GrafoNarrativo grafo = construirGrafoPrueba();
    grafo.moverA(1, {});
    grafo.moverA(2, {});

    // Sin llave: solo puede cerrar el caso (via Sala A) — no puede usar la llave
    auto sinLlave = grafo.getDecisiones(2, {});
    ASSERT(sinLlave.empty(), "Sin llave, no hay decisiones disponibles desde Sala B");

    auto conLlave = grafo.getDecisiones(2, {"llave_maestra"});
    ASSERT(conLlave.size() == 1, "Con llave, hay 1 decision disponible");
    ASSERT(conLlave[0].idDestino == 4, "La decision lleva al final bueno (nodo 4)");
}

void testBacktracking() {
    std::cout << "\n-- Test: Backtracking — encontrar camino al final --\n";
    GrafoNarrativo grafo = construirGrafoPrueba();

    // Sin llave: solo camino al Final Malo (nodo 3)
    std::vector<int> caminoSinLlave = grafo.encontrarCaminoAFinal({});
    ASSERT(!caminoSinLlave.empty(), "Se encuentra un camino (al final malo)");
    ASSERT(caminoSinLlave.back() == 3, "El camino sin llave termina en nodo 3 (Final Malo)");

    // Con llave: camino al Final Bueno (nodo 4) debe existir
    std::vector<int> caminoConLlave = grafo.encontrarCaminoAFinal({"llave_maestra"});
    ASSERT(!caminoConLlave.empty(), "Se encuentra un camino con la llave");

    // El primer final encontrado con DFS puede variar; verificar que el camino es válido
    int ultimoNodo = caminoConLlave.back();
    ASSERT(ultimoNodo == 3 || ultimoNodo == 4,
           "El camino con llave termina en un nodo final valido");
}

void testDesbloquearNodo() {
    std::cout << "\n-- Test: Desbloquear nodo manualmente --\n";
    GrafoNarrativo grafo = construirGrafoPrueba();

    NodoNarrativo* nodo2 = grafo.getNodo(2);
    ASSERT(nodo2 != nullptr && !nodo2->desbloqueado, "Nodo 2 inicialmente bloqueado");

    grafo.desbloquearNodo(2);
    ASSERT(grafo.getNodo(2)->desbloqueado, "Nodo 2 desbloqueado manualmente");
}

int main() {
    std::cout << "╔══════════════════════════════════════╗\n";
    std::cout << "║   TEST — GrafoNarrativo + DFS        ║\n";
    std::cout << "╚══════════════════════════════════════╝\n";

    testNavegacionBasica();
    testCondicionBloqueada();
    testCondicionDesbloqueada();
    testDecisionesDisponibles();
    testBacktracking();
    testDesbloquearNodo();

    std::cout << "\n========================================\n";
    std::cout << "  Resultados: " << passed << " OK, " << failed << " FAIL\n";
    std::cout << "========================================\n";

    return (failed == 0) ? 0 : 1;
}
