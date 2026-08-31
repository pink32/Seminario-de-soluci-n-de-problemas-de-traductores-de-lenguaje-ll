#include <cstdlib>
#include <iostream>
#include <string>

#include "lexico.h"

using namespace std;

int main(int argc, char *argv[]){

    //Cadena de prueba que utiliza los 24 tipos de token de simbolos_lexicos.pdf
    string codigo=
        "if (x >= 10 && y != 3.5) {\n"
        "    resultado = x + y * 2;\n"
        "} else {\n"
        "    return \"fin\";\n"
        "}\n"
        "float pi = 3.1416;\n"
        "int contador, total;\n"
        "while (contador <= 100) {\n"
        "    contador = contador + 1;\n"
        "}\n"
        "!bandera;\n"
        "a || b;\n";

    Lexico lexico(codigo);

    cout << "Resultado del Analisis Lexico" << endl << endl;
    cout << "Simbolo\t\tTipo\t\tValor" << endl;
    cout << "------------------------------------------------" << endl;

    while ( lexico.simbolo.compare("$") != 0 ){
          lexico.sigSimbolo();

          cout << lexico.simbolo << "\t\t" << lexico.tipoAcad(lexico.tipo)
               << "\t\t" << lexico.tipo << endl;
    }

    cin.get();

    return 0;
}
