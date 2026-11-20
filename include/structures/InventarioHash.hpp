#ifndef INVENTARIO_HASH_HPP
#define INVENTARIO_HASH_HPP

#include <string>
#include <vector>
#include <chainedhash.hpp>
#include <hash.hpp>

// ============================================================
// ObjetoInventario: representa un ítem recogible en el juego.
// ============================================================
struct ObjetoInventario {
    std::string clave;       // ID único, ej: "llave_oxida"
    std::string nombre;      // Nombre visible, ej: "Llave de Latón Antiguo"
    std::string descripcion; // Descripción narrativa
    bool usado;              // Si ya fue utilizado en una escena

    ObjetoInventario()
        : clave(""), nombre(""), descripcion(""), usado(false) {}

    ObjetoInventario(const std::string& c, const std::string& n,
                     const std::string& d, bool u = false)
        : clave(c), nombre(n), descripcion(d), usado(u) {}

    bool operator==(const ObjetoInventario& otro) const {
        return clave == otro.clave;
    }
};

// ============================================================
// InventarioHash: Tabla Hash respaldada por DeSiGNAR::SeparateChainingHashTable.
// ============================================================
class InventarioHash {
public:
    struct CmpObjeto {
        bool operator()(const ObjetoInventario& a, const ObjetoInventario& b) const {
            return a.clave == b.clave;
        }
    };

private:
    static Designar::nat_t hashObjeto(const ObjetoInventario& obj) {
        return Designar::super_fast_hash(obj.clave);
    }

    Designar::SeparateChainingHashTable<ObjetoInventario, CmpObjeto> tabla;

public:
    InventarioHash();
    ~InventarioHash() = default;

    InventarioHash(const InventarioHash&) = default;
    InventarioHash& operator=(const InventarioHash&) = default;

    void insertar(const std::string& clave, const std::string& nombre, const std::string& descripcion);
    ObjetoInventario* buscar(const std::string& clave);
    bool eliminar(const std::string& clave);
    bool marcarUsado(const std::string& clave);
    int contarObjetos() const;
    std::vector<ObjetoInventario> obtenerTodos() const;
    void mostrarInventario() const;
};

#endif // INVENTARIO_HASH_HPP
