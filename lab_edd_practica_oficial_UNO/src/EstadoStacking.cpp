#include "EstadoStacking.h"

EstadoStacking::EstadoStacking()
    : acumuladoRobo(0), tipoAcumulado(Carta::NUMERO) {
}

bool EstadoStacking::hayAcumulado() const {
    return acumuladoRobo > 0;
}

int EstadoStacking::getAcumulado() const { return acumuladoRobo; }
Carta::Tipo EstadoStacking::getTipoAcumulado() const { return tipoAcumulado; }

void EstadoStacking::iniciarAcumulado(Carta::Tipo tipo, int cantidad) {
    tipoAcumulado = tipo;
    acumuladoRobo = cantidad;
}

void EstadoStacking::sumarAcumulado(int cantidad) {
    acumuladoRobo += cantidad;
}

void EstadoStacking::reset() {
    acumuladoRobo = 0;
    tipoAcumulado = Carta::NUMERO;
}
