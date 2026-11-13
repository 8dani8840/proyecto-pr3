// ============================================================
// test_pila_cola.cpp — Tests de PilaEventos y ColaAlertas
// Verifica comportamiento LIFO y FIFO respectivamente.
// ============================================================

#include <iostream>
#include <cassert>
#include <stdexcept>
#include "structures/PilaEventos.hpp"
#include "structures/ColaAlertas.hpp"

static int passed = 0;
static int failed = 0;

#define ASSERT(cond, msg) \
    if (cond) { std::cout << "  [OK] " << msg << "\n"; ++passed; } \
    else { std::cout << "  [FAIL] " << msg << "\n"; ++failed; }

// ---- TESTS PILA ----

void testPilaLIFO() {
    std::cout << "\n-- Test: Pila LIFO --\n";
    PilaEventos pila;

    ASSERT(pila.estaVacia(), "Pila recién creada está vacía");
    ASSERT(pila.tamano() == 0, "Tamaño inicial == 0");

    pila.push(Evento(TipoEvento::ACCION, "Accion 1"));
    pila.push(Evento(TipoEvento::ACCION, "Accion 2"));
    pila.push(Evento(TipoEvento::ALERTA, "Alerta 3"));

    ASSERT(pila.tamano() == 3, "Tamaño tras 3 push == 3");
    ASSERT(!pila.estaVacia(), "Pila no está vacía");
    ASSERT(pila.peek().descripcion == "Alerta 3", "peek() retorna el ultimo elemento");

    Evento tope = pila.pop();
    ASSERT(tope.descripcion == "Alerta 3", "pop() retorna Alerta 3 (LIFO)");
    ASSERT(pila.tamano() == 2, "Tamaño tras pop == 2");

    Evento segundo = pila.pop();
    ASSERT(segundo.descripcion == "Accion 2", "Segundo pop retorna Accion 2");
}

void testPilaObtenerUltimos() {
    std::cout << "\n-- Test: Pila — obtenerUltimos() --\n";
    PilaEventos pila;

    for (int i = 1; i <= 10; ++i) {
        pila.push(Evento(TipoEvento::ACCION, "Evento " + std::to_string(i)));
    }

    auto ultimos3 = pila.obtenerUltimos(3);
    ASSERT(ultimos3.size() == 3, "obtenerUltimos(3) retorna 3 elementos");
    ASSERT(ultimos3[0].descripcion == "Evento 10", "Primer elemento == último insertado");
    ASSERT(ultimos3[2].descripcion == "Evento 8",  "Tercer elemento es el tercero desde el tope");
}

void testPilaExcepcionVacia() {
    std::cout << "\n-- Test: Pila — Excepcion en vacio --\n";
    PilaEventos pila;

    bool excepcionLanzada = false;
    try {
        pila.pop();
    } catch (const std::underflow_error&) {
        excepcionLanzada = true;
    }
    ASSERT(excepcionLanzada, "pop() en pila vacía lanza underflow_error");

    excepcionLanzada = false;
    try {
        pila.peek();
    } catch (const std::underflow_error&) {
        excepcionLanzada = true;
    }
    ASSERT(excepcionLanzada, "peek() en pila vacía lanza underflow_error");
}

// ---- TESTS COLA ----

void testColaFIFO() {
    std::cout << "\n-- Test: Cola FIFO --\n";
    ColaAlertas cola;

    ASSERT(cola.estaVacia(), "Cola recién creada está vacía");
    ASSERT(cola.tamano() == 0, "Tamaño inicial == 0");

    cola.enqueue(Evento(TipoEvento::ALERTA, "Alerta A"));
    cola.enqueue(Evento(TipoEvento::ALERTA, "Alerta B"));
    cola.enqueue(Evento(TipoEvento::DESCUBRIMIENTO, "Descubrimiento C"));

    ASSERT(cola.tamano() == 3, "Tamaño tras 3 enqueue == 3");
    ASSERT(cola.frente().descripcion == "Alerta A", "frente() retorna el primer elemento");

    Evento primero = cola.dequeue();
    ASSERT(primero.descripcion == "Alerta A", "dequeue() retorna Alerta A (FIFO)");
    ASSERT(cola.tamano() == 2, "Tamaño tras dequeue == 2");

    Evento segundo = cola.dequeue();
    ASSERT(segundo.descripcion == "Alerta B", "Segundo dequeue retorna Alerta B");
}

void testColaObtenerPendientes() {
    std::cout << "\n-- Test: Cola — obtenerPendientes() (no consume) --\n";
    ColaAlertas cola;

    cola.enqueue(Evento(TipoEvento::ALERTA, "A1"));
    cola.enqueue(Evento(TipoEvento::ALERTA, "A2"));
    cola.enqueue(Evento(TipoEvento::ALERTA, "A3"));

    auto pendientes = cola.obtenerPendientes();
    ASSERT(pendientes.size() == 3, "obtenerPendientes() retorna 3 sin consumir");
    ASSERT(cola.tamano() == 3, "Cola sigue teniendo 3 elementos (no consumidos)");
    ASSERT(pendientes[0].descripcion == "A1", "Primer pendiente es A1 (FIFO)");
}

void testColaExcepcionVacia() {
    std::cout << "\n-- Test: Cola — Excepcion en vacio --\n";
    ColaAlertas cola;

    bool excepcionLanzada = false;
    try {
        cola.dequeue();
    } catch (const std::underflow_error&) {
        excepcionLanzada = true;
    }
    ASSERT(excepcionLanzada, "dequeue() en cola vacía lanza underflow_error");
}

int main() {
    std::cout << "╔══════════════════════════════════════╗\n";
    std::cout << "║  TEST — PilaEventos + ColaAlertas   ║\n";
    std::cout << "╚══════════════════════════════════════╝\n";

    // Pila
    testPilaLIFO();
    testPilaObtenerUltimos();
    testPilaExcepcionVacia();

    // Cola
    testColaFIFO();
    testColaObtenerPendientes();
    testColaExcepcionVacia();

    std::cout << "\n========================================\n";
    std::cout << "  Resultados: " << passed << " OK, " << failed << " FAIL\n";
    std::cout << "========================================\n";

    return (failed == 0) ? 0 : 1;
}
