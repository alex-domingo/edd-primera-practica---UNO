# UNO (Consola) - Estructuras de Datos

Este proyecto implementa un juego UNO por consola utilizando **estructuras de datos hechas a mano** (punteros y nodos enlazados),
sin usar librerías de estructuras (sin `vector`, `list`, `stack`, etc.).

## Características implementadas
- Mazo UNO oficial (108 cartas por deck) y selección de `nDecks` según número de jugadores.
- Reposición automática del mazo usando el descarte (conservando la carta superior).
- Turnos sobre **lista circular doble** de jugadores.
- Mano como **lista enlazada simple**.
- Mazo/descarte como **pila enlazada**.
- Reglas configurables:
  - Stacking (+2 apila +2, +4 apila +4).
  - Modo de robo A/B.
  - Permitir o no ganar con carta negra.

## Requisitos
- Linux: `g++` o `clang++`, `make`
- Windows: MinGW-w64 (g++) o LLVM/Clang, y `make` (por ejemplo `mingw32-make`).

## Compilación (Makefile)
### Linux / macOS
```bash
make
./build/bin/uno
```

### Windows (PowerShell o CMD)
Si usas MinGW:
```bat
mingw32-make
build\bin\uno.exe
```

> Nota: El Makefile detecta Windows por la variable `OS=Windows_NT`.

## Targets útiles
- `make` o `make all`: compila el ejecutable.
- `make run`: compila y ejecuta.
- `make clean`: elimina binarios/objetos.

## Documentación
- Ver `Documentacion_UNO_EDD.docx` para:
  - Diagrama UML y jerarquía de cartas
  - TADs
  - Manual de usuario
  - Complejidad
