#ifndef COLA_ALERTAS_HPP
#define COLA_ALERTAS_HPP

#include "game/Evento.hpp"
#include <deque>
#include <stdexcept>
#include <vector>

// ============================================================
// ColaAlertas: Cola FIFO (First-In, First-Out).
// Gestiona las alertas narrativas y mensajes de la investigadora
// que se muestran en pantalla en orden de llegada.
// Complejidad: O(1) para enqueue y dequeue.
// ============================================================
class ColaAlertas {
private:
    static const int CAPACIDAD_MAX = 50; // Límite de alertas pendientes
    std::deque<Evento> cola;

public:
    ColaAlertas() = default;
    ~ColaAlertas() = default;

    // Agrega una alerta al final de la cola
    void enqueue(const Evento& evento);

    // Elimina y retorna la alerta del frente
    Evento dequeue();

    // Consulta la alerta del frente sin eliminarla
    const Evento& frente() const;

    bool estaVacia() const { return cola.empty(); }
    int tamano() const { return static_cast<int>(cola.size()); }

    // Retorna todas las alertas pendientes (sin eliminarlas)
    std::vector<Evento> obtenerPendientes() const;

    void limpiar() { cola.clear(); }
    void mostrarAlertas() const;
};

#endif // COLA_ALERTAS_HPP
