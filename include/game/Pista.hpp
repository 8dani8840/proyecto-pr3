#ifndef PISTA_HPP
#define PISTA_HPP

#include <string>

// ============================================================
// Categorías de pista para el expediente de la investigadora.
// ============================================================
enum class CategoriaPista {
    FISICA,     // Evidencia física: manchas, huellas, armas
    DOCUMENTAL, // Documentos: cartas, contratos, registros
    TESTIMONIAL,// Testimonios: grabaciones, declaraciones
    DIGITAL,    // Evidencia digital: correos, archivos
    DESCONOCIDA
};

// Convierte enum a string legible
inline std::string categoriaAString(CategoriaPista cat) {
    switch (cat) {
        case CategoriaPista::FISICA:      return "Fisica";
        case CategoriaPista::DOCUMENTAL:  return "Documental";
        case CategoriaPista::TESTIMONIAL: return "Testimonial";
        case CategoriaPista::DIGITAL:     return "Digital";
        default:                          return "Desconocida";
    }
}

// ============================================================
// Pista: unidad de evidencia que se almacena en el Árbol AVL.
// La clave de comparación es el ID numérico único.
// ============================================================
struct Pista {
    int id;                    // Clave para el AVL
    std::string titulo;        // Nombre corto de la pista
    std::string descripcion;   // Descripción narrativa completa
    CategoriaPista categoria;
    bool analizada;            // ¿Ya fue analizada por la investigadora?

    // Constructor por defecto
    Pista() : id(0), titulo(""), descripcion(""), categoria(CategoriaPista::DESCONOCIDA), analizada(false) {}

    Pista(int id, const std::string& titulo, const std::string& descripcion,
          CategoriaPista categoria = CategoriaPista::DESCONOCIDA)
        : id(id), titulo(titulo), descripcion(descripcion),
          categoria(categoria), analizada(false) {}

    bool operator<(const Pista& otro) const { return id < otro.id; }
    bool operator>(const Pista& otro) const { return id > otro.id; }
    bool operator==(const Pista& otro) const { return id == otro.id; }
};

#endif // PISTA_HPP
