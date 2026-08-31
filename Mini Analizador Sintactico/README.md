# Mini Analizador Sintáctico (LR(1) con pila de enteros)

Implementación de los **Ejercicios 1 y 2** de la práctica "Analizador Sintáctico LR(1) — Implementación usando Pila de Enteros", usando la técnica de tabla de decisiones (shift/reduce) enseñada en clase.

## Gramáticas

- **Ejercicio 1**: `E → <id> + <id>` — cadena de prueba: `a+b`
- **Ejercicio 2**: `E → <id> + E | <id>` (recursiva) — cadenas de prueba: `a+b`, `a+b+c`, `a+b+c+d`

## Estructura

- `pila.h` / `pila.cpp` — clase `Pila`, una pila de enteros (`push`, `pop`, `top`, `muestra`), donde el tope es el elemento más a la derecha.
- `lexico.h` / `lexico.cpp` — clase `Lexico` (autómata de estados, igual que en la práctica anterior) extendida para reconocer los tres terminales que usan estas tablas: `identificador= 0`, `+ (operador de adición)= 1`, `$ (fin de la entrada)= 2`.
- `principal.cpp` — contiene:
  - `analizarLR(...)`: el algoritmo general LR(1) con pila de enteros (funciona para cualquier tabla/gramática que se le pase).
  - `ejercicio1()` y `ejercicio2()`: arman su propia tabla LR y llaman a `analizarLR`.

## El algoritmo

En cada paso se consulta `tabla[pila.top()][lexico.tipo]`:

- **`accion > 0`** → desplazamiento: se mete a la pila el símbolo actual y el nuevo estado (`accion`), y se pide el siguiente símbolo.
- **`accion == -1`** → aceptación: la cadena es válida.
- **`accion < -1`** → reducción de la regla `(-accion - 2)`: se sacan de la pila `longitud_regla * 2` elementos (cada símbolo ocupa dos posiciones: su valor y su estado), y luego se hace la transición (`goto`) usando el no terminal de esa regla como columna.
- **`accion == 0`** → celda vacía: error sintáctico.

El Ejercicio 2 usa la gramática recursiva, así que el mismo algoritmo (con un `while`) maneja cualquier cantidad de `+id` sin cambiar el código — solo cambia la tabla y los arreglos de reglas.

### Reglas de reducción

| Ejercicio | Regla | No terminal (columna E) | Longitud |
|---|---|---|---|
| 1 | `E → id + id` | 3 | 3 |
| 2 | `E → id + E` | 3 | 3 |
| 2 | `E → id` | 3 | 1 |

## Compilar y ejecutar

**Visual Studio / MSVC:**
```
cl /EHsc pila.cpp lexico.cpp principal.cpp /Fe:minisint.exe
minisint.exe
```

**g++:**
```
g++ pila.cpp lexico.cpp principal.cpp -o minisint
./minisint
```

## Salida de ejemplo (Ejercicio 1, cadena `a+b`)

```
pila: 2 0
entrada: a      accion: 2
pila: 2 0 0 2
entrada: +      accion: 3
pila: 2 0 0 2 1 3
entrada: b      accion: 4
pila: 2 0 0 2 1 3 0 4
entrada: $      accion: -2
reduccion 1
pila: 2 0 3 1
entrada: $      accion: -1

Resultado: CADENA ACEPTADA
```

Esta traza coincide exactamente con la tabla del PDF de referencia (`Tabla Ejercicio 1 practica 2`).
