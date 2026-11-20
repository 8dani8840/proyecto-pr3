#include "structures/ColaAlertas.hpp"
#include <iostream>

// ============================================================
// ColaAlertas — Implementación con DeSiGNAR::DynQueue
// ============================================================

void ColaAlertas::enqueue(const Evento& evento) {
    if (static_cast<int>(cola.size()) >= CAPACIDAD_MAX) {
        // Descartar la más antigua (frente)
        cola.get();
    }
    cola.put(evento);
}

Evento ColaAlertas::dequeue() {
    if (estaVacia()) {
        throw std::underflow_error("ColaAlertas: intento de dequeue() en cola vacia.");
    }
    return cola.get();
}

const Evento& ColaAlertas::frente() const {
    if (estaVacia()) {
        throw std::underflow_error("ColaAlertas: intento de frente() en cola vacia.");
    }
    return cola.front();
}

std::vector<Evento> ColaAlertas::obtenerPendientes() const {
    Designar::DynQueue<Evento> copia = cola;
    std::vector<Evento> resultado;
    resultado.reserve(copia.size());
    while (!copia.is_empty()) {
        resultado.push_back(copia.get());
    }
    return resultado;
}

void ColaAlertas::mostrarAlertas() const {
    std::cout << "\n=== ALERTAS PENDIENTES (Cola FIFO — DeSiGNAR) ===\n";
    if (estaVacia()) {
        std::cout << "  (Sin alertas pendientes)\n";
        return;
    }
    std::vector<Evento> pendientes = obtenerPendientes();
    for (const auto& e : pendientes) {
        std::cout << "  [" << tipoEventoAString(e.tipo) << "] "
                  << e.descripcion << "\n";
    }
}
