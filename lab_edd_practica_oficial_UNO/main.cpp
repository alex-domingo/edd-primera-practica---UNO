#include <iostream>
#include "Partida.h"

int main() {
    std::cout << "===== UNO (Consola - EDD) =====\n\n";

    Partida partida;

    partida.configurarJugadores();
    partida.construirMazoBasicoYBarajar();
    partida.repartir(7);
    partida.mostrarEstadoInicial();

    std::cout << "Fin de prueba de estructuras. Luego metemos reglas completas.\n";
    return 0;
}
