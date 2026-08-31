# Analizador Sintáctico LR(1) — Implementación usando Objetos

Práctica 3: mismo analizador sintáctico LR(1) de la práctica anterior ([Mini Analizador Sintactico](../Mini%20Analizador%20Sintactico)), pero ahora la `Pila` guarda **objetos** en lugar de enteros, para que al imprimirla se vean los símbolos igual que en un análisis hecho a mano.

## Clases nuevas

- **`ElementoPila`** — clase base (no se instancia directamente), guarda el entero `valor` que antes se apilaba suelto, y declara `muestra()` como virtual para que cada tipo de elemento se imprima distinto.
- **`Terminal`** — un terminal de la gramática (`id`, `+`, `$`). Además del valor guarda el texto real que reconoció el `Lexico` (ej. `"a"`, `"+"`).
- **`NoTerminal`** — un no terminal de la gramática (en esta práctica, solo `E`).
- **`Estado`** — un estado del autómata LR (los números que antes iban sueltos en la pila de enteros).

`Pila` pasó de `vector<int>` a `list<ElementoPila*>` (push al frente, top/pop desde el frente, `muestra()` recorre del fondo hacia arriba con `rbegin()/rend()`), siguiendo el patrón que dio el profesor con el ejemplo `Alumno`/`Bachillerato`/`Licenciatura`.

## Por qué no hace falta downcast

`getValor()` está definido una sola vez en `ElementoPila` (no es virtual) y las 3 clases hijas guardan su entero ahí mismo en el constructor. Así, el algoritmo de `analizarLR` sigue leyendo `pila.top()->getValor()` para `fila`/`columna` exactamente igual que con la pila de enteros — lo único que cambia es **qué se apila** (`new Terminal(...)`, `new NoTerminal(...)`, `new Estado(...)`) en vez de un `int` suelto.

## Comparación con la pila de enteros (práctica anterior)

| Pila de enteros | Pila de objetos |
|---|---|
| `2 0 0 2 1 3 0 4` | `$ 0 a 2 + 3 b 4` |

## Compilar y ejecutar

**Visual Studio / MSVC:**
```
cl /EHsc pila.cpp lexico.cpp principal.cpp /Fe:minisint_obj.exe
minisint_obj.exe
```

**g++:**
```
g++ pila.cpp lexico.cpp principal.cpp -o minisint_obj
./minisint_obj
```

## Salida de ejemplo (Ejercicio 1, cadena `a+b`)

```
pila: $ 0
entrada: a      accion: 2
pila: $ 0 a 2
entrada: +      accion: 3
pila: $ 0 a 2 + 3
entrada: b      accion: 4
pila: $ 0 a 2 + 3 b 4
entrada: $      accion: -2
reduccion 1
pila: $ 0 E 1
entrada: $      accion: -1

Resultado: CADENA ACEPTADA
```

Coincide exactamente con la traza manual `$0, $0a2, $0a2+3, $0a2+3b4, $0E1` de la tabla del profesor.
