#ifndef INVENTARIO_HASH_HPP
#define INVENTARIO_HASH_HPP

#include <string>
#include <vector>

// ============================================================
// ObjetoInventario: representa un ítem recogible en el juego.
// ============================================================
struct ObjetoInventario {
    std::string clave;       // ID único, ej: "llave_oxida"
    std::string nombre;      // Nombre visible, ej: "Llave de Latón Antiguo"
    std::string descripcion; // Descripción narrativa
    bool usado;              // Si ya fue utilizado en una escena
};

// ============================================================
// InventarioHash: Tabla Hash con encadenamiento (chaining).
// Complejidad de inserción / búsqueda / eliminación: O(1) amortizado.
// ============================================================
class InventarioHash {
private:
    static const int CAPACIDAD = 19; // Número primo para minimizar colisiones

    struct Nodo {
        ObjetoInventario objeto;
        Nodo* siguiente;
        explicit Nodo(const ObjetoInventario& obj) : objeto(obj), siguiente(nullptr) {}
    };

    Nodo* tabla[CAPACIDAD];
    int funcionHash(const std::string& clave) const; // djb2

public:
    InventarioHash();
    ~InventarioHash();

    // No permitir copia accidental (gestión manual de memoria)
    InventarioHash(const InventarioHash&) = delete;
    InventarioHash& operator=(const InventarioHash&) = delete;

    void insertar(const std::string& clave, const std::string& nombre, const std::string& descripcion);
    ObjetoInventario* buscar(const std::string& clave);
    bool eliminar(const std::string& clave);
    bool marcarUsado(const std::string& clave);
    int contarObjetos() const;
    std::vector<ObjetoInventario> obtenerTodos() const;
    void mostrarInventario() const;
};

#endif // INVENTARIO_HASH_HPP
