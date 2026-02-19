#include "Jugador.h"

Jugador::Jugador(const std::string &nombre)
    : nombre(nombre), dijoUNOEnTurno(false) {
}

const std::string &Jugador::getNombre() const { return nombre; }

Mano &Jugador::getMano() { return mano; }

int Jugador::cantidadCartas() const {
    return mano.size();
}

void Jugador::setDijoUNOEnTurno(bool valor) {
    dijoUNOEnTurno = valor;
}

bool Jugador::getDijoUNOEnTurno() const {
    return dijoUNOEnTurno;
}

void Jugador::robarCartas(PilaCartas &mazo, int cantidadARobar) {
    for (int i = 0; i < cantidadARobar; i++) {
        Carta *cartaRobada = mazo.pop();
        if (cartaRobada == nullptr) {
            return;
        }
        mano.insertarAlFinal(cartaRobada);
    }
}
