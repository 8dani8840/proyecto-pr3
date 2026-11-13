// ============================================================
// test_avl.cpp — Tests del módulo ArbolAVL
// Verifica inserción, búsqueda, eliminación y balanceo.
// ============================================================

#include <iostream>
#include <cassert>
#include "structures/ArbolAVL.hpp"

static int passed = 0;
static int failed = 0;

#define ASSERT(cond, msg) \
    if (cond) { std::cout << "  [OK] " << msg << "\n"; ++passed; } \
    else { std::cout << "  [FAIL] " << msg << "\n"; ++failed; }

void testInsercionYBusqueda() {
    std::cout << "\n-- Test: Insercion y Busqueda --\n";
    ArbolAVL avl;

    avl.insertar(Pista(5,  "Pista Central",   "...", CategoriaPista::FISICA));
    avl.insertar(Pista(3,  "Pista Izquierda", "...", CategoriaPista::DOCUMENTAL));
    avl.insertar(Pista(7,  "Pista Derecha",   "...", CategoriaPista::TESTIMONIAL));
    avl.insertar(Pista(1,  "Pista Menor",     "...", CategoriaPista::FISICA));
    avl.insertar(Pista(10, "Pista Mayor",     "...", CategoriaPista::DIGITAL));

    ASSERT(avl.getTotalPistas() == 5, "Total pistas tras 5 inserciones == 5");
    ASSERT(avl.buscar(5) != nullptr, "Buscar ID existente retorna puntero valido");
    ASSERT(avl.buscar(5)->titulo == "Pista Central", "Titulo correcto");
    ASSERT(avl.buscar(999) == nullptr, "Buscar ID inexistente retorna nullptr");
}

void testOrdenInorden() {
    std::cout << "\n-- Test: Recorrido Inorden (Ordenado) --\n";
    ArbolAVL avl;

    // Insertar en orden aleatorio
    avl.insertar(Pista(30, "C", ".", CategoriaPista::FISICA));
    avl.insertar(Pista(10, "A", ".", CategoriaPista::FISICA));
    avl.insertar(Pista(20, "B", ".", CategoriaPista::FISICA));
    avl.insertar(Pista(50, "E", ".", CategoriaPista::FISICA));
    avl.insertar(Pista(40, "D", ".", CategoriaPista::FISICA));

    auto ordenado = avl.obtenerOrdenado();
    ASSERT(ordenado.size() == 5, "Recorrido retorna 5 pistas");
    ASSERT(ordenado[0].id == 10, "Primera pista tiene ID 10 (menor)");
    ASSERT(ordenado[4].id == 50, "Ultima pista tiene ID 50 (mayor)");

    bool estaOrdenado = true;
    for (size_t i = 1; i < ordenado.size(); ++i) {
        if (ordenado[i].id <= ordenado[i-1].id) { estaOrdenado = false; break; }
    }
    ASSERT(estaOrdenado, "Las pistas estan ordenadas ascendentemente por ID");
}

void testEliminacion() {
    std::cout << "\n-- Test: Eliminacion y Rebalanceo --\n";
    ArbolAVL avl;

    avl.insertar(Pista(5, "P5", ".", CategoriaPista::FISICA));
    avl.insertar(Pista(3, "P3", ".", CategoriaPista::FISICA));
    avl.insertar(Pista(7, "P7", ".", CategoriaPista::FISICA));

    bool eliminado = avl.eliminar(3);
    ASSERT(eliminado, "Eliminar ID existente retorna true");
    ASSERT(avl.buscar(3) == nullptr, "Pista eliminada no es encontrada");
    ASSERT(avl.getTotalPistas() == 2, "Total pistas == 2 tras eliminar");
    ASSERT(!avl.eliminar(999), "Eliminar ID inexistente retorna false");

    // Verificar que el árbol sigue ordenado
    auto ordenado = avl.obtenerOrdenado();
    ASSERT(ordenado[0].id == 5 && ordenado[1].id == 7, "Orden correcto tras eliminacion");
}

void testFiltrarPorCategoria() {
    std::cout << "\n-- Test: Filtrar por categoria --\n";
    ArbolAVL avl;

    avl.insertar(Pista(1, "F1", ".", CategoriaPista::FISICA));
    avl.insertar(Pista(2, "D1", ".", CategoriaPista::DOCUMENTAL));
    avl.insertar(Pista(3, "F2", ".", CategoriaPista::FISICA));
    avl.insertar(Pista(4, "T1", ".", CategoriaPista::TESTIMONIAL));
    avl.insertar(Pista(5, "F3", ".", CategoriaPista::FISICA));

    auto fisicas = avl.filtrarPorCategoria("Fisica");
    ASSERT(fisicas.size() == 3, "Filtro 'Fisica' retorna 3 pistas");

    auto documentales = avl.filtrarPorCategoria("Documental");
    ASSERT(documentales.size() == 1, "Filtro 'Documental' retorna 1 pista");

    auto digitales = avl.filtrarPorCategoria("Digital");
    ASSERT(digitales.empty(), "Filtro 'Digital' retorna 0 pistas (vacio)");
}

void testBalanceoAVL() {
    std::cout << "\n-- Test: Balanceo AVL (insercion en orden) --\n";
    ArbolAVL avl;

    // Insertar en orden ascendente — sin AVL sería una lista enlazada degenerada
    for (int i = 1; i <= 15; ++i) {
        avl.insertar(Pista(i, "P" + std::to_string(i), ".", CategoriaPista::FISICA));
    }

    ASSERT(avl.getTotalPistas() == 15, "15 pistas insertadas correctamente");
    ASSERT(avl.buscar(1) != nullptr, "Pista minima accesible");
    ASSERT(avl.buscar(15) != nullptr, "Pista maxima accesible");

    // Verificar que el recorrido inorden sigue correcto
    auto ord = avl.obtenerOrdenado();
    bool correcto = true;
    for (int i = 0; i < 15; ++i) {
        if (ord[i].id != i + 1) { correcto = false; break; }
    }
    ASSERT(correcto, "Recorrido inorden correcto tras 15 inserciones en orden ascendente");
}

int main() {
    std::cout << "╔══════════════════════════════════════╗\n";
    std::cout << "║    TEST — ArbolAVL (O(log n))        ║\n";
    std::cout << "╚══════════════════════════════════════╝\n";

    testInsercionYBusqueda();
    testOrdenInorden();
    testEliminacion();
    testFiltrarPorCategoria();
    testBalanceoAVL();

    std::cout << "\n========================================\n";
    std::cout << "  Resultados: " << passed << " OK, " << failed << " FAIL\n";
    std::cout << "========================================\n";

    return (failed == 0) ? 0 : 1;
}
