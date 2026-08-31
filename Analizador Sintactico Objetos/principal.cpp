#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "lexico.h"
#include "pila.h"

using namespace std;

/*
 Algoritmo general de analisis sintactico LR(1), ahora con pila de objetos
 (ElementoPila*) en lugar de pila de enteros.

 tabla[fila][columna]:
   > 0  => desplazamiento (numero de estado al que se pasa)
   -1   => aceptacion
   < -1 => reduccion; el indice de la regla es (-accion - 2)

 idReglas[i]  = entero que representa el no terminal del lado izquierdo de la regla i
 lonReglas[i] = numero de simbolos del lado derecho de la regla i (cuantos elementos
                se quitan de la pila, multiplicado por 2 porque cada simbolo ocupa
                dos posiciones: su valor y el estado que le sigue)

 fila y columna se siguen obteniendo con enteros (ElementoPila::getValor()),
 lo unico que cambia es que ahora se guardan objetos Terminal/NoTerminal/Estado
 en vez del entero suelto, para que pila.muestra() imprima los simbolos reales.
*/
void analizarLR(string entrada, vector< vector<int> > tabla, vector<int> idReglas, vector<int> lonReglas){

    Pila pila;
    Lexico lexico(entrada);
    int fila, columna, accion;
    bool aceptacion= false;
    bool error= false;

    cout << "Analizando la cadena: \"" << entrada << "\"" << endl << endl;

    pila.push( new Terminal(TipoSimbolo::PESOS, "$") );
    pila.push( new Estado(0) );
    lexico.sigSimbolo();

    while ( !aceptacion && !error ){

          fila= pila.top()->getValor();
          columna= lexico.tipo;
          accion= tabla[fila][columna];

          pila.muestra();
          cout << "entrada: " << lexico.simbolo << "\taccion: " << accion << endl;

          if ( accion > 0 ){
               //desplazamiento: se apila el terminal reconocido y el nuevo estado
               pila.push( new Terminal(lexico.tipo, lexico.simbolo) );
               pila.push( new Estado(accion) );
               lexico.sigSimbolo();
          }
          else
          if ( accion < 0 ){
               if ( accion == -1 ){
                    aceptacion= true;
               }
               else{
                    //reduccion
                    int numRegla= (-accion) - 2;
                    int lon= lonReglas[numRegla];
                    int noTerminal= idReglas[numRegla];

                    for ( int j= 0; j < lon * 2; j++ ) pila.pop();

                    fila= pila.top()->getValor();
                    accion= tabla[fila][noTerminal]; //transicion (goto)

                    pila.push( new NoTerminal(noTerminal, "E") );
                    pila.push( new Estado(accion) );

                    cout << "reduccion " << (numRegla + 1) << endl;
               }
          }
          else{
               //celda vacia => error sintactico
               error= true;
          }
    }

    cout << endl;
    if ( aceptacion ) cout << "Resultado: CADENA ACEPTADA" << endl << endl;
    else cout << "Resultado: ERROR SINTACTICO" << endl << endl;
}

//Ejercicio 1: Gramatica E -> <id> + <id>
void ejercicio1(){

    cout << "===== Ejercicio 1: E -> <id> + <id> =====" << endl << endl;

    //columnas: 0= id, 1= +, 2= $, 3= E
    vector< vector<int> > tabla= {
        { 2, 0,  0, 1},
        { 0, 0, -1, 0},
        { 0, 3,  0, 0},
        { 4, 0,  0, 0},
        { 0, 0, -2, 0}
    };

    //regla 1: E -> id + id  (no terminal E= 3, longitud= 3 simbolos)
    vector<int> idReglas=  {3};
    vector<int> lonReglas= {3};

    analizarLR("a+b", tabla, idReglas, lonReglas);
}

//Ejercicio 2: Gramatica E -> <id> + E | <id>  (recursiva)
void ejercicio2(){

    cout << "===== Ejercicio 2: E -> <id> + E | <id> =====" << endl << endl;

    //columnas: 0= id, 1= +, 2= $, 3= E
    vector< vector<int> > tabla= {
        { 2, 0,  0, 1},
        { 0, 0, -1, 0},
        { 0, 3, -3, 0},
        { 2, 0,  0, 4},
        { 0, 0, -2, 0}
    };

    //regla 1: E -> id + E  (no terminal E= 3, longitud= 3 simbolos)
    //regla 2: E -> id      (no terminal E= 3, longitud= 1 simbolo)
    vector<int> idReglas=  {3, 3};
    vector<int> lonReglas= {3, 1};

    analizarLR("a+b", tabla, idReglas, lonReglas);
    analizarLR("a+b+c", tabla, idReglas, lonReglas);
    analizarLR("a+b+c+d", tabla, idReglas, lonReglas);
}

int main(int argc, char *argv[]){

    ejercicio1();
    ejercicio2();

    cin.get();

    return 0;
}
