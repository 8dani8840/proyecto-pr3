// ============================================================
// test_inventario.cpp — Tests del módulo InventarioHash
// Verifica inserción, búsqueda, eliminación y colisiones.
// ============================================================

#include <iostream>
#include <cassert>
#include "structures/InventarioHash.hpp"

static int passed = 0;
static int failed = 0;

#define ASSERT(cond, msg) \
    if (cond) { std::cout << "  [OK] " << msg << "\n"; ++passed; } \
    else { std::cout << "  [FAIL] " << msg << "\n"; ++failed; }

void testInsercionYBusqueda() {
    std::cout << "\n-- Test: Insercion y Busqueda --\n";
    InventarioHash inv;

    inv.insertar("llave_b", "Llave B", "Abre el estudio.");
    inv.insertar("nota_01", "Nota #01", "Fragmento del diario.");

    ASSERT(inv.buscar("llave_b") != nullptr, "Buscar clave existente retorna puntero valido");
    ASSERT(inv.buscar("llave_b")->nombre == "Llave B", "Nombre del objeto correcto");
    ASSERT(inv.buscar("inexistente") == nullptr, "Buscar clave inexistente retorna nullptr");
}

void testActualizacion() {
    std::cout << "\n-- Test: Actualizacion al insertar duplicado --\n";
    InventarioHash inv;

    inv.insertar("obj_x", "Nombre Original", "Desc original.");
    inv.insertar("obj_x", "Nombre Actualizado", "Desc actualizada.");

    ObjetoInventario* obj = inv.buscar("obj_x");
    ASSERT(obj != nullptr, "Objeto sigue existiendo tras actualización");
    ASSERT(obj->nombre == "Nombre Actualizado", "Nombre fue actualizado correctamente");
    ASSERT(inv.contarObjetos() == 1, "Solo hay 1 objeto (no duplicado)");
}

void testEliminacion() {
    std::cout << "\n-- Test: Eliminacion --\n";
    InventarioHash inv;

    inv.insertar("clave_1", "Objeto 1", "Descripcion 1.");
    inv.insertar("clave_2", "Objeto 2", "Descripcion 2.");

    bool eliminado = inv.eliminar("clave_1");
    ASSERT(eliminado, "Eliminar objeto existente retorna true");
    ASSERT(inv.buscar("clave_1") == nullptr, "Objeto eliminado no es encontrado");
    ASSERT(inv.contarObjetos() == 1, "Queda 1 objeto en la tabla");
    ASSERT(!inv.eliminar("no_existe"), "Eliminar clave inexistente retorna false");
}

void testMarcarUsado() {
    std::cout << "\n-- Test: Marcar objeto como usado --\n";
    InventarioHash inv;
    inv.insertar("linterna", "Linterna", "Ilumina zonas oscuras.");

    ASSERT(!inv.buscar("linterna")->usado, "Objeto recién insertado no está usado");
    inv.marcarUsado("linterna");
    ASSERT(inv.buscar("linterna")->usado, "Objeto marcado como usado correctamente");
    ASSERT(!inv.marcarUsado("no_existe"), "marcarUsado con clave inexistente retorna false");
}

void testContarYObtenerTodos() {
    std::cout << "\n-- Test: Contar y obtenerTodos --\n";
    InventarioHash inv;

    ASSERT(inv.contarObjetos() == 0, "Inventario vacío tiene 0 objetos");

    inv.insertar("a", "A", ".");
    inv.insertar("b", "B", ".");
    inv.insertar("c", "C", ".");
    ASSERT(inv.contarObjetos() == 3, "Tras 3 inserciones, contarObjetos() == 3");

    auto todos = inv.obtenerTodos();
    ASSERT(todos.size() == 3, "obtenerTodos() retorna 3 elementos");
}

int main() {
    std::cout << "╔══════════════════════════════════════╗\n";
    std::cout << "║  TEST — InventarioHash (Tabla Hash)  ║\n";
    std::cout << "╚══════════════════════════════════════╝\n";

    testInsercionYBusqueda();
    testActualizacion();
    testEliminacion();
    testMarcarUsado();
    testContarYObtenerTodos();

    std::cout << "\n========================================\n";
    std::cout << "  Resultados: " << passed << " OK, " << failed << " FAIL\n";
    std::cout << "========================================\n";

    return (failed == 0) ? 0 : 1;
}
