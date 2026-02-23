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

    // Pools para liberar memoria al final
    Carta **poolCartas;
    int totalCartasPool;

    Jugador **poolJugadores;
    int totalJugadoresPool;

public:
    Partida();

    ~Partida();

    void jugar();

private:
    void configurarJugadores();

    void construirMazoOficialYBarajar(int cantidadJugadores);

    void repartir(int cartasPorJugador);

    void iniciarCartaEnMesaSoloNumero(); // para evitar casos raros al inicio
    void ejecutarTurnoJugador();

    // Robos con reposición
    void robarCartasAJugador(Jugador *jugador, int cantidadARobar);

    void reponerMazoSiVacio();

    // Efectos básicos de cartas (sin flags todavía)
    void aplicarEfectoBasicoDeCarta(Carta *cartaJugada);

    Carta::Color solicitarColorAlJugador();

    void liberarMemoriaCentralizada();

    void barajar(Carta **arregloCartas, int cantidadCartas);

    void mostrarCartaEnMesa() const;

    void mostrarResumenMazos() const;

    int calcularCantidadDecks(int cantidadJugadores) const;
};

#endif //LAB_EDD_PRACTICA_OFICIAL_UNO_PARTIDA_H
