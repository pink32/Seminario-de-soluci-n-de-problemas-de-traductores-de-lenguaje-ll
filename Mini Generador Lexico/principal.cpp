#include <cstdlib>
#include <iostream>
#include <string>

#include "lexico.h"

using namespace std;

int main(int argc, char *argv[]){

    Lexico lexico;
    lexico.entrada("suma total3 12.5 8 x1 12. 4a5 99.99");

    cout << "Resultado del Analisis Lexico" << endl << endl;
    cout << "Simbolo\t\tTipo" << endl;

    while ( lexico.simbolo.compare("$") != 0 ){
          lexico.sigSimbolo();

          cout <<  lexico.simbolo << "\t\t" << lexico.tipoAcad(lexico.tipo) << endl;
    }

    cin.get();

    return 0;
}
