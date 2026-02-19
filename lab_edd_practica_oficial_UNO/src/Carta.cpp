#include "Carta.h"
#include <iostream>

Carta::Carta(Color color, Tipo tipo, int valor)
    : color(color), tipo(tipo), valor(valor) {
}

Carta::Color Carta::getColor() const { return color; }
Carta::Tipo Carta::getTipo() const { return tipo; }
int Carta::getValor() const { return valor; }

bool Carta::esNegra() const { return color == NEGRO; }

std::string Carta::colorComoTexto() const {
    switch (color) {
        case ROJO: return "ROJO";
        case AMARILLO: return "AMARILLO";
        case AZUL: return "AZUL";
        case VERDE: return "VERDE";
        case NEGRO: return "NEGRO";
        default: return "DESCONOCIDO";
    }
}

std::string Carta::tipoComoTexto() const {
    switch (tipo) {
        case NUMERO: return "NUMERO";
        case SALTO: return "SALTO";
        case REVERSA: return "REVERSA";
        case ROBA2: return "ROBA2";
        case COMODIN: return "COMODIN";
        case ROBA4: return "ROBA4";
        case FLIP: return "FLIP";
        default: return "DESCONOCIDO";
    }
}

void Carta::imprimir() const {
    if (tipo == NUMERO) {
        std::cout << "[" << colorComoTexto() << " " << valor << "]";
    } else {
        std::cout << "[" << colorComoTexto() << " " << tipoComoTexto() << "]";
    }
}
