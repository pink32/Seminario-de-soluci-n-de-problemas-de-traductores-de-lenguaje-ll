#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>

#include "lexico.h"
#include "pila.h"

using namespace std;

//Para mostrar en pantalla/reporte: recorta saltos de linea y limita el largo,
//util para simbolos muy largos como una cadena sin cerrar que consumio el resto del archivo.
string paraMostrar(string simbolo){
    size_t salto= simbolo.find('\n');
    string cad= (salto == string::npos) ? simbolo : simbolo.substr(0, salto) + "...";

    const size_t maxLargo= 30;
    if ( cad.length() > maxLargo ) cad= cad.substr(0, maxLargo) + "...";

    return cad;
}

bool leerArchivo(string ruta, string &contenido){
    ifstream archivo(ruta);
    if ( !archivo.is_open() ) return false;

    ostringstream buffer;
    buffer << archivo.rdbuf();
    contenido= buffer.str();

    return true;
}

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

//Recorre todo el codigo y escribe la lista de tokens en "out". Regresa la cantidad de errores lexicos.
int listaTokens(string codigo, ostream &out){

    Lexico lexico(codigo);
    int errores= 0;

    out << "--- Lista de Tokens (analisis lexico) ---" << endl << endl;
    out << left << setw(7) << "Linea" << setw(16) << "Simbolo" << setw(22) << "Tipo" << "Valor" << endl;
    out << string(55, '-') << endl;

    while ( lexico.simbolo.compare("$") != 0 ){
          lexico.sigSimbolo();

          out << left << setw(7) << lexico.linea << setw(16) << paraMostrar(lexico.simbolo) << " "
              << setw(22) << lexico.tipoAcad(lexico.tipo) << lexico.tipo << endl;

          if ( lexico.tipo == TipoSimbolo::ERROR ){
               errores++;
               out << "         >> ERROR LEXICO en linea " << lexico.linea
                   << ": \"" << paraMostrar(lexico.simbolo) << "\" no es un simbolo valido" << endl;
          }
    }

    out << endl;
    if ( errores == 0 ) out << "Analisis lexico: sin errores." << endl << endl;
    else out << "Analisis lexico: " << errores << " error(es) lexico(s) encontrado(s)." << endl << endl;

    return errores;
}

/*
 Algoritmo general de analisis sintactico LR(1), con pila de objetos (ElementoPila*)
 y la tabla cargada desde un archivo .lr. Reporta errores lexicos y sintacticos con
 numero de linea, y en un error sintactico indica que simbolos se esperaban.
*/
bool analizarLR(string entrada, vector< vector<int> > &tabla, vector<int> &idReglas,
                 vector<int> &lonReglas, vector<string> &nombreReglas, ostream &out){

    Pila pila;
    Lexico lexico(entrada);
    int fila, columna, accion;
    bool aceptacion= false;
    bool error= false;

    out << "--- Analisis sintactico ---" << endl << endl;

    pila.push( new Terminal(TipoSimbolo::PESOS, "$") );
    pila.push( new Estado(0) );
    lexico.sigSimbolo();

    while ( !aceptacion && !error ){

          if ( lexico.tipo == TipoSimbolo::ERROR ){
               out << "ERROR LEXICO en linea " << lexico.linea
                   << ": \"" << paraMostrar(lexico.simbolo) << "\" no es un simbolo valido; "
                   << "el analisis sintactico no puede continuar." << endl;
               error= true;
               break;
          }

          fila= pila.top()->getValor();
          columna= lexico.tipo;
          accion= tabla[fila][columna];

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
               }
          }
          else{
               //celda vacia => error sintactico
               out << "ERROR SINTACTICO en linea " << lexico.linea << ": no se esperaba \""
                   << paraMostrar(lexico.simbolo) << "\" (" << lexico.tipoAcad(lexico.tipo) << ")" << endl;

               out << "  Se esperaba alguno de: ";
               bool alguno= false;
               for ( int col= 0; col <= TipoSimbolo::PESOS; col++ ){
                     if ( tabla[fila][col] != 0 ){
                          if (alguno) out << ", ";
                          out << lexico.tipoAcad(col);
                          alguno= true;
                     }
               }
               if ( !alguno ) out << "(ningun simbolo es valido en este punto)";
               out << endl;

               error= true;
          }
    }

    out << endl;
    if ( aceptacion ) out << "Resultado: PROGRAMA ACEPTADO (sin errores sintacticos)." << endl << endl;
    else out << "Resultado: EL PROGRAMA FUE RECHAZADO." << endl << endl;

    return aceptacion;
}

//Lee rutaEntrada, corre el analisis lexico y sintactico, e imprime/guarda el reporte en rutaSalida.
void procesarArchivo(string rutaEntrada, string rutaSalida, vector< vector<int> > &tabla,
                      vector<int> &idReglas, vector<int> &lonReglas, vector<string> &nombreReglas){

    string codigo;
    if ( !leerArchivo(rutaEntrada, codigo) ){
         cout << "No se pudo abrir " << rutaEntrada << endl << endl;
         return;
    }

    ostringstream reporte;

    reporte << "=========================================================" << endl;
    reporte << " Analisis de: " << rutaEntrada << endl;
    reporte << "=========================================================" << endl << endl;

    reporte << "--- Codigo fuente ---" << endl << endl << codigo << endl << endl;

    listaTokens(codigo, reporte);
    analizarLR(codigo, tabla, idReglas, lonReglas, nombreReglas, reporte);

    cout << reporte.str();

    ofstream salida(rutaSalida);
    salida << reporte.str();
    salida.close();

    cout << "(Reporte guardado en " << rutaSalida << ")" << endl << endl;
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

    //Si se pasa un archivo por linea de comandos, se analiza solo ese.
    if ( argc > 1 ){
         procesarArchivo(argv[1], "salida.txt", tabla, idReglas, lonReglas, nombreReglas);
         cin.get();
         return 0;
    }

    //Sin argumentos, se corren los 3 casos de muestra del proyecto.
    procesarArchivo("entrada1_valido.txt",          "salida1_valido.txt",          tabla, idReglas, lonReglas, nombreReglas);
    procesarArchivo("entrada2_error_lexico.txt",     "salida2_error_lexico.txt",     tabla, idReglas, lonReglas, nombreReglas);
    procesarArchivo("entrada3_error_sintactico.txt", "salida3_error_sintactico.txt", tabla, idReglas, lonReglas, nombreReglas);

    cin.get();

    return 0;
}
