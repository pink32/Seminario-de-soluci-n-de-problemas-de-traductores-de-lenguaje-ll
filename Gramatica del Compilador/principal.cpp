#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "lexico.h"
#include "pila.h"

using namespace std;

/*
 Carga un archivo .lr (formato descrito en DescripcionArchivosGramaticas.pdf):

   numReglas
   numReglas lineas con: idNoTerminal  longitudRegla  nombreNoTerminal
   numFilas  numColumnas
   numFilas lineas con numColumnas enteros (la tabla LR)
*/
bool cargarTabla(string archivo, vector< vector<int> > &tabla, vector<int> &idReglas,
                  vector<int> &lonReglas, vector<string> &nombreReglas){

    ifstream in(archivo);
    if ( !in.is_open() ) return false;

    int numReglas;
    in >> numReglas;

    idReglas.resize(numReglas);
    lonReglas.resize(numReglas);
    nombreReglas.resize(numReglas);

    for ( int i= 0; i < numReglas; i++ )
          in >> idReglas[i] >> lonReglas[i] >> nombreReglas[i];

    int filas, columnas;
    in >> filas >> columnas;

    tabla.assign( filas, vector<int>(columnas) );

    for ( int f= 0; f < filas; f++ )
          for ( int c= 0; c < columnas; c++ )
                in >> tabla[f][c];

    return true;
}

/*
 Algoritmo general de analisis sintactico LR(1), con pila de objetos
 (ElementoPila*) y la tabla cargada desde un archivo .lr.

 tabla[fila][columna]:
   > 0  => desplazamiento (numero de estado al que se pasa)
   -1   => aceptacion
   < -1 => reduccion; el indice de la regla es (-accion - 2)
*/
bool analizarLR(string entrada, vector< vector<int> > &tabla, vector<int> &idReglas,
                 vector<int> &lonReglas, vector<string> &nombreReglas, bool detalle= true){

    Pila pila;
    Lexico lexico(entrada);
    int fila, columna, accion;
    bool aceptacion= false;
    bool error= false;

    pila.push( new Terminal(TipoSimbolo::PESOS, "$") );
    pila.push( new Estado(0) );
    lexico.sigSimbolo();

    while ( !aceptacion && !error ){

          fila= pila.top()->getValor();
          columna= lexico.tipo;

          if ( columna == TipoSimbolo::ERROR ){
               if (detalle) cout << "Error lexico en el simbolo: " << lexico.simbolo << endl;
               error= true;
               break;
          }

          accion= tabla[fila][columna];

          if (detalle){
               pila.muestra();
               cout << "entrada: " << lexico.simbolo << "\taccion: " << accion << endl;
          }

          if ( accion > 0 ){
               //desplazamiento
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

                    pila.push( new NoTerminal(noTerminal, nombreReglas[numRegla]) );
                    pila.push( new Estado(accion) );

                    if (detalle) cout << "reduccion R" << (numRegla + 1) << endl;
               }
          }
          else{
               //celda vacia => error sintactico
               error= true;
          }
    }

    if (detalle) cout << endl;
    if ( aceptacion ) cout << "Resultado: PROGRAMA ACEPTADO" << endl << endl;
    else cout << "Resultado: ERROR SINTACTICO" << endl << endl;

    return aceptacion;
}

int main(int argc, char *argv[]){

    vector< vector<int> > tabla;
    vector<int> idReglas, lonReglas;
    vector<string> nombreReglas;

    if ( !cargarTabla("compilador.lr", tabla, idReglas, lonReglas, nombreReglas) ){
         cout << "No se pudo abrir compilador.lr" << endl;
         cin.get();
         return 1;
    }

    cout << "Tabla cargada: " << tabla.size() << " filas x " << tabla[0].size()
         << " columnas, " << idReglas.size() << " reglas." << endl << endl;

    string programa=
        "int max(int a, int b) {\n"
        "    if (a > b) {\n"
        "        return a;\n"
        "    } else {\n"
        "        return b;\n"
        "    }\n"
        "}\n"
        "int contador;\n"
        "int main() {\n"
        "    int resultado;\n"
        "    contador = 0;\n"
        "    while (contador < 10) {\n"
        "        contador = contador + 1;\n"
        "    }\n"
        "    resultado = max(contador, 5);\n"
        "    return resultado;\n"
        "}\n";

    cout << "===== Traza detallada: \"int contador;\" =====" << endl << endl;
    analizarLR("int contador;", tabla, idReglas, lonReglas, nombreReglas, true);

    cout << "===== Analizando el programa de prueba =====" << endl << endl;
    cout << programa << endl;

    analizarLR(programa, tabla, idReglas, lonReglas, nombreReglas, false);

    string programaInvalido= "int x = 5;\n"; //falta <ListaVar> antes del valor, esto no es una asignacion valida en DefVar
    cout << "===== Analizando un programa invalido =====" << endl << endl;
    cout << programaInvalido << endl;
    analizarLR(programaInvalido, tabla, idReglas, lonReglas, nombreReglas, false);

    cin.get();

    return 0;
}
