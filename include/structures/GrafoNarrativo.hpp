#ifndef GRAFO_NARRATIVO_HPP
#define GRAFO_NARRATIVO_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

// ============================================================
// NodoNarrativo: representa un estado/escena de la historia.
// ============================================================
struct NodoNarrativo {
    int id;
    std::string titulo;        // Nombre corto de la escena
    std::string descripcion;   // Texto narrativo completo
    bool desbloqueado;         // Si el jugador puede acceder
    bool esFinale;             // Si es un final del juego
    std::string condicion;     // Clave de pista/objeto requerido (vacío = libre)

    NodoNarrativo() : id(-1), titulo(""), descripcion(""), desbloqueado(false), esFinale(false), condicion("") {}
    NodoNarrativo(int id, const std::string& titulo, const std::string& desc,
                  bool desbloqueado = false, bool esFinale = false,
                  const std::string& condicion = "")
        : id(id), titulo(titulo), descripcion(desc),
          desbloqueado(desbloqueado), esFinale(esFinale), condicion(condicion) {}
};

// ============================================================
// AristaDecision: conexión dirigida entre nodos narrativos.
// Representa una decisión que el jugador puede tomar.
// ============================================================
struct AristaDecision {
    int idDestino;
    std::string textoDecision;    // Texto del botón de elección
    std::string condicion;        // Clave de objeto/pista requerida (vacío = libre)

    AristaDecision(int dest, const std::string& texto, const std::string& cond = "")
        : idDestino(dest), textoDecision(texto), condicion(cond) {}
};

// ============================================================
// GrafoNarrativo: Grafo dirigido de la narrativa ramificada.
// Implementa DFS para explorar caminos y backtracking para
// encontrar rutas alternativas hacia los finales.
// ============================================================
class GrafoNarrativo {
private:
    std::unordered_map<int, NodoNarrativo> nodos;
    std::unordered_map<int, std::vector<AristaDecision>> adyacencia;
    int nodoActual;

    // DFS con backtracking para encontrar caminos hacia finales
    bool dfsBacktracking(int actual, int destino,
                         std::vector<int>& camino,
                         std::vector<bool>& visitado,
                         const std::vector<std::string>& inventarioClaves) const;

public:
    GrafoNarrativo();
    ~GrafoNarrativo() = default;

    // Construcción del grafo
    void agregarNodo(const NodoNarrativo& nodo);
    void agregarArista(int idOrigen, const AristaDecision& arista);

    // Navegación
    int getNodoActual() const { return nodoActual; }
    void setNodoActual(int id) { nodoActual = id; }
    bool moverA(int idDestino, const std::vector<std::string>& inventarioClaves);

    // Consultas
    NodoNarrativo* getNodo(int id);
    std::vector<AristaDecision> getDecisiones(int id,
                                               const std::vector<std::string>& inventarioClaves) const;
    std::vector<int> encontrarCaminoAFinal(const std::vector<std::string>& inventarioClaves) const;

    // Desbloquear nodo (al descubrir pista clave)
    void desbloquearNodo(int id);

    // Debug
    void mostrarGrafo() const;
};

#endif // GRAFO_NARRATIVO_HPP
