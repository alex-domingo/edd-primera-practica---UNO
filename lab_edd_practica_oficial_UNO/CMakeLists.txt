cmake_minimum_required(VERSION 4.1)
project(lab_edd_practica_oficial_UNO)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_executable(lab_edd_practica_oficial_UNO main.cpp
        include/Carta.h
        include/PilaCartas.h
        include/Mano.h
        include/Jugador.h
        include/MesaJugadores.h
        include/Partida.h
        src/Carta.cpp
        src/PilaCartas.cpp
        src/Mano.cpp
        src/Jugador.cpp
        src/MesaJugadores.cpp
        src/Partida.cpp
)
