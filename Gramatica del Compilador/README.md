# Gramática del Compilador

Analizador sintáctico LR(1) que carga e implementa la **gramática completa del compilador** (52 reglas, `programa`, definiciones de variables/funciones, sentencias, expresiones), usando el mismo `Lexico` y la misma `Pila` de objetos de las prácticas anteriores, pero ahora la tabla LR(1) **se carga desde el archivo `compilador.lr`** en vez de escribirse a mano en el código.

## Qué cambia respecto a la práctica anterior

- `lexico.h`/`lexico.cpp` — sin cambios (viene de [Analizador Lexico Completo](../Analizador%20Lexico%20Completo)); sus 24 tipos de token ya coinciden exactamente con las columnas que pide `compilador.inf`.
- `pila.h`/`pila.cpp` — sin cambios (viene de [Analizador Sintactico Objetos](../Analizador%20Sintactico%20Objetos)): `Pila` de `ElementoPila*` con `Terminal`, `NoTerminal` y `Estado`.
- `principal.cpp` — se agrega `cargarTabla(...)`, que lee un archivo `.lr` con el formato descrito en `DescripcionArchivosGramaticas.pdf`, y `analizarLR(...)` ahora usa el nombre real de cada no terminal (`nombreReglas[i]`) en vez del `"E"` fijo de antes, ya que aquí hay 22 no terminales distintos (`programa`, `Definiciones`, `DefVar`, `Sentencia`, `Expresion`, etc.).

## Formato del archivo `.lr`

```
numReglas
numReglas lineas: idNoTerminal  longitudRegla  nombreNoTerminal
numFilas  numColumnas
numFilas lineas de numColumnas enteros (la tabla LR)
```

`cargarTabla` lo lee completo con `ifstream` y `>>` (que salta espacios/saltos de línea automáticamente), sin necesidad de parsear línea por línea.

## Archivos de la gramática

- `compilador.lr` — tabla LR(1) codificada (95 filas × 46 columnas: 24 terminales + 22 no terminales), la lee el programa en tiempo de ejecución.
- `compilador.inf` — documentación de la gramática: las 24 constantes de token (idénticas a las de `Analizador Lexico Completo`) y las 52 reglas numeradas (R1...R52). No la lee el programa, es solo referencia.

## Compilar y ejecutar

**Importante**: `compilador.lr` debe estar en la misma carpeta que el ejecutable (el programa lo abre con una ruta relativa).

**Visual Studio / MSVC:**
```
cl /EHsc pila.cpp lexico.cpp principal.cpp /Fe:gramatica.exe
gramatica.exe
```

**g++:**
```
g++ pila.cpp lexico.cpp principal.cpp -o gramatica
./gramatica
```

O simplemente dale doble clic a `ejecutar.bat`.

## Pruebas incluidas en `principal.cpp`

1. **Traza detallada** de `"int contador;"` — muestra paso a paso cómo la pila reduce por `R7 (ListaVar::=ε)`, `R6 (DefVar::=tipo identificador ListaVar ;)`, `R4`, `R2`, `R3`, `R1`, hasta aceptar.
2. **Programa completo** con dos funciones (`max`, `main`), `if/else`, `while`, llamada a función, variables locales y globales — se acepta correctamente.
3. **Programa inválido** (`"int x = 5;"`, que no es válido porque `DefVar` no admite inicialización) — se detecta como error sintáctico.

## Salida de ejemplo (traza de `"int contador;"`)

```
pila: $ 0
entrada: int      accion: 5
pila: $ 0 int 5
entrada: contador accion: 8
pila: $ 0 int 5 contador 8
entrada: ;        accion: -8
reduccion R7
pila: $ 0 int 5 contador 8 ListaVar 9
entrada: ;        accion: 12
pila: $ 0 int 5 contador 8 ListaVar 9 ; 12
entrada: $        accion: -7
reduccion R6
pila: $ 0 DefVar 4
entrada: $        accion: -5
reduccion R4
pila: $ 0 Definicion 3
entrada: $        accion: -3
reduccion R2
pila: $ 0 Definicion 3 Definiciones 7
entrada: $        accion: -4
reduccion R3
pila: $ 0 Definiciones 2
entrada: $        accion: -2
reduccion R1
pila: $ 0 programa 1
entrada: $        accion: -1

Resultado: PROGRAMA ACEPTADO
```
