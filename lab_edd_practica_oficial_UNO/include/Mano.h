#ifndef LAB_EDD_PRACTICA_OFICIAL_UNO_MANO_H
#define LAB_EDD_PRACTICA_OFICIAL_UNO_MANO_H

#include "Carta.h"

class Mano {
private:
    struct NodoMano {
        Carta* carta;
        NodoMano* siguiente;
    };

    NodoMano* cabeza;
    int cantidad;

public:
    Mano();
    ~Mano();

    void insertarAlFinal(Carta* carta);
    Carta* obtenerPorIndice(int indice) const;
    Carta* removerPorIndice(int indice);

    int size() const;
    void imprimir() const;

    void vaciarSoloNodos(); // NO libera Carta*
};

#endif //LAB_EDD_PRACTICA_OFICIAL_UNO_MANO_H