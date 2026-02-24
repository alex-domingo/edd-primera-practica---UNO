#ifndef LAB_EDD_PRACTICA_OFICIAL_UNO_PARTIDA_H
#define LAB_EDD_PRACTICA_OFICIAL_UNO_PARTIDA_H

#include "MesaJugadores.h"
#include "PilaCartas.h"
#include "ReglasUNO.h"
#include "EstadoStacking.h"

class MotorReglasUNO;

class Partida {
private:
    friend class MotorReglasUNO;

    MesaJugadores mesaJugadores;
    PilaCartas mazoRobar;
    PilaCartas pilaDescarte;

    int direccionJuego; // +1 / -1
    Carta::Color colorActual;

    // pools para liberar memoria al final
    Carta **poolCartas;
    int totalCartasPool;

    Jugador **poolJugadores;
    int totalJugadoresPool;

    // configuración y estado de reglas
    ReglasUNO reglas;
    EstadoStacking estadoStacking;

    // motor que aplica reglas y efectos usando "reglas" + "estadoStacking"
    MotorReglasUNO *motorReglas;

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

    // robos con reposición
    void robarCartasAJugador(Jugador *jugador, int cantidadARobar);

    void reponerMazoSiVacio();

    // efectos básicos creados inicialmente para pruebas
    void aplicarEfectoBasicoDeCarta(Carta *cartaJugada);

    Carta::Color solicitarColorAlJugador();

    void liberarMemoriaCentralizada();

    void barajar(Carta **arregloCartas, int cantidadCartas);

    void mostrarCartaEnMesa() const;

    void mostrarResumenMazos() const;

    int calcularCantidadDecks(int cantidadJugadores) const;
};

#endif //LAB_EDD_PRACTICA_OFICIAL_UNO_PARTIDA_H
