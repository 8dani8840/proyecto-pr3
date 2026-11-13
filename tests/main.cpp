#include <iostream>
#include "structures/InventarioHash.hpp"

int main() {
    std::cout << "Inicializando Cold Case - Enigma Byte Studio...\n";

    InventarioHash inventario;
    inventario.insertar("llave_oxida", "Llave Oxidadas", "Abre la puerta del despacho principal.");
    inventario.insertar("nota_diario", "Página del Diario", "Fragmento con fecha de hace 20 años.");

    inventario.mostrarInventario();

    ObjetoInventario* obj = inventario.buscar("llave_oxida");
    if (obj) {
        std::cout << "\nObjeto encontrado instantáneamente: " << obj->nombre << "\n";
    }

    return 0;
}