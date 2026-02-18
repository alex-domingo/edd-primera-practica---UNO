#ifndef LAB_EDD_PRACTICA_OFICIAL_UNO_PILACARTAS_H
#define LAB_EDD_PRACTICA_OFICIAL_UNO_PILACARTAS_H

#include "Carta.h"

class PilaCartas {
private:
    struct NodoCarta {
        Carta* carta;
        NodoCarta* siguiente;
    };

    NodoCarta* tope;
    int cantidad;

public:
    PilaCartas();
    ~PilaCartas();

    void push(Carta* carta);
    Carta* pop();
    Carta* peek() const;

    bool estaVacia() const;
    int size() const;

    void vaciarSoloNodos(); // NO libera Carta*, solo nodos
};

#endif //LAB_EDD_PRACTICA_OFICIAL_UNO_PILACARTAS_H