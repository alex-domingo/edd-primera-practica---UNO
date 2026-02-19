#include "Partida.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

Partida::Partida()
    : direccionJuego(1),
      colorActual(Carta::ROJO),
      poolCartas(nullptr),
      totalCartasPool(0),
      poolJugadores(nullptr),
      totalJugadoresPool(0) {
}

Partida::~Partida() {
    liberarMemoriaCentralizada();
}

void Partida::configurarJugadores() {
    int cantidadJugadores = 0;
    std::cout << "Ingrese cantidad de jugadores: ";
    std::cin >> cantidadJugadores;

    if (cantidadJugadores <= 1) {
        std::cout << "Se necesitan al menos 2 jugadores.\n";
        return;
    }

    totalJugadoresPool = cantidadJugadores;
    poolJugadores = new Jugador *[totalJugadoresPool];

    for (int i = 0; i < cantidadJugadores; i++) {
        std::string nombreJugador;
        std::cout << "Nombre del jugador " << (i + 1) << ": ";
        std::cin >> nombreJugador;

        Jugador *jugadorNuevo = new Jugador(nombreJugador);
        poolJugadores[i] = jugadorNuevo;
        mesaJugadores.agregarJugador(jugadorNuevo);
    }
}

void Partida::construirMazoBasicoYBarajar() {
    // Base simple para probar (luego lo hacemos 108 real + comodines + acciones):
    // 4 colores * 10 numeros = 40 cartas
    // Esto es SOLO para validar estructuras desde ya
    totalCartasPool = 40;
    poolCartas = new Carta *[totalCartasPool];

    int indiceCarta = 0;
    for (int color = 0; color < 4; color++) {
        for (int valor = 0; valor <= 9; valor++) {
            poolCartas[indiceCarta] = new Carta((Carta::Color) color, Carta::NUMERO, valor);
            indiceCarta++;
        }
    }

    std::srand((unsigned) std::time(nullptr));
    barajar(poolCartas, totalCartasPool);

    // Apilamos en mazoRobar
    for (int i = 0; i < totalCartasPool; i++) {
        mazoRobar.push(poolCartas[i]);
    }
}

void Partida::barajar(Carta **arregloCartas, int cantidadCartas) {
    // Fisher-Yates O(N)
    for (int i = cantidadCartas - 1; i > 0; i--) {
        int indiceAleatorio = std::rand() % (i + 1);
        Carta *temporal = arregloCartas[i];
        arregloCartas[i] = arregloCartas[indiceAleatorio];
        arregloCartas[indiceAleatorio] = temporal;
    }
}

void Partida::repartir(int cartasPorJugador) {
    for (int r = 0; r < cartasPorJugador; r++) {
        for (int j = 0; j < mesaJugadores.size(); j++) {
            Jugador *jugadorActual = mesaJugadores.getJugadorActual();
            if (jugadorActual != nullptr) {
                jugadorActual->robarCartas(mazoRobar, 1);
            }
            mesaJugadores.avanzarTurno(direccionJuego);
        }
    }

    // Dejamos el turno en el primer jugador (por simplicidad)
}

void Partida::mostrarEstadoInicial() {
    std::cout << "\n--- ESTADO INICIAL ---\n";
    std::cout << "Jugadores: " << mesaJugadores.size() << "\n";
    std::cout << "Cartas en mazo: " << mazoRobar.size() << "\n\n";

    for (int i = 0; i < mesaJugadores.size(); i++) {
        Jugador *jugador = mesaJugadores.getJugadorActual();
        std::cout << "Mano de " << jugador->getNombre() << " (" << jugador->cantidadCartas() << "):\n";
        jugador->getMano().imprimir();
        std::cout << "\n";
        mesaJugadores.avanzarTurno(direccionJuego);
    }
}

void Partida::liberarMemoriaCentralizada() {
    // Liberar jugadores
    if (poolJugadores != nullptr) {
        for (int i = 0; i < totalJugadoresPool; i++) {
            delete poolJugadores[i];
        }
        delete[] poolJugadores;
        poolJugadores = nullptr;
        totalJugadoresPool = 0;
    }

    // Liberar cartas
    if (poolCartas != nullptr) {
        for (int i = 0; i < totalCartasPool; i++) {
            delete poolCartas[i];
        }
        delete[] poolCartas;
        poolCartas = nullptr;
        totalCartasPool = 0;
    }
}
