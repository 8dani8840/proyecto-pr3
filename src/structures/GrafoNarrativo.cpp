#include "structures/GrafoNarrativo.hpp"
#include <iostream>
#include <algorithm>

// ============================================================
// GrafoNarrativo — Implementación
// Grafo dirigido con DFS + Backtracking para rutas narrativas.
// ============================================================

GrafoNarrativo::GrafoNarrativo() : nodoActual(0) {}

void GrafoNarrativo::agregarNodo(const NodoNarrativo& nodo) {
    nodos[nodo.id] = nodo;
    // Asegura que exista entrada en adyacencia aunque sin aristas
    if (adyacencia.find(nodo.id) == adyacencia.end()) {
        adyacencia[nodo.id] = {};
    }
}

void GrafoNarrativo::agregarArista(int idOrigen, const AristaDecision& arista) {
    adyacencia[idOrigen].push_back(arista);
}

NodoNarrativo* GrafoNarrativo::getNodo(int id) {
    auto it = nodos.find(id);
    return (it != nodos.end()) ? &(it->second) : nullptr;
}

void GrafoNarrativo::desbloquearNodo(int id) {
    auto it = nodos.find(id);
    if (it != nodos.end()) {
        it->second.desbloqueado = true;
    }
}

// Retorna las decisiones disponibles para un nodo dado el inventario actual
std::vector<AristaDecision> GrafoNarrativo::getDecisiones(
        int id, const std::vector<std::string>& inventarioClaves) const {

    std::vector<AristaDecision> disponibles;
    auto it = adyacencia.find(id);
    if (it == adyacencia.end()) return disponibles;

    for (const auto& arista : it->second) {
        // Si no tiene condición, siempre disponible
        if (arista.condicion.empty()) {
            disponibles.push_back(arista);
            continue;
        }
        // Verificar si el jugador tiene el objeto/pista requerida
        bool tieneCondicion = std::find(inventarioClaves.begin(),
                                         inventarioClaves.end(),
                                         arista.condicion) != inventarioClaves.end();
        if (tieneCondicion) {
            disponibles.push_back(arista);
        }
    }
    return disponibles;
}

bool GrafoNarrativo::moverA(int idDestino, const std::vector<std::string>& inventarioClaves) {
    auto it = nodos.find(idDestino);
    if (it == nodos.end()) return false;

    // Verificar que el nodo esté desbloqueado o no tenga condición
    NodoNarrativo& nodoDestino = it->second;
    if (!nodoDestino.desbloqueado && !nodoDestino.condicion.empty()) {
        bool tieneCondicion = std::find(inventarioClaves.begin(),
                                         inventarioClaves.end(),
                                         nodoDestino.condicion) != inventarioClaves.end();
        if (!tieneCondicion) return false;
    }

    nodoActual = idDestino;
    nodoDestino.desbloqueado = true;
    return true;
}

// DFS con backtracking para encontrar camino hacia un final
bool GrafoNarrativo::dfsBacktracking(int actual, int destino,
                                      std::vector<int>& camino,
                                      std::vector<bool>& visitado,
                                      const std::vector<std::string>& inventarioClaves) const {
    if (actual == destino) return true;

    auto it = adyacencia.find(actual);
    if (it == adyacencia.end()) return false;

    for (const auto& arista : it->second) {
        int siguiente = arista.idDestino;

        // Verificar que el índice es válido para el vector de visitados
        if (siguiente < 0 || siguiente >= static_cast<int>(visitado.size())) continue;
        if (visitado[siguiente]) continue;

        // Verificar condición de la arista
        if (!arista.condicion.empty()) {
            bool tiene = std::find(inventarioClaves.begin(),
                                   inventarioClaves.end(),
                                   arista.condicion) != inventarioClaves.end();
            if (!tiene) continue;
        }

        visitado[siguiente] = true;
        camino.push_back(siguiente);

        if (dfsBacktracking(siguiente, destino, camino, visitado, inventarioClaves))
            return true;

        // Backtrack
        camino.pop_back();
        visitado[siguiente] = false;
    }
    return false;
}

std::vector<int> GrafoNarrativo::encontrarCaminoAFinal(
        const std::vector<std::string>& inventarioClaves) const {

    // Encontrar el ID máximo para dimensionar el vector de visitados
    int maxId = 0;
    for (const auto& par : nodos) {
        maxId = std::max(maxId, par.first);
    }

    for (const auto& par : nodos) {
        if (par.second.esFinale) {
            std::vector<int> camino = {nodoActual};
            std::vector<bool> visitado(maxId + 1, false);
            visitado[nodoActual] = true;

            if (dfsBacktracking(nodoActual, par.first, camino, visitado, inventarioClaves)) {
                return camino;
            }
        }
    }
    return {}; // Sin camino disponible con el inventario actual
}

void GrafoNarrativo::mostrarGrafo() const {
    std::cout << "\n=== GRAFO NARRATIVO ===\n";
    for (const auto& par : nodos) {
        const NodoNarrativo& n = par.second;
        std::cout << "  [" << n.id << "] " << n.titulo
                  << (n.desbloqueado ? " [DESBLOQUEADO]" : " [BLOQUEADO]")
                  << (n.esFinale ? " [FINAL]" : "")
                  << "\n";
        auto it = adyacencia.find(n.id);
        if (it != adyacencia.end()) {
            for (const auto& arista : it->second) {
                std::cout << "    --> [" << arista.idDestino << "] \""
                          << arista.textoDecision << "\"";
                if (!arista.condicion.empty()) {
                    std::cout << " (requiere: " << arista.condicion << ")";
                }
                std::cout << "\n";
            }
        }
    }
}
