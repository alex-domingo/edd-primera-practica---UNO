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

    // Guardamos punteros para poder liberar memoria al final
    Carta** poolCartas;
    int totalCartasPool;

    Jugador** poolJugadores;
    int totalJugadoresPool;

public:
    Partida();
    ~Partida();

    void configurarJugadores();
    void construirMazoBasicoYBarajar(); // de base (pero puede expandirse)
    void repartir(int cartasPorJugador);
    void mostrarEstadoInicial();

private:
    void liberarMemoriaCentralizada();
    void barajar(Carta** arregloCartas, int cantidadCartas);
};

#endif //LAB_EDD_PRACTICA_OFICIAL_UNO_PARTIDA_H