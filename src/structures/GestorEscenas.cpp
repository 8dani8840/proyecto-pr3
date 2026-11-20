#include "structures/GestorEscenas.hpp"
#include <iostream>
#include <algorithm>

GestorEscenas::GestorEscenas()
    : tablaEscenas(32, CmpEscena(), &GestorEscenas::hashEscena), escenaActual(0) {}

RegistroEscena* GestorEscenas::buscarRegistro(int id) {
    RegistroEscena clave;
    clave.escena.id = id;
    return tablaEscenas.search(clave);
}

const RegistroEscena* GestorEscenas::buscarRegistro(int id) const {
    RegistroEscena clave;
    clave.escena.id = id;
    return const_cast<Designar::SeparateChainingHashTable<RegistroEscena, CmpEscena>&>(tablaEscenas).search(clave);
}

bool GestorEscenas::cumpleCondicion(const std::string& condicion,
                                    const std::vector<std::string>& inventarioClaves) {
    if (condicion.empty()) {
        return true;
    }
    return std::find(inventarioClaves.begin(), inventarioClaves.end(), condicion) != inventarioClaves.end();
}

void GestorEscenas::agregarNodo(const NodoNarrativo& nodo) {
    RegistroEscena* existente = buscarRegistro(nodo.id);
    if (existente != nullptr) {
        existente->escena = nodo;
        return;
    }
    tablaEscenas.insert(RegistroEscena(nodo));
}

void GestorEscenas::agregarArista(int idOrigen, const AristaDecision& arista) {
    RegistroEscena* origen = buscarRegistro(idOrigen);
    if (origen != nullptr) {
        origen->decisiones.append(arista);
    }
}

NodoNarrativo* GestorEscenas::getNodo(int id) {
    RegistroEscena* reg = buscarRegistro(id);
    return (reg != nullptr) ? &reg->escena : nullptr;
}

std::vector<AristaDecision> GestorEscenas::getDecisiones(
    int id, const std::vector<std::string>& inventarioClaves) const {
    std::vector<AristaDecision> disponibles;
    const RegistroEscena* reg = buscarRegistro(id);
    if (reg == nullptr) {
        return disponibles;
    }

    reg->decisiones.for_each([&](const AristaDecision& decision) {
        if (cumpleCondicion(decision.condicion, inventarioClaves)) {
            disponibles.push_back(decision);
        }
    });

    return disponibles;
}

bool GestorEscenas::moverA(int idDestino, const std::vector<std::string>& inventarioClaves) {
    RegistroEscena* actual = buscarRegistro(escenaActual);
    RegistroEscena* destino = buscarRegistro(idDestino);
    if (actual == nullptr || destino == nullptr) {
        return false;
    }

    bool transicionValida = false;
    actual->decisiones.for_each([&](const AristaDecision& decision) {
        if (decision.idDestino == idDestino &&
            cumpleCondicion(decision.condicion, inventarioClaves)) {
            transicionValida = true;
        }
    });

    if (!transicionValida || !cumpleCondicion(destino->escena.condicion, inventarioClaves)) {
        return false;
    }

    destino->escena.desbloqueado = true;
    escenaActual = idDestino;
    return true;
}

void GestorEscenas::desbloquearNodo(int id) {
    RegistroEscena* reg = buscarRegistro(id);
    if (reg != nullptr) {
        reg->escena.desbloqueado = true;
    }
}

void GestorEscenas::mostrarEscenas() const {
    std::cout << "\n--- Tabla Hash de Escenas (DeSiGNAR) ---\n";
    std::cout << "Escena actual ID: " << escenaActual << "\n";
    const_cast<Designar::SeparateChainingHashTable<RegistroEscena, CmpEscena>&>(tablaEscenas).for_each(
        [](const RegistroEscena& reg) {
            std::cout << "  [" << reg.escena.id << "] " << reg.escena.titulo
                      << (reg.escena.desbloqueado ? " (Desbloqueada)" : " (Bloqueada)") << "\n";
        });
}
