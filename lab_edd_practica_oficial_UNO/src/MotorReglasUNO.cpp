#include "MotorReglasUNO.h"
#include "Partida.h"
#include "Jugador.h"
#include <iostream>

MotorReglasUNO::MotorReglasUNO(const ReglasUNO &reglas)
    : reglas(reglas), acumuladoRobo(0), tipoAcumulado(Carta::NUMERO) {
}

bool MotorReglasUNO::permiteJugarComoUltima(Jugador *jugador, Carta *carta) const {
    if (jugador == nullptr || carta == nullptr) return true;

    // Si la regla dice que NO se puede ganar con negra:
    if (!reglas.ganarConNegra() && jugador->cantidadCartas() == 1 && carta->esNegra()) {
        return false;
    }
    return true;
}

bool MotorReglasUNO::jugadorTieneCartaDeTipo(Jugador *jugador, Carta::Tipo tipoBuscado) const {
    if (jugador == nullptr) return false;

    int cantidadCartas = jugador->cantidadCartas();
    for (int indice = 0; indice < cantidadCartas; indice++) {
        Carta *carta = jugador->getMano().obtenerPorIndice(indice);
        if (carta != nullptr && carta->getTipo() == tipoBuscado) {
            return true;
        }
    }
    return false;
}

bool MotorReglasUNO::procesarInicioTurno(Partida &partida) {
    if (acumuladoRobo <= 0) return false;

    Jugador *jugadorActual = partida.mesaJugadores.getJugadorActual();
    if (jugadorActual == nullptr) return false;

    std::cout << "\n*** ACUMULADO ACTIVO: " << acumuladoRobo << " carta(s) ("
            << (tipoAcumulado == Carta::ROBA2 ? "ROBA2" : "ROBA4")
            << ") para " << jugadorActual->getNombre() << " ***\n";

    // stacking apagado => roba y pierde turno
    if (!reglas.stackingActivado()) {
        std::cout << "Stacking desactivado -> Robas " << acumuladoRobo << " y pierdes turno.\n";
        partida.robarCartasAJugador(jugadorActual, acumuladoRobo);

        acumuladoRobo = 0;
        tipoAcumulado = Carta::NUMERO;

        partida.mesaJugadores.avanzarTurno(partida.direccionJuego);
        return true;
    }

    // stacking activado => solo puede responder con la misma carta
    bool tieneRespuesta = jugadorTieneCartaDeTipo(jugadorActual, tipoAcumulado);
    if (!tieneRespuesta) {
        std::cout << "No tienes carta para apilar -> Robas " << acumuladoRobo << " y pierdes turno.\n";
        partida.robarCartasAJugador(jugadorActual, acumuladoRobo);

        acumuladoRobo = 0;
        tipoAcumulado = Carta::NUMERO;

        partida.mesaJugadores.avanzarTurno(partida.direccionJuego);
        return true;
    }

    std::cout << "(1) Apilar | (2) Robar " << acumuladoRobo << " y perder turno: ";
    int opcion = 0;
    std::cin >> opcion;

    if (opcion != 1) {
        std::cout << "Robas " << acumuladoRobo << " y pierdes turno.\n";
        partida.robarCartasAJugador(jugadorActual, acumuladoRobo);

        acumuladoRobo = 0;
        tipoAcumulado = Carta::NUMERO;

        partida.mesaJugadores.avanzarTurno(partida.direccionJuego);
        return true;
    }

    std::cout << "Elige indice para apilar: ";
    int indiceElegido = -1;
    std::cin >> indiceElegido;

    Carta *cartaElegida = jugadorActual->getMano().obtenerPorIndice(indiceElegido);
    if (cartaElegida == nullptr || cartaElegida->getTipo() != tipoAcumulado) {
        std::cout << "Carta invalida para apilar. Se aplica acumulado.\n";
        partida.robarCartasAJugador(jugadorActual, acumuladoRobo);

        acumuladoRobo = 0;
        tipoAcumulado = Carta::NUMERO;

        partida.mesaJugadores.avanzarTurno(partida.direccionJuego);
        return true;
    }

    // juega la carta apilada
    Carta *cartaApilada = jugadorActual->getMano().removerPorIndice(indiceElegido);
    partida.pilaDescarte.push(cartaApilada);

    std::cout << "Apilaste: ";
    cartaApilada->imprimir();
    std::cout << "\n";

    // actualizar color actual
    if (cartaApilada->esComodin()) {
        partida.colorActual = partida.solicitarColorAlJugador();
    } else {
        partida.colorActual = cartaApilada->getColor();
    }

    // incrementar acumulado (mismo tipo)
    if (tipoAcumulado == Carta::ROBA2) acumuladoRobo += 2;
    else acumuladoRobo += 4;

    // turno termina y pasa al siguiente (para que resuelva acumulado)
    partida.mesaJugadores.avanzarTurno(partida.direccionJuego);
    return true;
}

bool MotorReglasUNO::procesarRobo(Partida &partida, Jugador *jugador) {
    if (jugador == nullptr) return true;

    // Modo A: roba 1 y termina
    if (reglas.modoRobo() == 'A') {
        partida.robarCartasAJugador(jugador, 1);

        Carta *ultimaRobada = jugador->getMano().obtenerPorIndice(jugador->cantidadCartas() - 1);
        if (ultimaRobada != nullptr) {
            std::cout << "Robaste: ";
            ultimaRobada->imprimir();
            std::cout << "\n";
        }
        return true;
    }

    // Modo B: roba hasta que la robada sea jugable y se juega automáticamente
    std::cout << "(Modo B) Robaras hasta obtener una carta jugable.\n";

    while (true) {
        partida.robarCartasAJugador(jugador, 1);

        Carta *ultimaRobada = jugador->getMano().obtenerPorIndice(jugador->cantidadCartas() - 1);
        std::cout << "Robaste: ";
        if (ultimaRobada != nullptr) ultimaRobada->imprimir();
        std::cout << "\n";

        Carta *cartaEnMesa = partida.pilaDescarte.peek();
        if (ultimaRobada != nullptr && ultimaRobada->esJugableSobre(cartaEnMesa, partida.colorActual)) {
            // Si NO puede ganar con negra, y esta carta te deja en 0 y es negra, seguimos robando
            if (!permiteJugarComoUltima(jugador, ultimaRobada)) {
                std::cout << "No puedes ganar con carta negra. Seguiras robando.\n";
                continue;
            }

            std::cout << "La carta robada es jugable. Se jugara automaticamente.\n";

            int indiceUltima = jugador->cantidadCartas() - 1;
            Carta *cartaJugada = jugador->getMano().removerPorIndice(indiceUltima);
            partida.pilaDescarte.push(cartaJugada);

            std::cout << "Jugaste: ";
            cartaJugada->imprimir();
            std::cout << "\n";

            if (cartaJugada->esComodin()) {
                partida.colorActual = partida.solicitarColorAlJugador();
            } else {
                partida.colorActual = cartaJugada->getColor();
            }

            aplicarEfectoDeCarta(partida, cartaJugada);
            return true;
        }
    }
}

void MotorReglasUNO::aplicarEfectoDeCarta(Partida &partida, Carta *cartaJugada) {
    if (cartaJugada == nullptr) return;

    switch (cartaJugada->getTipo()) {
        case Carta::REVERSA:
            partida.direccionJuego *= -1;
            std::cout << "(Efecto) REVERSA: cambia direccion.\n";
            break;

        case Carta::SALTO:
            std::cout << "(Efecto) SALTO: se salta al siguiente jugador.\n";
            partida.mesaJugadores.avanzarTurno(partida.direccionJuego);
            break;

        case Carta::ROBA2:
            std::cout << "(Efecto) ROBA2: se acumula 2 para el siguiente.\n";
            acumuladoRobo += 2;
            tipoAcumulado = Carta::ROBA2;
            break;

        case Carta::ROBA4:
            std::cout << "(Efecto) ROBA4: se acumula 4 para el siguiente.\n";
            acumuladoRobo += 4;
            tipoAcumulado = Carta::ROBA4;
            break;

        default:
            break;
    }
}
