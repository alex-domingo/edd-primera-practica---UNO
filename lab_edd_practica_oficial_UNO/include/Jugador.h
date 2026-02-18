#ifndef LAB_EDD_PRACTICA_OFICIAL_UNO_JUGADOR_H
#define LAB_EDD_PRACTICA_OFICIAL_UNO_JUGADOR_H

#include <string>
#include "Mano.h"
#include "PilaCartas.h"

class Jugador {
private:
    std::string nombre;
    Mano mano;
    bool dijoUNOEnTurno;

public:
    explicit Jugador(const std::string& nombre);

    const std::string& getNombre() const;
    Mano& getMano();
    int cantidadCartas() const;

    void setDijoUNOEnTurno(bool valor);
    bool getDijoUNOEnTurno() const;

    void robarCartas(PilaCartas& mazo, int cantidadARobar);
};

#endif //LAB_EDD_PRACTICA_OFICIAL_UNO_JUGADOR_H