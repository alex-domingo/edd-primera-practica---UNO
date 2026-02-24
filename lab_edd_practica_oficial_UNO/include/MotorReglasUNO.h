#ifndef LAB_EDD_PRACTICA_OFICIAL_UNO_MOTORREGLASUNO_H
#define LAB_EDD_PRACTICA_OFICIAL_UNO_MOTORREGLASUNO_H

#include "Carta.h"
#include "ReglasUNO.h"

class Partida;
class Jugador;

class MotorReglasUNO {
private:
    const ReglasUNO &reglas;

    // Estado de stacking
    int acumuladoRobo;
    Carta::Tipo tipoAcumulado; // ROBA2 o ROBA4 cuando acumuladoRobo > 0

public:
    explicit MotorReglasUNO(const ReglasUNO &reglas);

    // Se llama al inicio del turno: resuelve acumulados si existen
    // Retorna true si el turno se consumió (robó y perdió turno, o apiló y pasó)
    bool procesarInicioTurno(Partida &partida);

    // Se llama cuando el jugador elige "R"
    // Retorna true si el turno ya se consumió (en modo A siempre, en modo B depende)
    bool procesarRobo(Partida &partida, Jugador *jugador);

    // Regla "no ganar con negra"
    bool permiteJugarComoUltima(Jugador *jugador, Carta *carta) const;

    // Aplicar efecto básico + enganchar stacking para ROBA2/ROBA4
    void aplicarEfectoDeCarta(Partida &partida, Carta *cartaJugada);

private:
    bool jugadorTieneCartaDeTipo(Jugador *jugador, Carta::Tipo tipoBuscado) const;
};

#endif //LAB_EDD_PRACTICA_OFICIAL_UNO_MOTORREGLASUNO_H
