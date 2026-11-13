#ifndef ARBOL_AVL_HPP
#define ARBOL_AVL_HPP

#include "game/Pista.hpp"
#include <vector>
#include <functional>
#include <string>

// ============================================================
// ArbolAVL: Árbol Binario de Búsqueda autobalanceado (AVL).
// Almacena las Pistas descubiertas por la investigadora.
// Complejidad de inserción / búsqueda / eliminación: O(log n).
// ============================================================
class ArbolAVL {
private:
    // ---- Nodo interno del árbol ----
    struct NodoAVL {
        Pista pista;
        NodoAVL* izquierdo;
        NodoAVL* derecho;
        int altura;

        explicit NodoAVL(const Pista& p)
            : pista(p), izquierdo(nullptr), derecho(nullptr), altura(1) {}
    };

    NodoAVL* raiz;
    int totalPistas;

    // ---- Métodos privados de balanceo ----
    int altura(NodoAVL* nodo) const;
    int factorBalance(NodoAVL* nodo) const;
    void actualizarAltura(NodoAVL* nodo);

    NodoAVL* rotarDerecha(NodoAVL* y);
    NodoAVL* rotarIzquierda(NodoAVL* x);
    NodoAVL* balancear(NodoAVL* nodo);

    NodoAVL* insertar(NodoAVL* nodo, const Pista& pista);
    NodoAVL* eliminar(NodoAVL* nodo, int id);
    NodoAVL* buscar(NodoAVL* nodo, int id) const;
    NodoAVL* minimoNodo(NodoAVL* nodo) const;

    // Recorridos
    void inorden(NodoAVL* nodo, std::vector<Pista>& resultado) const;
    void buscarPorCategoria(NodoAVL* nodo, const std::string& categoria, std::vector<Pista>& resultado) const;
    void destruir(NodoAVL* nodo);

public:
    ArbolAVL();
    ~ArbolAVL();

    // No permitir copia accidental
    ArbolAVL(const ArbolAVL&) = delete;
    ArbolAVL& operator=(const ArbolAVL&) = delete;

    void insertar(const Pista& pista);
    bool eliminar(int id);
    Pista* buscar(int id);

    // Recorrido ordenado (inorden = IDs ascendentes)
    std::vector<Pista> obtenerOrdenado() const;

    // Búsqueda por categoría
    std::vector<Pista> filtrarPorCategoria(const std::string& categoria) const;

    int getTotalPistas() const { return totalPistas; }
    bool estaVacio() const { return raiz == nullptr; }

    void mostrarExpediente() const;
};

#endif // ARBOL_AVL_HPP
