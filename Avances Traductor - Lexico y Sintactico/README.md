# Avances en la Construcción de tu Traductor — Análisis Léxico y Sintáctico

Analizador léxico y sintáctico que lee un archivo de código fuente, produce su lista de tokens, valida la sintaxis contra la gramática completa del compilador (52 reglas, cargada de `compilador.lr`), y reporta errores léxicos/sintácticos con número de línea.

Es la integración de las prácticas anteriores del curso: el `Lexico` de [Analizador Lexico Completo](../Analizador%20Lexico%20Completo), la `Pila` de objetos de [Analizador Sintactico Objetos](../Analizador%20Sintactico%20Objetos), y la carga de tabla LR(1) desde archivo de [Gramatica del Compilador](../Gramatica%20del%20Compilador) — ahora con entrada por archivo (no cadenas fijas en el código) y mensajes de error más completos.

## Diseño

- **`lexico.h` / `lexico.cpp`** — autómata de estados finitos que reconoce los 24 tipos de token (identificadores, enteros, reales, cadenas, palabras reservadas, operadores, puntuación). Sin cambios en la lógica de reconocimiento respecto a la práctica anterior.
- **`pila.h` / `pila.cpp`** — `Pila` de `ElementoPila*` (`Terminal`, `NoTerminal`, `Estado`), para que la pila del analizador sintáctico se pueda inspeccionar con los símbolos reales.
- **`principal.cpp`** — agrega:
  - **`struct Token { int linea; string simbolo; int tipo; }`** — la estructura de datos que almacena cada token. Es deliberadamente independiente de `Lexico` (que solo expone el símbolo "actual" mientras recorre el código): `Token` es la "fotografía" de un símbolo ya reconocido, lista para guardarse en una colección.
  - `leerArchivo(ruta, contenido)`: carga un archivo de texto completo a un `string`.
  - `cargarTabla(...)`: lee `compilador.lr` (igual que la práctica anterior).
  - `tokenizar(codigo)`: corre el `Lexico` sobre todo el código y arma un **`vector<Token>`** — la lista de tokens completa, en memoria, de principio a fin del archivo (incluyendo el token final `$`).
  - `listaTokens(tokens, out)`: recorre ese `vector<Token>` ya construido e **imprime la lista de tokens** (línea, símbolo, tipo, valor) — el entregable explícito de "una lista de tokens".
  - `analizarLR(tokens, ...)`: el algoritmo LR(1) shift-reduce de siempre, pero ahora recibe el `vector<Token>` que ya generó `tokenizar()` y avanza con un índice sobre él, en vez de volver a leer el código fuente. Así la lista de tokens es realmente lo que "se usa en fases posteriores del proceso de traducción" (como pide el enunciado), no solo una impresión en pantalla que se descarta.
  - `procesarArchivo(entrada, salida, ...)`: junta todo — lee el archivo, tokeniza una sola vez, corre el análisis léxico (impresión) y sintáctico (sobre la misma lista), imprime el reporte en consola **y lo guarda en un archivo de salida**.

## Decisiones de diseño importantes

0. **La lista de tokens se guarda en un `vector<Token>`, no se descarta al imprimirla.** `tokenizar()` corre el léxico una sola vez y produce la lista completa; tanto `listaTokens()` (la imprime) como `analizarLR()` (la analiza sintácticamente) reciben ese mismo `vector<Token>`. Antes, el analizador sintáctico volvía a crear su propio `Lexico` y releía el código desde cero — funcionaba, pero no reflejaba que el léxico y el sintáctico son dos fases que se comunican a través de una lista de tokens, que es justo lo que describe el enunciado de la práctica.
1. **Número de línea en cada token.** Se agregó un contador `linea` a `Lexico` (incrementado en `sigCaracter()` cada vez que se lee `'\n'`, y ajustado en `retroceso()` si el carácter que se regresa era un salto de línea). Sin esto, un error solo diría "el símbolo X está mal", sin decir dónde — no cumple con el requisito de "mensajes de error útiles y precisos".
2. **Truncar símbolos largos al mostrarlos** (`paraMostrar`). Una cadena sin comilla de cierre consume el resto del archivo como un solo token; imprimirlo completo (con saltos de línea y todo) hace el reporte ilegible. Se corta en el primer salto de línea y se limita a 30 caracteres, solo para el reporte — el símbolo real (`lexico.simbolo`) no se modifica.
3. **Un error léxico detiene el análisis sintáctico.** Si el lexer no puede identificar un símbolo, el parser no tiene con qué columna de la tabla trabajar, así que continuar generaría errores en cascada sin sentido. En cuanto aparece un token de tipo `ERROR`, se reporta y se detiene ahí — igual que como se comporta un compilador real.
4. **El error sintáctico dice qué se esperaba.** Cuando la celda de la tabla está vacía (0), en vez de solo decir "error", se recorren las columnas de terminales (0 a 23) de esa misma fila y se listan los nombres de los símbolos que sí tenían una acción válida (usando `lexico.tipoAcad(columna)` para el nombre). Esto reutiliza la tabla ya cargada, no requiere información extra.
5. **La tabla LR se carga del archivo, no se escribe a mano.** Igual que en la práctica anterior — permite que el mismo ejecutable sirva para cualquier gramática con ese formato `.lr`, sin recompilar.

## Cómo se manejan los errores

- **Léxicos**: cualquier carácter que no encaje en ningún estado del autómata, un real mal formado (ej. `3.` sin dígitos después del punto), o una cadena sin comilla de cierre, se marcan con tipo `ERROR` y se reportan como `ERROR LEXICO en linea N: "..." no es un simbolo valido`.
- **Sintácticos**: cuando la tabla LR no tiene una acción para el estado/token actual, se reporta `ERROR SINTACTICO en linea N: no se esperaba "..."`, junto con la lista de símbolos que sí eran válidos en ese punto.
- En ambos casos el análisis se detiene en el primer error (no se intenta "recuperación de errores" — no lo pide la práctica, y complicaría la implementación sin aportar al objetivo).

## Archivos de entrada de muestra

| Archivo | Qué demuestra |
|---|---|
| `entrada1_valido.txt` | Programa completo y válido: dos funciones, `if/else`, `while`, llamada a función, aritmética, variables locales y globales. Se acepta sin errores. |
| `entrada2_error_lexico.txt` | Tres errores léxicos distintos: real mal formado (`3.`), carácter inválido (`@`), y cadena sin comilla de cierre. |
| `entrada3_error_sintactico.txt` | Un `int contador` sin punto y coma — error sintáctico que muestra los símbolos esperados (`;`, `,`, `(`). |

La salida de cada uno queda guardada en `salida1_valido.txt`, `salida2_error_lexico.txt` y `salida3_error_sintactico.txt` respectivamente (se generan solas al correr el programa).

## Compilar y ejecutar

**Importante**: `compilador.lr` y los archivos `entradaN_*.txt` deben estar en la misma carpeta que el ejecutable.

**Visual Studio / MSVC:**
```
cl /EHsc pila.cpp lexico.cpp principal.cpp /Fe:traductor.exe
traductor.exe
```

**g++:**
```
g++ pila.cpp lexico.cpp principal.cpp -o traductor
./traductor
```

O dale doble clic a `ejecutar.bat`.

Sin argumentos, el programa corre los 3 archivos de muestra automáticamente. También se puede analizar un archivo específico:

```
traductor.exe miArchivo.txt
```

(el reporte se guarda en `salida.txt`).

## Salida de ejemplo (`entrada3_error_sintactico.txt`)

```
--- Analisis sintactico ---

ERROR SINTACTICO en linea 3: no se esperaba "int" (Tipo)
  Se esperaba alguno de: Punto y Coma, Coma, Parentesis Izq.

Resultado: EL PROGRAMA FUE RECHAZADO.
```

(El `(` aparece como válido porque hasta ese punto `tipo identificador` podría ser el inicio de una variable **o** de una función — la gramática solo lo sabe al ver el siguiente símbolo.)
