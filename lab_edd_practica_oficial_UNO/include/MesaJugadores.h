#ifndef LAB_EDD_PRACTICA_OFICIAL_UNO_MESAJUGADORES_H
#define LAB_EDD_PRACTICA_OFICIAL_UNO_MESAJUGADORES_H

#include "Jugador.h"

class MesaJugadores {
private:
    struct NodoJugador {
        Jugador* jugador;
        NodoJugador* siguiente;
        NodoJugador* anterior;
    };

    NodoJugador* nodoActual;
    int cantidad;

public:
    MesaJugadores();
    ~MesaJugadores();

    void agregarJugador(Jugador* jugador);
    Jugador* getJugadorActual() const;

    void avanzarTurno(int direccionJuego); // +1 horario, -1 antihorario
    int size() const;

    void vaciarSoloNodos(); // NO libera Jugador*
};

#endif //LAB_EDD_PRACTICA_OFICIAL_UNO_MESAJUGADORES_H