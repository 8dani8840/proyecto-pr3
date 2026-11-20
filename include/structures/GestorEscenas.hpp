#ifndef GESTOR_ESCENAS_HPP
#define GESTOR_ESCENAS_HPP

#include <string>
#include <vector>
#include <chainedhash.hpp>
#include <hash.hpp>
#include <list.hpp>

// ============================================================
// NodoNarrativo / EscenaNarrativa: representa una escena del caso.
// ============================================================
struct NodoNarrativo {
    int id;
    std::string titulo;        // Nombre corto de la escena
    std::string descripcion;   // Texto narrativo completo
    bool desbloqueado;         // Si el jugador puede acceder
    bool esFinale;             // Si es un final del juego
    std::string condicion;     // Clave de pista/objeto requerido (vacío = libre)

    NodoNarrativo()
        : id(-1), titulo(""), descripcion(""), desbloqueado(false), esFinale(false), condicion("") {}

    NodoNarrativo(int id, const std::string& titulo, const std::string& desc,
                  bool desbloqueado = false, bool esFinale = false,
                  const std::string& condicion = "")
        : id(id), titulo(titulo), descripcion(desc),
          desbloqueado(desbloqueado), esFinale(esFinale), condicion(condicion) {}
};

using EscenaNarrativa = NodoNarrativo;

// ============================================================
// AristaDecision / OpcionDecision: opción elegible en una escena.
// ============================================================
struct AristaDecision {
    int idDestino;
    std::string textoDecision;    // Texto del botón de elección
    std::string condicion;        // Clave de objeto/pista requerida (vacío = libre)

    AristaDecision() : idDestino(-1), textoDecision(""), condicion("") {}
    AristaDecision(int dest, const std::string& texto, const std::string& cond = "")
        : idDestino(dest), textoDecision(texto), condicion(cond) {}
};

using OpcionDecision = AristaDecision;

// ============================================================
// RegistroEscena: entrada almacenada en la Tabla Hash de escenas.
// ============================================================
struct RegistroEscena {
    NodoNarrativo escena;
    Designar::DLList<AristaDecision> decisiones;

    RegistroEscena() = default;
    explicit RegistroEscena(const NodoNarrativo& e) : escena(e) {}

    bool operator==(const RegistroEscena& otro) const {
        return escena.id == otro.escena.id;
    }
};

// ============================================================
// GestorEscenas: Tabla Hash de escenas con lista de opciones.
// Respaldado por DeSiGNAR::SeparateChainingHashTable y Designar::DLList.
// ============================================================
class GestorEscenas {
public:
    struct CmpEscena {
        bool operator()(const RegistroEscena& a, const RegistroEscena& b) const {
            return a.escena.id == b.escena.id;
        }
    };

private:
    static Designar::nat_t hashEscena(const RegistroEscena& reg) {
        return Designar::super_fast_hash(std::to_string(reg.escena.id));
    }

    Designar::SeparateChainingHashTable<RegistroEscena, CmpEscena> tablaEscenas;
    int escenaActual;

    RegistroEscena* buscarRegistro(int id);
    const RegistroEscena* buscarRegistro(int id) const;
    static bool cumpleCondicion(const std::string& condicion,
                                const std::vector<std::string>& inventarioClaves);

public:
    GestorEscenas();
    ~GestorEscenas() = default;

    GestorEscenas(const GestorEscenas&) = default;
    GestorEscenas& operator=(const GestorEscenas&) = default;

    // Registro de escenas y transiciones (compatibles con GameEngine)
    void agregarNodo(const NodoNarrativo& nodo);
    void agregarArista(int idOrigen, const AristaDecision& arista);

    // Navegación
    int getNodoActual() const { return escenaActual; }
    void setNodoActual(int id) { escenaActual = id; }
    bool moverA(int idDestino, const std::vector<std::string>& inventarioClaves);

    // Consultas
    NodoNarrativo* getNodo(int id);
    std::vector<AristaDecision> getDecisiones(int id,
                                               const std::vector<std::string>& inventarioClaves) const;
    void desbloquearNodo(int id);

    // Debug
    void mostrarEscenas() const;
};

#endif // GESTOR_ESCENAS_HPP
