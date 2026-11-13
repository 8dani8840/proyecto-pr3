#ifndef EVENTO_HPP
#define EVENTO_HPP

#include <string>
#include <chrono>

// ============================================================
// TipoEvento: clasifica las acciones y alertas del juego.
// ============================================================
enum class TipoEvento {
    ACCION,       // El jugador realizó una acción (abrió puerta, recogió objeto)
    ALERTA,       // Alerta narrativa (texto de la investigadora)
    DESCUBRIMIENTO, // Se descubrió una pista nueva
    DECISION,     // El jugador tomó una decisión de narrativa
    SISTEMA       // Evento interno del motor
};

inline std::string tipoEventoAString(TipoEvento tipo) {
    switch (tipo) {
        case TipoEvento::ACCION:          return "ACCION";
        case TipoEvento::ALERTA:          return "ALERTA";
        case TipoEvento::DESCUBRIMIENTO:  return "DESCUBRIMIENTO";
        case TipoEvento::DECISION:        return "DECISION";
        case TipoEvento::SISTEMA:         return "SISTEMA";
        default:                          return "DESCONOCIDO";
    }
}

// ============================================================
// Evento: unidad de dato para Pila (historial) y Cola (alertas).
// ============================================================
struct Evento {
    TipoEvento tipo;
    std::string descripcion; // Texto narrativo del evento
    std::string timestamp;   // Momento en que ocurrió (simplificado como string)

    Evento() : tipo(TipoEvento::SISTEMA), descripcion(""), timestamp("") {}

    Evento(TipoEvento tipo, const std::string& desc, const std::string& ts = "")
        : tipo(tipo), descripcion(desc), timestamp(ts) {}
};

#endif // EVENTO_HPP
