#include "structures/PilaEventos.hpp"
#include <iostream>
#include <algorithm>

// ============================================================
// PilaEventos — Implementación con DeSiGNAR::DynStack
// ============================================================

void PilaEventos::push(const Evento& evento) {
    if (static_cast<int>(pila.size()) >= CAPACIDAD_MAX) {
        // En caso de alcanzar CAPACIDAD_MAX, descartamos el más antiguo (fondo)
        // Para preservar los elementos más recientes en orden LIFO:
        std::vector<Evento> temp;
        while (!pila.is_empty()) {
            temp.push_back(pila.pop());
        }
        // temp[0] es el tope más reciente, temp.back() es el fondo (más antiguo)
        temp.pop_back(); // descartar el fondo
        for (int i = static_cast<int>(temp.size()) - 1; i >= 0; --i) {
            pila.push(temp[i]);
        }
    }
    pila.push(evento);
}

Evento PilaEventos::pop() {
    if (estaVacia()) {
        throw std::underflow_error("PilaEventos: intento de pop() en pila vacia.");
    }
    return pila.pop();
}

const Evento& PilaEventos::peek() const {
    if (estaVacia()) {
        throw std::underflow_error("PilaEventos: intento de peek() en pila vacia.");
    }
    return pila.top();
}

std::vector<Evento> PilaEventos::obtenerUltimos(int n) const {
    std::vector<Evento> todos = obtenerTodos();
    if (n >= static_cast<int>(todos.size())) {
        return todos;
    }
    return std::vector<Evento>(todos.begin(), todos.begin() + n);
}

std::vector<Evento> PilaEventos::obtenerTodos() const {
    // Para no vaciar la pila original, hacemos una copia temporal
    Designar::DynStack<Evento> copia = pila;
    std::vector<Evento> resultado;
    resultado.reserve(copia.size());
    while (!copia.is_empty()) {
        resultado.push_back(copia.pop());
    }
    return resultado;
}

void PilaEventos::mostrarHistorial() const {
    std::cout << "\n=== HISTORIAL DE ACCIONES (Pila LIFO — DeSiGNAR) ===\n";
    if (estaVacia()) {
        std::cout << "  (Sin acciones registradas)\n";
        return;
    }
    std::vector<Evento> todos = obtenerTodos();
    for (const auto& ev : todos) {
        std::cout << "  [" << tipoEventoAString(ev.tipo) << "] "
                  << ev.descripcion << "\n";
    }
}
