#include "Mano.h"
#include <iostream>

Mano::Mano() : cabeza(nullptr), cantidad(0) {
}

Mano::~Mano() {
    vaciarSoloNodos();
}

void Mano::insertarAlFinal(Carta *carta) {
    NodoMano *nuevoNodo = new NodoMano();
    nuevoNodo->carta = carta;
    nuevoNodo->siguiente = nullptr;

    if (cabeza == nullptr) {
        cabeza = nuevoNodo;
    } else {
        NodoMano *actual = cabeza;
        while (actual->siguiente != nullptr) {
            actual = actual->siguiente;
        }
        actual->siguiente = nuevoNodo;
    }
    cantidad++;
}

Carta *Mano::obtenerPorIndice(int indice) const {
    if (indice < 0 || indice >= cantidad) return nullptr;

    NodoMano *actual = cabeza;
    int indiceActual = 0;
    while (actual != nullptr && indiceActual < indice) {
        actual = actual->siguiente;
        indiceActual++;
    }
    return (actual != nullptr) ? actual->carta : nullptr;
}

Carta *Mano::removerPorIndice(int indice) {
    if (indice < 0 || indice >= cantidad) return nullptr;

    NodoMano *nodoAEliminar = nullptr;
    Carta *cartaExtraida = nullptr;

    if (indice == 0) {
        nodoAEliminar = cabeza;
        cabeza = cabeza->siguiente;
    } else {
        NodoMano *anterior = cabeza;
        int indiceActual = 0;
        while (anterior != nullptr && indiceActual < (indice - 1)) {
            anterior = anterior->siguiente;
            indiceActual++;
        }
        nodoAEliminar = anterior->siguiente;
        anterior->siguiente = nodoAEliminar->siguiente;
    }

    cartaExtraida = nodoAEliminar->carta;
    delete nodoAEliminar;
    cantidad--;

    return cartaExtraida;
}

int Mano::size() const {
    return cantidad;
}

void Mano::imprimir() const {
    NodoMano *actual = cabeza;
    int indice = 0;
    while (actual != nullptr) {
        std::cout << indice << ": ";
        actual->carta->imprimir();
        std::cout << "\n";
        actual = actual->siguiente;
        indice++;
    }
}

void Mano::vaciarSoloNodos() {
    while (cabeza != nullptr) {
        NodoMano *nodoAEliminar = cabeza;
        cabeza = cabeza->siguiente;
        delete nodoAEliminar;
    }
    cantidad = 0;
}
