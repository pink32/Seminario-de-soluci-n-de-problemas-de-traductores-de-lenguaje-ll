#ifndef _LEXICO
#define _LEXICO

#include <iostream>
#include <string>
using namespace std;

// Tipos de token reconocidos por el analizador
class TipoSimbolo{
public:
 static const int ERROR= -1;
 static const int IDENTIFICADOR= 0;
 static const int ENTERO= 1;
 static const int REAL= 2;
 static const int PESOS= 3;       // fin de la entrada
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
