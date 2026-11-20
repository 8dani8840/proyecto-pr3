#include "structures/ArbolAVL.hpp"
#include <iostream>

// ============================================================
// ArbolAVL — Implementación con DeSiGNAR::AVLTree
// ============================================================

void ArbolAVL::insertar(const Pista& pista) {
    Pista* exist = buscar(pista.id);
    if (exist != nullptr) {
        *exist = pista;
        return;
    }
    arbol.insert(pista);
}

bool ArbolAVL::eliminar(int id) {
    Pista dummy(id, "", "");
    return arbol.remove(dummy);
}

Pista* ArbolAVL::buscar(int id) {
    Pista dummy(id, "", "");
    return arbol.search(dummy);
}

std::vector<Pista> ArbolAVL::obtenerOrdenado() const {
    std::vector<Pista> resultado;
    resultado.reserve(arbol.size());
    for (typename Designar::AVLTree<Pista, CmpPista>::InorderIterator it(arbol);
         it.has_current(); it.next()) {
        resultado.push_back(it.get_current());
    }
    return resultado;
}

std::vector<Pista> ArbolAVL::filtrarPorCategoria(const std::string& categoria) const {
    std::vector<Pista> resultado;
    for (typename Designar::AVLTree<Pista, CmpPista>::InorderIterator it(arbol);
         it.has_current(); it.next()) {
        const auto& p = it.get_current();
        if (categoriaAString(p.categoria) == categoria) {
            resultado.push_back(p);
        }
    }
    return resultado;
}

void ArbolAVL::mostrarExpediente() const {
    std::cout << "\n=== EXPEDIENTE DE PISTAS (AVL — DeSiGNAR) ===\n";
    if (arbol.is_empty()) {
        std::cout << "  (El expediente esta vacio)\n";
        return;
    }
    for (typename Designar::AVLTree<Pista, CmpPista>::InorderIterator it(arbol);
         it.has_current(); it.next()) {
        const auto& p = it.get_current();
        std::cout << "  [ID:" << p.id << "] [" << categoriaAString(p.categoria) << "] "
                  << p.titulo
                  << (p.analizada ? " (ANALIZADA)" : " (PENDIENTE)")
                  << "\n    " << p.descripcion << "\n";
    }
}
