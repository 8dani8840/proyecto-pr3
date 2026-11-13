#include "structures/ArbolAVL.hpp"
#include <iostream>
#include <algorithm>

// ============================================================
// ArbolAVL — Implementación
// BST autobalanceado. Garantiza O(log n) para todas las ops.
// ============================================================

ArbolAVL::ArbolAVL() : raiz(nullptr), totalPistas(0) {}

ArbolAVL::~ArbolAVL() {
    destruir(raiz);
}

void ArbolAVL::destruir(NodoAVL* nodo) {
    if (nodo == nullptr) return;
    destruir(nodo->izquierdo);
    destruir(nodo->derecho);
    delete nodo;
}

// ---- Altura y factor de balance ----

int ArbolAVL::altura(NodoAVL* nodo) const {
    return (nodo == nullptr) ? 0 : nodo->altura;
}

int ArbolAVL::factorBalance(NodoAVL* nodo) const {
    return (nodo == nullptr) ? 0 : altura(nodo->izquierdo) - altura(nodo->derecho);
}

void ArbolAVL::actualizarAltura(NodoAVL* nodo) {
    if (nodo != nullptr) {
        nodo->altura = 1 + std::max(altura(nodo->izquierdo), altura(nodo->derecho));
    }
}

// ---- Rotaciones ----

// Rotación simple derecha (caso izquierda-izquierda)
ArbolAVL::NodoAVL* ArbolAVL::rotarDerecha(NodoAVL* y) {
    NodoAVL* x = y->izquierdo;
    NodoAVL* T2 = x->derecho;

    x->derecho = y;
    y->izquierdo = T2;

    actualizarAltura(y);
    actualizarAltura(x);
    return x;
}

// Rotación simple izquierda (caso derecha-derecha)
ArbolAVL::NodoAVL* ArbolAVL::rotarIzquierda(NodoAVL* x) {
    NodoAVL* y = x->derecho;
    NodoAVL* T2 = y->izquierdo;

    y->izquierdo = x;
    x->derecho = T2;

    actualizarAltura(x);
    actualizarAltura(y);
    return y;
}

// Aplica la rotación correcta según el factor de balance
ArbolAVL::NodoAVL* ArbolAVL::balancear(NodoAVL* nodo) {
    actualizarAltura(nodo);
    int fb = factorBalance(nodo);

    // Caso Izquierda-Izquierda
    if (fb > 1 && factorBalance(nodo->izquierdo) >= 0)
        return rotarDerecha(nodo);

    // Caso Izquierda-Derecha
    if (fb > 1 && factorBalance(nodo->izquierdo) < 0) {
        nodo->izquierdo = rotarIzquierda(nodo->izquierdo);
        return rotarDerecha(nodo);
    }

    // Caso Derecha-Derecha
    if (fb < -1 && factorBalance(nodo->derecho) <= 0)
        return rotarIzquierda(nodo);

    // Caso Derecha-Izquierda
    if (fb < -1 && factorBalance(nodo->derecho) > 0) {
        nodo->derecho = rotarDerecha(nodo->derecho);
        return rotarIzquierda(nodo);
    }

    return nodo; // Ya balanceado
}

// ---- Inserción ----

ArbolAVL::NodoAVL* ArbolAVL::insertar(NodoAVL* nodo, const Pista& pista) {
    if (nodo == nullptr) {
        ++totalPistas;
        return new NodoAVL(pista);
    }

    if (pista.id < nodo->pista.id)
        nodo->izquierdo = insertar(nodo->izquierdo, pista);
    else if (pista.id > nodo->pista.id)
        nodo->derecho = insertar(nodo->derecho, pista);
    else {
        // ID duplicado: actualizar datos
        nodo->pista = pista;
        return nodo;
    }

    return balancear(nodo);
}

void ArbolAVL::insertar(const Pista& pista) {
    raiz = insertar(raiz, pista);
}

// ---- Eliminación ----

ArbolAVL::NodoAVL* ArbolAVL::minimoNodo(NodoAVL* nodo) const {
    NodoAVL* actual = nodo;
    while (actual->izquierdo != nullptr)
        actual = actual->izquierdo;
    return actual;
}

ArbolAVL::NodoAVL* ArbolAVL::eliminar(NodoAVL* nodo, int id) {
    if (nodo == nullptr) return nullptr;

    if (id < nodo->pista.id)
        nodo->izquierdo = eliminar(nodo->izquierdo, id);
    else if (id > nodo->pista.id)
        nodo->derecho = eliminar(nodo->derecho, id);
    else {
        // Nodo encontrado
        if (nodo->izquierdo == nullptr || nodo->derecho == nullptr) {
            NodoAVL* temp = nodo->izquierdo ? nodo->izquierdo : nodo->derecho;
            delete nodo;
            --totalPistas;
            return temp;
        }
        // Nodo con dos hijos: reemplazar con sucesor inorden
        NodoAVL* sucesor = minimoNodo(nodo->derecho);
        nodo->pista = sucesor->pista;
        nodo->derecho = eliminar(nodo->derecho, sucesor->pista.id);
    }

    return balancear(nodo);
}

bool ArbolAVL::eliminar(int id) {
    int anteriorTotal = totalPistas;
    raiz = eliminar(raiz, id);
    return totalPistas < anteriorTotal;
}

// ---- Búsqueda ----

ArbolAVL::NodoAVL* ArbolAVL::buscar(NodoAVL* nodo, int id) const {
    if (nodo == nullptr) return nullptr;
    if (id == nodo->pista.id) return nodo;
    if (id < nodo->pista.id) return buscar(nodo->izquierdo, id);
    return buscar(nodo->derecho, id);
}

Pista* ArbolAVL::buscar(int id) {
    NodoAVL* resultado = buscar(raiz, id);
    return (resultado != nullptr) ? &(resultado->pista) : nullptr;
}

// ---- Recorridos ----

void ArbolAVL::inorden(NodoAVL* nodo, std::vector<Pista>& resultado) const {
    if (nodo == nullptr) return;
    inorden(nodo->izquierdo, resultado);
    resultado.push_back(nodo->pista);
    inorden(nodo->derecho, resultado);
}

std::vector<Pista> ArbolAVL::obtenerOrdenado() const {
    std::vector<Pista> resultado;
    inorden(raiz, resultado);
    return resultado;
}

void ArbolAVL::buscarPorCategoria(NodoAVL* nodo, const std::string& categoria,
                                   std::vector<Pista>& resultado) const {
    if (nodo == nullptr) return;
    buscarPorCategoria(nodo->izquierdo, categoria, resultado);
    if (categoriaAString(nodo->pista.categoria) == categoria) {
        resultado.push_back(nodo->pista);
    }
    buscarPorCategoria(nodo->derecho, categoria, resultado);
}

std::vector<Pista> ArbolAVL::filtrarPorCategoria(const std::string& categoria) const {
    std::vector<Pista> resultado;
    buscarPorCategoria(raiz, categoria, resultado);
    return resultado;
}

void ArbolAVL::mostrarExpediente() const {
    std::cout << "\n=== EXPEDIENTE DE PISTAS (AVL — O(log n)) ===\n";
    std::vector<Pista> pistas = obtenerOrdenado();
    if (pistas.empty()) {
        std::cout << "  (El expediente esta vacio)\n";
        return;
    }
    for (const auto& p : pistas) {
        std::cout << "  [ID:" << p.id << "] [" << categoriaAString(p.categoria) << "] "
                  << p.titulo
                  << (p.analizada ? " (ANALIZADA)" : " (PENDIENTE)")
                  << "\n    " << p.descripcion << "\n";
    }
}
