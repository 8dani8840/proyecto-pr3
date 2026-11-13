#include "structures/ColaAlertas.hpp"
#include <iostream>

// ============================================================
// ColaAlertas — Implementación
// Cola FIFO respaldada por std::deque.
// ============================================================

void ColaAlertas::enqueue(const Evento& evento) {
    if (static_cast<int>(cola.size()) >= CAPACIDAD_MAX) {
        // Si la cola está llena, descartar la alerta más antigua
        cola.pop_front();
    }
    cola.push_back(evento);
}

Evento ColaAlertas::dequeue() {
    if (estaVacia()) {
        throw std::underflow_error("ColaAlertas: intento de dequeue() en cola vacia.");
    }
    Evento frente = cola.front();
    cola.pop_front();
    return frente;
}

const Evento& ColaAlertas::frente() const {
    if (estaVacia()) {
        throw std::underflow_error("ColaAlertas: intento de frente() en cola vacia.");
    }
    return cola.front();
}

std::vector<Evento> ColaAlertas::obtenerPendientes() const {
    return std::vector<Evento>(cola.begin(), cola.end());
}

void ColaAlertas::mostrarAlertas() const {
    std::cout << "\n=== ALERTAS PENDIENTES (Cola FIFO) ===\n";
    if (estaVacia()) {
        std::cout << "  (Sin alertas pendientes)\n";
        return;
    }
    for (const auto& e : cola) {
        std::cout << "  [" << tipoEventoAString(e.tipo) << "] "
                  << e.descripcion << "\n";
    }
}
