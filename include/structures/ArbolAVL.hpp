#ifndef ARBOL_AVL_HPP
#define ARBOL_AVL_HPP

#include "game/Pista.hpp"
#include <vector>
#include <string>
#include <avltree.hpp>

// ============================================================
// ArbolAVL: Árbol Binario de Búsqueda autobalanceado (AVL).
// Respaldado internamente por DeSiGNAR::AVLTree.
// ============================================================
class ArbolAVL {
public:
    struct CmpPista {
        bool operator()(const Pista& a, const Pista& b) const {
            return a.id < b.id;
        }
    };

private:
    Designar::AVLTree<Pista, CmpPista> arbol;

public:
    ArbolAVL() = default;
    ~ArbolAVL() = default;

    ArbolAVL(const ArbolAVL&) = default;
    ArbolAVL& operator=(const ArbolAVL&) = default;

    void insertar(const Pista& pista);
    bool eliminar(int id);
    Pista* buscar(int id);

    std::vector<Pista> obtenerOrdenado() const;
    std::vector<Pista> filtrarPorCategoria(const std::string& categoria) const;

    int getTotalPistas() const { return static_cast<int>(arbol.size()); }
    bool estaVacio() const { return arbol.is_empty(); }

    void mostrarExpediente() const;
};

#endif // ARBOL_AVL_HPP
