#include "MesaJugadores.h"

MesaJugadores::MesaJugadores() : nodoActual(nullptr), cantidad(0) {
}

MesaJugadores::~MesaJugadores() {
    vaciarSoloNodos();
}

void MesaJugadores::agregarJugador(Jugador *jugador) {
    NodoJugador *nuevoNodo = new NodoJugador();
    nuevoNodo->jugador = jugador;

    if (nodoActual == nullptr) {
        nuevoNodo->siguiente = nuevoNodo;
        nuevoNodo->anterior = nuevoNodo;
        nodoActual = nuevoNodo;
    } else {
        NodoJugador *ultimo = nodoActual->anterior;

        ultimo->siguiente = nuevoNodo;
        nuevoNodo->anterior = ultimo;

        nuevoNodo->siguiente = nodoActual;
        nodoActual->anterior = nuevoNodo;
    }

    cantidad++;
}

Jugador *MesaJugadores::getJugadorActual() const {
    if (nodoActual == nullptr) return nullptr;
    return nodoActual->jugador;
}

void MesaJugadores::avanzarTurno(int direccionJuego) {
    if (nodoActual == nullptr) return;

    if (direccionJuego >= 0) {
        nodoActual = nodoActual->siguiente;
    } else {
        nodoActual = nodoActual->anterior;
    }
}

int MesaJugadores::size() const {
    return cantidad;
}

void MesaJugadores::vaciarSoloNodos() {
    if (nodoActual == nullptr) {
        cantidad = 0;
        return;
    }

    NodoJugador *inicio = nodoActual;
    NodoJugador *actual = inicio->siguiente;

    while (actual != inicio) {
        NodoJugador *nodoAEliminar = actual;
        actual = actual->siguiente;
        delete nodoAEliminar;
    }

    delete inicio;
    nodoActual = nullptr;
    cantidad = 0;
}
