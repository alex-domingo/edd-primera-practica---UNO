#ifndef LAB_EDD_PRACTICA_OFICIAL_UNO_ESTADOSTACKING_H
#define LAB_EDD_PRACTICA_OFICIAL_UNO_ESTADOSTACKING_H

#include "Carta.h"

class EstadoStacking {
private:
    int acumuladoRobo;
    Carta::Tipo tipoAcumulado;

public:
    EstadoStacking();

    bool hayAcumulado() const;

    int getAcumulado() const;

    Carta::Tipo getTipoAcumulado() const;

    void iniciarAcumulado(Carta::Tipo tipo, int cantidad);

    void sumarAcumulado(int cantidad);

    void reset();
};

#endif //LAB_EDD_PRACTICA_OFICIAL_UNO_ESTADOSTACKING_H
