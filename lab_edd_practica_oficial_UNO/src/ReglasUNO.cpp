#include "ReglasUNO.h"

ReglasUNO::ReglasUNO()
    : stackingActivadoFlag(false),
      modoRoboFlag('A'),
      ganarConNegraFlag(true) {
}

void ReglasUNO::configurarDesdeConsola() {
    char opcion;

    std::cout << "\n--- CONFIGURACION DE REGLAS ---\n";

    std::cout << "Stacking (+2 apila +2, +4 apila +4)? (S/N): ";
    std::cin >> opcion;
    stackingActivadoFlag = (opcion == 'S' || opcion == 's');

    std::cout << "Modo de robo (A = roba 1 y pasa, B = roba hasta poder jugar): ";
    std::cin >> opcion;
    modoRoboFlag = (opcion == 'B' || opcion == 'b') ? 'B' : 'A';

    std::cout << "Permitir ganar con carta negra (COMODIN/ROBA4)? (S/N): ";
    std::cin >> opcion;
    ganarConNegraFlag = (opcion == 'S' || opcion == 's');

    std::cout << "\n";
}

void ReglasUNO::imprimirResumen() const {
    std::cout << "Reglas activas -> Stacking: " << (stackingActivadoFlag ? "SI" : "NO")
            << " | ModoRobo: " << modoRoboFlag
            << " | GanarConNegra: " << (ganarConNegraFlag ? "SI" : "NO")
            << "\n\n";
}

bool ReglasUNO::stackingActivado() const { return stackingActivadoFlag; }
char ReglasUNO::modoRobo() const { return modoRoboFlag; }
bool ReglasUNO::ganarConNegra() const { return ganarConNegraFlag; }

void ReglasUNO::setStackingActivado(bool valor) { stackingActivadoFlag = valor; }
void ReglasUNO::setModoRobo(char valor) { modoRoboFlag = valor; }
void ReglasUNO::setGanarConNegra(bool valor) { ganarConNegraFlag = valor; }
