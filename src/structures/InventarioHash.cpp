#include "structures/InventarioHash.hpp"
#include <iostream>

// ============================================================
// InventarioHash — Implementación
// Tabla Hash con encadenamiento (chaining) para O(1) amortizado.
// ============================================================

InventarioHash::InventarioHash() {
    for (int i = 0; i < CAPACIDAD; ++i) {
        tabla[i] = nullptr;
    }
}

InventarioHash::~InventarioHash() {
    for (int i = 0; i < CAPACIDAD; ++i) {
        Nodo* actual = tabla[i];
        while (actual != nullptr) {
            Nodo* aBorrar = actual;
            actual = actual->siguiente;
            delete aBorrar;
        }
        tabla[i] = nullptr;
    }
}

// Función hash djb2 — minimiza colisiones para strings
int InventarioHash::funcionHash(const std::string& clave) const {
    unsigned long hash = 5381;
    for (char c : clave) {
        hash = ((hash << 5) + hash) + static_cast<unsigned char>(c);
    }
    return static_cast<int>(hash % CAPACIDAD);
}

void InventarioHash::insertar(const std::string& clave, const std::string& nombre,
                               const std::string& descripcion) {
    int indice = funcionHash(clave);
    ObjetoInventario nuevoObj = {clave, nombre, descripcion, false};

    // Si ya existe la clave, actualizar
    Nodo* actual = tabla[indice];
    while (actual != nullptr) {
        if (actual->objeto.clave == clave) {
            actual->objeto.nombre = nombre;
            actual->objeto.descripcion = descripcion;
            return;
        }
        actual = actual->siguiente;
    }

    // Insertar al frente de la cadena (O(1))
    Nodo* nuevoNodo = new Nodo(nuevoObj);
    nuevoNodo->siguiente = tabla[indice];
    tabla[indice] = nuevoNodo;
}

ObjetoInventario* InventarioHash::buscar(const std::string& clave) {
    int indice = funcionHash(clave);
    Nodo* actual = tabla[indice];

    while (actual != nullptr) {
        if (actual->objeto.clave == clave) {
            return &(actual->objeto);
        }
        actual = actual->siguiente;
    }
    return nullptr; // No encontrado
}

bool InventarioHash::eliminar(const std::string& clave) {
    int indice = funcionHash(clave);
    Nodo* actual = tabla[indice];
    Nodo* anterior = nullptr;

    while (actual != nullptr) {
        if (actual->objeto.clave == clave) {
            if (anterior == nullptr) {
                tabla[indice] = actual->siguiente;
            } else {
                anterior->siguiente = actual->siguiente;
            }
            delete actual;
            return true;
        }
        anterior = actual;
        actual = actual->siguiente;
    }
    return false; // No encontrado
}

bool InventarioHash::marcarUsado(const std::string& clave) {
    ObjetoInventario* obj = buscar(clave);
    if (obj != nullptr) {
        obj->usado = true;
        return true;
    }
    return false;
}

int InventarioHash::contarObjetos() const {
    int total = 0;
    for (int i = 0; i < CAPACIDAD; ++i) {
        Nodo* actual = tabla[i];
        while (actual != nullptr) {
            ++total;
            actual = actual->siguiente;
        }
    }
    return total;
}

std::vector<ObjetoInventario> InventarioHash::obtenerTodos() const {
    std::vector<ObjetoInventario> resultado;
    for (int i = 0; i < CAPACIDAD; ++i) {
        Nodo* actual = tabla[i];
        while (actual != nullptr) {
            resultado.push_back(actual->objeto);
            actual = actual->siguiente;
        }
    }
    return resultado;
}

void InventarioHash::mostrarInventario() const {
    std::cout << "\n=== INVENTARIO (Tabla Hash — O(1)) ===\n";
    bool vacio = true;
    for (int i = 0; i < CAPACIDAD; ++i) {
        Nodo* actual = tabla[i];
        while (actual != nullptr) {
            vacio = false;
            std::cout << "  [" << actual->objeto.clave << "] "
                      << actual->objeto.nombre
                      << (actual->objeto.usado ? " (USADO)" : "")
                      << "\n    " << actual->objeto.descripcion << "\n";
            actual = actual->siguiente;
        }
    }
    if (vacio) {
        std::cout << "  (El inventario esta vacio)\n";
    }
}
