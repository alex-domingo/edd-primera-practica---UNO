#ifndef LAB_EDD_PRACTICA_OFICIAL_UNO_PARTIDA_H
#define LAB_EDD_PRACTICA_OFICIAL_UNO_PARTIDA_H

#include "MesaJugadores.h"
#include "PilaCartas.h"

class Partida {
private:
    MesaJugadores mesaJugadores;
    PilaCartas mazoRobar;
    PilaCartas pilaDescarte;

    int direccionJuego; // +1 / -1
    Carta::Color colorActual;

    Carta **poolCartas;
    int totalCartasPool;

    Jugador **poolJugadores;
    int totalJugadoresPool;

public:
    Partida();

    ~Partida();

    void jugar(); // flujo completo jugable

private:
    void configurarJugadores();

    void construirMazoBasicoYBarajar();

    void repartir(int cartasPorJugador);

    void iniciarCartaEnMesa();

    void ejecutarTurnoJugador();

    Carta::Color solicitarColorAlJugador();

    void liberarMemoriaCentralizada();

    void barajar(Carta **arregloCartas, int cantidadCartas);

    void mostrarCartaEnMesa() const;
};

#endif //LAB_EDD_PRACTICA_OFICIAL_UNO_PARTIDA_H
