#ifndef LAB_EDD_PRACTICA_OFICIAL_UNO_CARTA_H
#define LAB_EDD_PRACTICA_OFICIAL_UNO_CARTA_H

#include <string>

class Carta {
public:
    enum Color {
        ROJO = 0,
        AMARILLO = 1,
        AZUL = 2,
        VERDE = 3,
        NEGRO = 4
    };

    enum Tipo {
        NUMERO = 0,
        SALTO = 1,
        REVERSA = 2,
        ROBA2 = 3,
        COMODIN = 4,
        ROBA4 = 5,
        FLIP = 6
    };

private:
    Color color;
    Tipo tipo;
    int valor; // solo aplica si tipo == NUMERO

public:
    Carta(Color color, Tipo tipo, int valor);

    Color getColor() const;
    Tipo getTipo() const;
    int getValor() const;

    bool esNegra() const;

    std::string colorComoTexto() const;
    std::string tipoComoTexto() const;

    void imprimir() const;
};

#endif //LAB_EDD_PRACTICA_OFICIAL_UNO_CARTA_H