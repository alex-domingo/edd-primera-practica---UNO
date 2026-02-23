#include "Carta.h"
#include <iostream>

Carta::Carta(Color color, Tipo tipo, int valor)
    : color(color), tipo(tipo), valor(valor) {
}

Carta::Color Carta::getColor() const { return color; }
Carta::Tipo Carta::getTipo() const { return tipo; }
int Carta::getValor() const { return valor; }

bool Carta::esNegra() const { return color == NEGRO; }

bool Carta::esComodin() const {
    return tipo == COMODIN || tipo == ROBA4;
}

bool Carta::esCartaAccion() const {
    return tipo == SALTO || tipo == REVERSA || tipo == ROBA2;
}

bool Carta::esJugableSobre(const Carta *cartaEnMesa, Color colorActual) const {
    if (cartaEnMesa == nullptr) return true;

    // comodines siempre jugables
    if (esComodin()) return true;

    // por color
    if (this->color == colorActual) return true;

    // por número
    if (this->tipo == NUMERO && cartaEnMesa->getTipo() == NUMERO) {
        return this->valor == cartaEnMesa->getValor();
    }

    // por símbolo/acción (skip sobre skip, reverse sobre reverse, draw2 sobre draw2)
    if (this->tipo != NUMERO && cartaEnMesa->getTipo() != NUMERO) {
        return this->tipo == cartaEnMesa->getTipo();
    }

    return false;
}

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
