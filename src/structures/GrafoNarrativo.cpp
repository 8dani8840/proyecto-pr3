#include "structures/GrafoNarrativo.hpp"
#include <iostream>
#include <algorithm>

// ============================================================
// GrafoNarrativo — Implementación con DeSiGNAR::Digraph
// ============================================================

GrafoNarrativo::GrafoNarrativo() : nodoActual(0) {}

GrafoNarrativo::GrafoNarrativo(const GrafoNarrativo& otro) : nodoActual(otro.nodoActual) {
    // Reconstruir los nodos en la nueva instancia
    for (typename DigrafoType::NodeIterator it(otro.grafo); it.has_current(); it.next()) {
        agregarNodo(it.get_current()->get_info());
    }
    // Reconstruir las aristas
    for (typename DigrafoType::NodeIterator it(otro.grafo); it.has_current(); it.next()) {
        DNode* n = it.get_current();
        int origenId = n->get_info().id;
        for (typename DigrafoType::AdjacentArcIterator ait(otro.grafo, n); ait.has_current(); ait.next()) {
            agregarArista(origenId, ait.get_current()->get_info());
        }
    }
}

GrafoNarrativo& GrafoNarrativo::operator=(const GrafoNarrativo& otro) {
    if (this == &otro) return *this;
    grafo.clear();
    nodoActual = otro.nodoActual;
    for (typename DigrafoType::NodeIterator it(otro.grafo); it.has_current(); it.next()) {
        agregarNodo(it.get_current()->get_info());
    }
    for (typename DigrafoType::NodeIterator it(otro.grafo); it.has_current(); it.next()) {
        DNode* n = it.get_current();
        int origenId = n->get_info().id;
        for (typename DigrafoType::AdjacentArcIterator ait(otro.grafo, n); ait.has_current(); ait.next()) {
            agregarArista(origenId, ait.get_current()->get_info());
        }
    }
    return *this;
}

GrafoNarrativo::DNode* GrafoNarrativo::buscarDNode(int id) const {
    for (typename DigrafoType::NodeIterator it(grafo); it.has_current(); it.next()) {
        DNode* n = it.get_current();
        if (n->get_info().id == id) {
            return n;
        }
    }
    return nullptr;
}

void GrafoNarrativo::agregarNodo(const NodoNarrativo& nodo) {
    DNode* existente = buscarDNode(nodo.id);
    if (existente != nullptr) {
        existente->get_info() = nodo;
    } else {
        grafo.insert_node(nodo);
    }
}

void GrafoNarrativo::agregarArista(int idOrigen, const AristaDecision& arista) {
    DNode* src = buscarDNode(idOrigen);
    DNode* tgt = buscarDNode(arista.idDestino);
    if (src != nullptr && tgt != nullptr) {
        grafo.insert_arc(src, tgt, arista);
    }
}

NodoNarrativo* GrafoNarrativo::getNodo(int id) {
    DNode* n = buscarDNode(id);
    return (n != nullptr) ? &(n->get_info()) : nullptr;
}

void GrafoNarrativo::desbloquearNodo(int id) {
    DNode* n = buscarDNode(id);
    if (n != nullptr) {
        n->get_info().desbloqueado = true;
    }
}

std::vector<AristaDecision> GrafoNarrativo::getDecisiones(
        int id, const std::vector<std::string>& inventarioClaves) const {

    std::vector<AristaDecision> disponibles;
    DNode* src = buscarDNode(id);
    if (src == nullptr) return disponibles;

    for (typename DigrafoType::AdjacentArcIterator it(grafo, src); it.has_current(); it.next()) {
        const DArc* arc = it.get_current();
        const AristaDecision& decision = arc->get_info();

        if (decision.condicion.empty()) {
            disponibles.push_back(decision);
            continue;
        }

        bool tieneCondicion = std::find(inventarioClaves.begin(),
                                         inventarioClaves.end(),
                                         decision.condicion) != inventarioClaves.end();
        if (tieneCondicion) {
            disponibles.push_back(decision);
        }
    }
    return disponibles;
}

bool GrafoNarrativo::moverA(int idDestino, const std::vector<std::string>& inventarioClaves) {
    DNode* tgt = buscarDNode(idDestino);
    if (tgt == nullptr) return false;

    NodoNarrativo& nodoDestino = tgt->get_info();
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

bool GrafoNarrativo::dfsBacktracking(int actual, int destino,
                                      std::vector<int>& camino,
                                      std::vector<bool>& visitado,
                                      const std::vector<std::string>& inventarioClaves) const {
    if (actual == destino) return true;

    DNode* src = buscarDNode(actual);
    if (src == nullptr) return false;

    for (typename DigrafoType::AdjacentArcIterator it(grafo, src); it.has_current(); it.next()) {
        const DArc* arc = it.get_current();
        const AristaDecision& decision = arc->get_info();
        int siguiente = decision.idDestino;

        if (siguiente < 0 || siguiente >= static_cast<int>(visitado.size())) continue;
        if (visitado[siguiente]) continue;

        if (!decision.condicion.empty()) {
            bool tiene = std::find(inventarioClaves.begin(),
                                   inventarioClaves.end(),
                                   decision.condicion) != inventarioClaves.end();
            if (!tiene) continue;
        }

        visitado[siguiente] = true;
        camino.push_back(siguiente);

        if (dfsBacktracking(siguiente, destino, camino, visitado, inventarioClaves))
            return true;

        camino.pop_back();
        visitado[siguiente] = false;
    }
    return false;
}

std::vector<int> GrafoNarrativo::encontrarCaminoAFinal(
        const std::vector<std::string>& inventarioClaves) const {

    int maxId = 0;
    for (typename DigrafoType::NodeIterator it(grafo); it.has_current(); it.next()) {
        maxId = std::max(maxId, it.get_current()->get_info().id);
    }

    for (typename DigrafoType::NodeIterator it(grafo); it.has_current(); it.next()) {
        const NodoNarrativo& n = it.get_current()->get_info();
        if (n.esFinale) {
            std::vector<int> camino = {nodoActual};
            std::vector<bool> visitado(maxId + 1, false);
            if (nodoActual >= 0 && nodoActual <= maxId) {
                visitado[nodoActual] = true;
            }

            if (dfsBacktracking(nodoActual, n.id, camino, visitado, inventarioClaves)) {
                return camino;
            }
        }
    }
    return {};
}

void GrafoNarrativo::mostrarGrafo() const {
    std::cout << "\n=== GRAFO NARRATIVO (DeSiGNAR::Digraph) ===\n";
    for (typename DigrafoType::NodeIterator it(grafo); it.has_current(); it.next()) {
        DNode* n = it.get_current();
        const NodoNarrativo& info = n->get_info();
        std::cout << "  [" << info.id << "] " << info.titulo
                  << (info.desbloqueado ? " [DESBLOQUEADO]" : " [BLOQUEADO]")
                  << (info.esFinale ? " [FINAL]" : "")
                  << "\n";

        for (typename DigrafoType::AdjacentArcIterator ait(grafo, n); ait.has_current(); ait.next()) {
            const DArc* arc = ait.get_current();
            const AristaDecision& dec = arc->get_info();
            std::cout << "    --> [" << dec.idDestino << "] \""
                      << dec.textoDecision << "\"";
            if (!dec.condicion.empty()) {
                std::cout << " (requiere: " << dec.condicion << ")";
            }
            std::cout << "\n";
        }
    }
}
