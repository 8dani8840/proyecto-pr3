#include "structures/InventarioHash.hpp"
#include <iostream>

// ============================================================
// InventarioHash — Implementación con DeSiGNAR
// ============================================================

InventarioHash::InventarioHash()
    : tabla(19, CmpObjeto(), &InventarioHash::hashObjeto) {}

void InventarioHash::insertar(const std::string& clave, const std::string& nombre,
                               const std::string& descripcion) {
    ObjetoInventario* exist = buscar(clave);
    if (exist != nullptr) {
        exist->nombre = nombre;
        exist->descripcion = descripcion;
        return;
    }

    ObjetoInventario nuevoObj(clave, nombre, descripcion, false);
    tabla.insert(nuevoObj);
}

ObjetoInventario* InventarioHash::buscar(const std::string& clave) {
    ObjetoInventario dummy(clave, "", "");
    return tabla.search(dummy);
}

bool InventarioHash::eliminar(const std::string& clave) {
    ObjetoInventario dummy(clave, "", "");
    return tabla.remove(dummy);
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
    return static_cast<int>(tabla.size());
}

std::vector<ObjetoInventario> InventarioHash::obtenerTodos() const {
    std::vector<ObjetoInventario> resultado;
    resultado.reserve(tabla.size());
    for (typename Designar::SeparateChainingHashTable<ObjetoInventario, CmpObjeto>::Iterator it(tabla);
         it.has_current(); it.next()) {
        resultado.push_back(it.get_current());
    }
    return resultado;
}

void InventarioHash::mostrarInventario() const {
    std::cout << "\n=== INVENTARIO (Tabla Hash — DeSiGNAR) ===\n";
    if (tabla.is_empty()) {
        std::cout << "  (El inventario esta vacio)\n";
        return;
    }
    for (typename Designar::SeparateChainingHashTable<ObjetoInventario, CmpObjeto>::Iterator it(tabla);
         it.has_current(); it.next()) {
        const auto& obj = it.get_current();
        std::cout << "  [" << obj.clave << "] "
                  << obj.nombre
                  << (obj.usado ? " (USADO)" : "")
                  << "\n    " << obj.descripcion << "\n";
    }
}
