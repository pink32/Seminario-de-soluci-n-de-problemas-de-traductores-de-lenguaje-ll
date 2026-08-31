# Mini Generador Léxico

Analizador léxico en C++ que reconoce dos tipos de token a partir de un autómata de estados finitos, siguiendo la estructura de la plantilla del curso ([referencia](https://github.com/TraductoresLenguajes2/Traductores/tree/master/Modulo1/Compilador)).

## Gramática reconocida

```
identificador = letra(letra|digito)*
real          = entero.entero+
```

- **Identificador**: empieza con una letra y sigue con cero o más letras/dígitos. Ej: `x`, `suma`, `total3`.
- **Real**: un entero, un punto y al menos un dígito después del punto. Ej: `12.5`, `99.99`. Un punto sin dígitos después (`12.`) se marca como **Error**.
- Además se reconocen **Enteros** sueltos (sin punto) y `$` como fin de la entrada.

## Estructura

- `lexico.h` / `lexico.cpp` — clase `Lexico`: implementa el autómata (`sigSimbolo`, `sigEstado`, `aceptacion`, `retroceso`) y la clase `TipoSimbolo` con los tipos de token.
- `principal.cpp` — programa de prueba que alimenta una cadena de entrada y muestra cada símbolo con su tipo.

## Autómata

```
0 --letra-->        1 --letra|digito--> 1   (retroceso => Identificador)
0 --digito-->        2 --digito--> 2
                      2 --'.'-----> 3 --digito--> 4 --digito--> 4  (retroceso => Real)
                      2 --otro-----------------------------> (retroceso => Entero)
                      3 --otro (sin digito)---------------> (retroceso => Error)
0 --espacio-->       0   (se ignora)
0 --'$'-->           6   (=> Fin de la Entrada)
0 --otro-->          5   (=> Error)
```

## Compilar y ejecutar

**Visual Studio / MSVC:**
```
cl /EHsc lexico.cpp principal.cpp /Fe:minilex.exe
minilex.exe
```

**g++:**
```
g++ lexico.cpp principal.cpp -o minilex
./minilex
```

## Salida de ejemplo

Entrada: `suma total3 12.5 8 x1 12. 4a5 99.99`

```
Simbolo         Tipo
suma            Identificador
total3          Identificador
12.5            Real
8               Entero
x1              Identificador
12.             Error
4               Entero
a5              Identificador
99.99           Real
$               Fin de la Entrada
```
