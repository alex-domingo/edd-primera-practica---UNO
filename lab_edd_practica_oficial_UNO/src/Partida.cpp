#include "Partida.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "MotorReglasUNO.h"

Partida::Partida()
    : direccionJuego(1),
      colorActual(Carta::ROJO),
      poolCartas(nullptr),
      totalCartasPool(0),
      poolJugadores(nullptr),
      totalJugadoresPool(0),
      motorReglas(nullptr) {
    motorReglas = new MotorReglasUNO(reglas);
}

Partida::~Partida() {
    delete motorReglas;
    motorReglas = nullptr;
    liberarMemoriaCentralizada();
}

void Partida::jugar() {
    std::cout << "===== UNO (Consola - EDD) =====\n\n";

    configurarJugadores();
    if (mesaJugadores.size() < 2) return;

    reglas.configurarDesdeConsola();
    reglas.imprimirResumen();

    construirMazoOficialYBarajar(mesaJugadores.size());
    repartir(7);
    iniciarCartaEnMesaSoloNumero();

    bool juegoTerminado = false;

    while (!juegoTerminado) {
        Jugador *jugadorActual = mesaJugadores.getJugadorActual();
        if (jugadorActual == nullptr) return;

        // ✅ NUEVO: resolver acumulados/stacking ANTES del turno normal
        if (motorReglas != nullptr && motorReglas->procesarInicioTurno(*this)) {
            // El motor ya consumió el turno (robó y perdió, o apiló y pasó)
            continue;
        }

        std::cout << "\n---------------------------------\n";
        std::cout << "Turno de: " << jugadorActual->getNombre() << "\n";
        mostrarCartaEnMesa();
        std::cout << "Color actual: " << Carta(colorActual, Carta::NUMERO, 0).colorComoTexto() << "\n";
        mostrarResumenMazos();

        std::cout << "\nMano:\n";
        jugadorActual->getMano().imprimir();

        ejecutarTurnoJugador();

        if (jugadorActual->cantidadCartas() == 0) {
            std::cout << "\n/// GANADOR: " << jugadorActual->getNombre() << " ///\n";
            juegoTerminado = true;
            continue;
        }

        // Avanzar turno normal (si hubo SALTO, el motor ya avanzó internamente)
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

int Partida::calcularCantidadDecks(int cantidadJugadores) const {
    int cantidadDecks = ((cantidadJugadores - 1) / 6) + 1;
    return cantidadDecks;
}

void Partida::construirMazoOficialYBarajar(int cantidadJugadores) {
    int cantidadDecks = calcularCantidadDecks(cantidadJugadores);
    int cartasPorDeck = 108;
    totalCartasPool = cantidadDecks * cartasPorDeck;

    poolCartas = new Carta *[totalCartasPool];

    int indiceCarta = 0;

    for (int deck = 0; deck < cantidadDecks; deck++) {
        for (int color = 0; color < 4; color++) {
            poolCartas[indiceCarta++] = new Carta((Carta::Color) color, Carta::NUMERO, 0);

            for (int valor = 1; valor <= 9; valor++) {
                poolCartas[indiceCarta++] = new Carta((Carta::Color) color, Carta::NUMERO, valor);
                poolCartas[indiceCarta++] = new Carta((Carta::Color) color, Carta::NUMERO, valor);
            }

            for (int copia = 0; copia < 2; copia++) {
                poolCartas[indiceCarta++] = new Carta((Carta::Color) color, Carta::SALTO, -1);
                poolCartas[indiceCarta++] = new Carta((Carta::Color) color, Carta::REVERSA, -1);
                poolCartas[indiceCarta++] = new Carta((Carta::Color) color, Carta::ROBA2, -1);
            }
        }

        for (int i = 0; i < 4; i++) {
            poolCartas[indiceCarta++] = new Carta(Carta::NEGRO, Carta::COMODIN, -1);
            poolCartas[indiceCarta++] = new Carta(Carta::NEGRO, Carta::ROBA4, -1);
        }
    }

    if (indiceCarta != totalCartasPool) {
        std::cout << "ADVERTENCIA: total cartas construidas (" << indiceCarta
                << ") != esperado (" << totalCartasPool << ")\n";
        totalCartasPool = indiceCarta;
    }

    std::srand((unsigned) std::time(nullptr));
    barajar(poolCartas, totalCartasPool);

    for (int i = 0; i < totalCartasPool; i++) {
        mazoRobar.push(poolCartas[i]);
    }

    std::cout << "\nMazo oficial construido: " << totalCartasPool
            << " cartas (" << cantidadDecks << " deck(s)).\n";
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
                robarCartasAJugador(jugador, 1);
            }
            mesaJugadores.avanzarTurno(direccionJuego);
        }
    }
}

void Partida::iniciarCartaEnMesaSoloNumero() {
    Carta *cartaInicial = nullptr;

    while (true) {
        reponerMazoSiVacio();
        cartaInicial = mazoRobar.pop();
        if (cartaInicial == nullptr) {
            std::cout << "No hay cartas para iniciar.\n";
            return;
        }

        if (cartaInicial->getTipo() == Carta::NUMERO && !cartaInicial->esNegra()) {
            break;
        }

        mazoRobar.push(cartaInicial);
    }

    pilaDescarte.push(cartaInicial);
    colorActual = cartaInicial->getColor();

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

void Partida::mostrarResumenMazos() const {
    std::cout << "Mazo: " << mazoRobar.size() << " | Descarte: " << pilaDescarte.size() << "\n";

    std::cout << "Modo robo: " << reglas.modoRobo()
            << " | Stacking: " << (reglas.stackingActivado() ? "SI" : "NO")
            << " | Ganar con negra: " << (reglas.ganarConNegra() ? "SI" : "NO")
            << "\n";
}

void Partida::reponerMazoSiVacio() {
    if (!mazoRobar.estaVacia()) return;

    if (pilaDescarte.size() <= 1) {
        return;
    }

    Carta *cartaEnMesa = pilaDescarte.pop();

    int cantidadParaReponer = pilaDescarte.size();
    Carta **arregloTemporal = new Carta *[cantidadParaReponer];

    for (int i = 0; i < cantidadParaReponer; i++) {
        arregloTemporal[i] = pilaDescarte.pop();
    }

    barajar(arregloTemporal, cantidadParaReponer);

    for (int i = 0; i < cantidadParaReponer; i++) {
        mazoRobar.push(arregloTemporal[i]);
    }

    delete[] arregloTemporal;

    pilaDescarte.push(cartaEnMesa);

    std::cout << "(Repuesto mazo con el descarte)\n";
}

void Partida::robarCartasAJugador(Jugador *jugador, int cantidadARobar) {
    if (jugador == nullptr) return;

    for (int i = 0; i < cantidadARobar; i++) {
        reponerMazoSiVacio();
        Carta *cartaRobada = mazoRobar.pop();
        if (cartaRobada == nullptr) {
            std::cout << "No hay cartas para robar.\n";
            return;
        }
        jugador->getMano().insertarAlFinal(cartaRobada);
    }
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

// ⚠️ Esta función queda SIN USO si ya usas MotorReglasUNO.
// Puedes eliminarla luego junto con su declaración en Partida.h si ya no la necesitas.
void Partida::aplicarEfectoBasicoDeCarta(Carta *cartaJugada) {
    if (cartaJugada == nullptr) return;

    switch (cartaJugada->getTipo()) {
        case Carta::REVERSA:
            direccionJuego *= -1;
            std::cout << "(Efecto) REVERSA: cambia direccion.\n";
            break;

        case Carta::SALTO:
            std::cout << "(Efecto) SALTO: se salta al siguiente jugador.\n";
            mesaJugadores.avanzarTurno(direccionJuego);
            break;

        case Carta::ROBA2: {
            std::cout << "(Efecto) ROBA2: siguiente jugador roba 2 y pierde turno.\n";
            mesaJugadores.avanzarTurno(direccionJuego);
            Jugador *jugadorAfectado = mesaJugadores.getJugadorActual();
            robarCartasAJugador(jugadorAfectado, 2);
            mesaJugadores.avanzarTurno(direccionJuego);
            break;
        }

        case Carta::ROBA4: {
            std::cout << "(Efecto) ROBA4: siguiente jugador roba 4 y pierde turno.\n";
            mesaJugadores.avanzarTurno(direccionJuego);
            Jugador *jugadorAfectado = mesaJugadores.getJugadorActual();
            robarCartasAJugador(jugadorAfectado, 4);
            mesaJugadores.avanzarTurno(direccionJuego);
            break;
        }

        default:
            break;
    }
}

void Partida::ejecutarTurnoJugador() {
    Jugador *jugador = mesaJugadores.getJugadorActual();
    if (jugador == nullptr) return;

    Carta *cartaEnMesa = pilaDescarte.peek();

    std::cout << "\nAccion: (J)ugar por indice, (R)obar: ";
    char opcionAccion;
    std::cin >> opcionAccion;

    // ✅ NUEVO: robo A/B (y auto-play en modo B) lo maneja el motor
    if (opcionAccion == 'R' || opcionAccion == 'r') {
        if (motorReglas != nullptr) {
            motorReglas->procesarRobo(*this, jugador);
        } else {
            // fallback (no debería pasar)
            robarCartasAJugador(jugador, 1);
        }
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

        // ✅ NUEVO: regla no ganar con negra (si aplica)
        if (motorReglas != nullptr && !motorReglas->permiteJugarComoUltima(jugador, cartaElegida)) {
            std::cout << "No puedes ganar con carta negra segun las reglas. Debes jugar otra o robar.\n";
            return;
        }

        if (!cartaElegida->esJugableSobre(cartaEnMesa, colorActual)) {
            std::cout << "Esa carta NO es jugable sobre la carta en mesa.\n";
            return;
        }

        Carta *cartaJugada = jugador->getMano().removerPorIndice(indiceElegido);
        pilaDescarte.push(cartaJugada);

        std::cout << "Jugaste: ";
        cartaJugada->imprimir();
        std::cout << "\n";

        if (cartaJugada->esComodin()) {
            colorActual = solicitarColorAlJugador();
        } else {
            colorActual = cartaJugada->getColor();
        }

        // ✅ NUEVO: efectos + stacking se manejan en el motor
        if (motorReglas != nullptr) {
            motorReglas->aplicarEfectoDeCarta(*this, cartaJugada);
        } else {
            aplicarEfectoBasicoDeCarta(cartaJugada);
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
