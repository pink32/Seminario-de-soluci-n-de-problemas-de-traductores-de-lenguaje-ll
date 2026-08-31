# Analizador Léxico Completo

Analizador léxico en C++ que reconoce **todos** los símbolos definidos en `simbolos_lexicos.pdf` (Proyecto Taller Compiladores), con el mismo autómata de estados finitos usado en las prácticas anteriores.

## Símbolos reconocidos y su valor de tipo

| Símbolo | Tipo | Valor |
|---|---|---|
| identificador | `letra(letra\|digito)*` | 0 |
| entero | `digito+` | 1 |
| real | `entero.entero+` | 2 |
| cadena | texto entre comillas dobles `"..."` | 3 |
| tipo | `int`, `float`, `void` | 4 |
| opSuma | `+`  `-` | 5 |
| opMul | `*`  `/` | 6 |
| opRelac | `<`  `<=`  `>`  `>=` | 7 |
| opOr | `\|\|` | 8 |
| opAnd | `&&` | 9 |
| opNot | `!` | 10 |
| opIgualdad | `==`  `!=` | 11 |
| `;` | | 12 |
| `,` | | 13 |
| `(` | | 14 |
| `)` | | 15 |
| `{` | | 16 |
| `}` | | 17 |
| `=` (asignación) | | 18 |
| `if` | | 19 |
| `while` | | 20 |
| `return` | | 21 |
| `else` | | 22 |
| `$` (fin de la entrada) | | 23 |

Las palabras reservadas (`if`, `while`, `return`, `else`, `int`, `float`, `void`) se detectan reconociendo primero un identificador normal y luego comparando el texto contra la lista de reservadas (`Lexico::tipoPalabraReservada`) — la técnica estándar de "maximal munch + lookup".

## Estructura

- `lexico.h` / `lexico.cpp` — clase `Lexico` (autómata de estados) y `TipoSimbolo` con los 24 tipos + `ERROR`.
- `principal.cpp` — prueba con un fragmento de código que usa los 24 tipos de token, y una tabla imprimiendo símbolo/tipo/valor.

## Autómata (resumen)

```
0 --letra-------> 1 (identificador / palabra reservada)
0 --digito------> 2 --'.'--> 3 --digito--> 4   (entero / real)
0 --'"'---------> 5 --'"'--> 6                  (cadena)
0 --'+'|'-'-----> 7                              (opSuma)
0 --'*'|'/'-----> 8                              (opMul)
0 --'<'---------> 9  --'='--> 10                (opRelac: "<" o "<=")
0 --'>'---------> 11 --'='--> 12                (opRelac: ">" o ">=")
0 --'='---------> 13 --'='--> 14                (asignacion "=" o opIgualdad "==")
0 --'!'---------> 15 --'='--> 16                (opNot "!" o opIgualdad "!=")
0 --'&'---------> 17 --'&'--> 18                (opAnd "&&"; "&" solo = error)
0 --'|'---------> 19 --'|'--> 20                (opOr "||"; "|" solo = error)
0 --';'|','|'('|')'|'{'|'}'|'$' --> aceptacion directa
0 --espacio-----> 0 (se ignora)
0 --otro--------> error (caracter no reconocido)
```

Casos de error detectados: carácter no reconocido, real mal formado (`12.` sin dígitos después del punto), cadena sin comilla de cierre, y `&`/`|` sueltos (no forman `&&`/`||`).

## Compilar y ejecutar

**Visual Studio / MSVC:**
```
cl /EHsc lexico.cpp principal.cpp /Fe:lexico_completo.exe
lexico_completo.exe
```

**g++:**
```
g++ lexico.cpp principal.cpp -o lexico_completo
./lexico_completo
```

## Salida de ejemplo (extracto)

```
Simbolo         Tipo                    Valor
------------------------------------------------
if              if                      19
(               Parentesis Izq.         14
x               Identificador           0
>=              Op. Relacional          7
10              Entero                  1
&&              Op. And                 9
y               Identificador           0
!=              Op. Igualdad            11
3.5             Real                    2
)               Parentesis Der.         15
...
"fin"           Cadena                  3
...
float           Tipo                    4
...
$               Fin de la Entrada       23
```
