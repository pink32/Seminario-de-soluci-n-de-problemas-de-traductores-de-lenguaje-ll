#ifndef _LEXICO
#define _LEXICO

#include <iostream>
#include <string>
using namespace std;

// Tipos usados por las tablas LR de esta practica:
// identificador= 0, operador de adicion (+)= 1, fin de la entrada ($)= 2
class TipoSimbolo{
public:
 static const int ERROR= -1;
 static const int IDENTIFICADOR= 0;
 static const int OPADIC= 1;
 static const int PESOS= 2;
};


class Lexico{
      private:
       string fuente;

       int ind;
       bool continua;
       char c;
       int estado;

       char sigCaracter();
       void sigEstado(int estado);
       void aceptacion(int estado);
       bool esLetra(char c);
       bool esDigito(char c);
       bool esEspacio(char c);
       void retroceso();

      public:
       string simbolo;
       int tipo;

       Lexico(string fuente);
       Lexico();

       void entrada(string fuente);
       string tipoAcad(int tipo);

       int sigSimbolo();
       bool terminado();

};


#endif
