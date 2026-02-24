#ifndef LAB_EDD_PRACTICA_OFICIAL_UNO_REGLASUNO_H
#define LAB_EDD_PRACTICA_OFICIAL_UNO_REGLASUNO_H

#include <iostream>

class ReglasUNO {
private:
    bool stackingActivadoFlag;
    char modoRoboFlag; // 'A' o 'B'
    bool ganarConNegraFlag;

public:
    ReglasUNO();

    void configurarDesdeConsola();

    void imprimirResumen() const;

    bool stackingActivado() const;

    char modoRobo() const;

    bool ganarConNegra() const;

    void setStackingActivado(bool valor);

    void setModoRobo(char valor);

    void setGanarConNegra(bool valor);
};

#endif //LAB_EDD_PRACTICA_OFICIAL_UNO_REGLASUNO_H
