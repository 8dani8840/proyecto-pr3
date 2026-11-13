#include "structures/PilaEventos.hpp"
#include <iostream>
#include <algorithm>

// ============================================================
// PilaEventos — Implementación
// Pila LIFO respaldada por std::vector.
// ============================================================

void PilaEventos::push(const Evento& evento) {
    if (static_cast<int>(pila.size()) >= CAPACIDAD_MAX) {
        // Eliminar el evento más antiguo (el fondo de la pila)
        pila.erase(pila.begin());
    }
    pila.push_back(evento);
}

Evento PilaEventos::pop() {
    if (estaVacia()) {
        throw std::underflow_error("PilaEventos: intento de pop() en pila vacia.");
    }
    Evento tope = pila.back();
    pila.pop_back();
    return tope;
}

const Evento& PilaEventos::peek() const {
    if (estaVacia()) {
        throw std::underflow_error("PilaEventos: intento de peek() en pila vacia.");
    }
    return pila.back();
}

std::vector<Evento> PilaEventos::obtenerUltimos(int n) const {
    std::vector<Evento> resultado;
    int inicio = std::max(0, static_cast<int>(pila.size()) - n);
    for (int i = static_cast<int>(pila.size()) - 1; i >= inicio; --i) {
        resultado.push_back(pila[i]);
    }
    return resultado;
}

std::vector<Evento> PilaEventos::obtenerTodos() const {
    std::vector<Evento> resultado(pila.rbegin(), pila.rend());
    return resultado;
}

void PilaEventos::mostrarHistorial() const {
    std::cout << "\n=== HISTORIAL DE ACCIONES (Pila LIFO) ===\n";
    if (estaVacia()) {
        std::cout << "  (Sin acciones registradas)\n";
        return;
    }
    for (int i = static_cast<int>(pila.size()) - 1; i >= 0; --i) {
        std::cout << "  [" << tipoEventoAString(pila[i].tipo) << "] "
                  << pila[i].descripcion << "\n";
    }
}
