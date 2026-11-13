#ifndef PILA_EVENTOS_HPP
#define PILA_EVENTOS_HPP

#include "game/Evento.hpp"
#include <vector>
#include <stdexcept>

// ============================================================
// PilaEventos: Pila LIFO (Last-In, First-Out).
// Registra el historial de acciones de la investigadora.
// El último evento registrado es el primero en consultarse.
// Complejidad: O(1) para push, pop y peek.
// ============================================================
class PilaEventos {
private:
    static const int CAPACIDAD_MAX = 100; // Límite de historial visible
    std::vector<Evento> pila;

public:
    PilaEventos() = default;
    ~PilaEventos() = default;

    // Agrega un evento al tope de la pila
    void push(const Evento& evento);

    // Elimina y retorna el evento del tope
    Evento pop();

    // Consulta el evento del tope sin eliminarlo
    const Evento& peek() const;

    bool estaVacia() const { return pila.empty(); }
    int tamano() const { return static_cast<int>(pila.size()); }

    // Retorna los últimos N eventos (para mostrar historial en la UI)
    std::vector<Evento> obtenerUltimos(int n) const;

    // Retorna todos los eventos en orden LIFO
    std::vector<Evento> obtenerTodos() const;

    void limpiar() { pila.clear(); }
    void mostrarHistorial() const;
};

#endif // PILA_EVENTOS_HPP
