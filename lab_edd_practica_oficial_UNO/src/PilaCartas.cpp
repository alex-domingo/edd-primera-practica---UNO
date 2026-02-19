#include "PilaCartas.h"

PilaCartas::PilaCartas() : tope(nullptr), cantidad(0) {
}

PilaCartas::~PilaCartas() {
    vaciarSoloNodos();
}

void PilaCartas::push(Carta *carta) {
    NodoCarta *nuevoNodo = new NodoCarta();
    nuevoNodo->carta = carta;
    nuevoNodo->siguiente = tope;
    tope = nuevoNodo;
    cantidad++;
}

Carta *PilaCartas::pop() {
    if (tope == nullptr) return nullptr;

    NodoCarta *nodoAEliminar = tope;
    Carta *cartaExtraida = nodoAEliminar->carta;

    tope = tope->siguiente;
    delete nodoAEliminar;

    cantidad--;
    return cartaExtraida;
}

Carta *PilaCartas::peek() const {
    if (tope == nullptr) return nullptr;
    return tope->carta;
}

bool PilaCartas::estaVacia() const {
    return tope == nullptr;
}

int PilaCartas::size() const {
    return cantidad;
}

void PilaCartas::vaciarSoloNodos() {
    while (tope != nullptr) {
        NodoCarta *nodoAEliminar = tope;
        tope = tope->siguiente;
        delete nodoAEliminar;
    }
    cantidad = 0;
}
