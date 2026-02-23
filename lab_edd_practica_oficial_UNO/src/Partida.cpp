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

void Partida::jugar() {
    std::cout << "===== UNO (Consola - EDD) =====\n\n";

    configurarJugadores();
    if (mesaJugadores.size() < 2) return;

    construirMazoBasicoYBarajar();
    repartir(7);
    iniciarCartaEnMesa();

    bool juegoTerminado = false;

    while (!juegoTerminado) {
        Jugador *jugadorActual = mesaJugadores.getJugadorActual();
        if (jugadorActual == nullptr) return;

        std::cout << "\n---------------------------------\n";
        std::cout << "Turno de: " << jugadorActual->getNombre() << "\n";
        mostrarCartaEnMesa();
        std::cout << "Color actual: " << Carta(colorActual, Carta::NUMERO, 0).colorComoTexto() << "\n";
        std::cout << "\nMano:\n";
        jugadorActual->getMano().imprimir();

        ejecutarTurnoJugador();

        if (jugadorActual->cantidadCartas() == 0) {
            std::cout << "\n*** GANADOR: " << jugadorActual->getNombre() << " ***\n";
            juegoTerminado = true;
            continue;
        }

        mesaJugadores.avanzarTurno(direccionJuego);
    }
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
    // mazo base para tener juego jugable de momento:
    // 4 colores * 10 números = 40 cartas.
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

    for (int i = 0; i < totalCartasPool; i++) {
        mazoRobar.push(poolCartas[i]);
    }
}

void Partida::barajar(Carta **arregloCartas, int cantidadCartas) {
    for (int indice = cantidadCartas - 1; indice > 0; indice--) {
        int indiceAleatorio = std::rand() % (indice + 1);
        Carta *temporal = arregloCartas[indice];
        arregloCartas[indice] = arregloCartas[indiceAleatorio];
        arregloCartas[indiceAleatorio] = temporal;
    }
}

void Partida::repartir(int cartasPorJugador) {
    for (int ronda = 0; ronda < cartasPorJugador; ronda++) {
        for (int i = 0; i < mesaJugadores.size(); i++) {
            Jugador *jugador = mesaJugadores.getJugadorActual();
            if (jugador != nullptr) {
                jugador->robarCartas(mazoRobar, 1);
            }
            mesaJugadores.avanzarTurno(direccionJuego);
        }
    }
}

void Partida::iniciarCartaEnMesa() {
    Carta *cartaInicial = mazoRobar.pop();
    if (cartaInicial == nullptr) {
        std::cout << "No hay cartas para iniciar.\n";
        return;
    }

    pilaDescarte.push(cartaInicial);

    // si no es negra, el color actual es el de la carta
    if (!cartaInicial->esNegra()) {
        colorActual = cartaInicial->getColor();
    } else {
        colorActual = Carta::ROJO; // por defecto, pero en mazo base no hay negras
    }

    std::cout << "\nCarta inicial en mesa: ";
    cartaInicial->imprimir();
    std::cout << "\n";
}

void Partida::mostrarCartaEnMesa() const {
    Carta *cartaEnMesa = pilaDescarte.peek();
    std::cout << "Carta en mesa: ";
    if (cartaEnMesa == nullptr) {
        std::cout << "(ninguna)";
    } else {
        cartaEnMesa->imprimir();
    }
    std::cout << "\n";
}

Carta::Color Partida::solicitarColorAlJugador() {
    int opcionColor = 0;
    std::cout << "Elige color (1=ROJO, 2=AMARILLO, 3=AZUL, 4=VERDE): ";
    std::cin >> opcionColor;

    switch (opcionColor) {
        case 1: return Carta::ROJO;
        case 2: return Carta::AMARILLO;
        case 3: return Carta::AZUL;
        case 4: return Carta::VERDE;
        default:
            std::cout << "Opcion invalida. Se usara ROJO.\n";
            return Carta::ROJO;
    }
}

void Partida::ejecutarTurnoJugador() {
    Jugador *jugador = mesaJugadores.getJugadorActual();
    if (jugador == nullptr) return;

    Carta *cartaEnMesa = pilaDescarte.peek();

    std::cout << "\nAccion: (J)ugar por indice, (R)obar 1: ";
    char opcionAccion;
    std::cin >> opcionAccion;

    if (opcionAccion == 'R' || opcionAccion == 'r') {
        Carta *cartaRobada = mazoRobar.pop();
        if (cartaRobada == nullptr) {
            std::cout << "Mazo vacio (reponer vendra despues).\n";
            return;
        }
        jugador->getMano().insertarAlFinal(cartaRobada);
        std::cout << "Robaste: ";
        cartaRobada->imprimir();
        std::cout << "\n";
        return;
    }

    if (opcionAccion == 'J' || opcionAccion == 'j') {
        int indiceElegido = -1;
        std::cout << "Ingrese indice de carta a jugar: ";
        std::cin >> indiceElegido;

        Carta *cartaElegida = jugador->getMano().obtenerPorIndice(indiceElegido);
        if (cartaElegida == nullptr) {
            std::cout << "Indice invalido.\n";
            return;
        }

        if (!cartaElegida->esJugableSobre(cartaEnMesa, colorActual)) {
            std::cout << "Esa carta NO es jugable sobre la carta en mesa.\n";
            return;
        }

        // removemos de la mano y ponemos en descarte
        Carta *cartaJugada = jugador->getMano().removerPorIndice(indiceElegido);
        pilaDescarte.push(cartaJugada);

        std::cout << "Jugaste: ";
        cartaJugada->imprimir();
        std::cout << "\n";

        // actualizamos color actual
        if (cartaJugada->esComodin()) {
            colorActual = solicitarColorAlJugador();
        } else {
            colorActual = cartaJugada->getColor();
        }

        return;
    }

    std::cout << "Opcion invalida. Pierdes el turno.\n";
}

void Partida::liberarMemoriaCentralizada() {
    if (poolJugadores != nullptr) {
        for (int i = 0; i < totalJugadoresPool; i++) {
            delete poolJugadores[i];
        }
        delete[] poolJugadores;
        poolJugadores = nullptr;
        totalJugadoresPool = 0;
    }

    if (poolCartas != nullptr) {
        for (int i = 0; i < totalCartasPool; i++) {
            delete poolCartas[i];
        }
        delete[] poolCartas;
        poolCartas = nullptr;
        totalCartasPool = 0;
    }
}
