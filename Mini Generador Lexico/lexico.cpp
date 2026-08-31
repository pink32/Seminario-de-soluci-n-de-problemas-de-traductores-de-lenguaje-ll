#include "lexico.h"

  Lexico::Lexico(string fuente){
    ind= 0;
    this->fuente= fuente;
  }

  Lexico::Lexico(){
    ind= 0;
  }

  string Lexico::tipoAcad(int tipo){
         string cad= "";

         switch (tipo){
           case TipoSimbolo::IDENTIFICADOR:
                cad= "Identificador";
                break;

           case TipoSimbolo::ENTERO:
                cad= "Entero";
                break;

           case TipoSimbolo::REAL:
                cad= "Real";
                break;

           case TipoSimbolo::PESOS:
                cad= "Fin de la Entrada";
                break;

           default:
                cad= "Error";
         }

         return cad;
  }

  void Lexico::entrada(string fuente){
       ind= 0;
      this->fuente= fuente;
  }

/*
 Automata de estados finitos:

  0 --letra--> 1 --letra|digito--> 1 (retroceso => IDENTIFICADOR)
  0 --digito--> 2 --digito--> 2
                2 --'.'-------> 3 --digito--> 4 --digito--> 4 (retroceso => REAL)
                2 --otro------> retroceso (=> ENTERO)
                3 --otro (sin digito)--> retroceso (=> ERROR, real mal formado)
  0 --espacio--> 0 (se ignora)
  0 --'$'------> 6 (=> PESOS)
  0 --otro-----> 5 (=> ERROR)
*/
int Lexico::sigSimbolo(){

    estado=0;
    continua= true;
    simbolo= "";

    //Inicio del Automata
    while (continua){
      c= sigCaracter();

      switch (estado){
        case 0:
             if ( esLetra(c) ) sigEstado(1);
             else
             if ( esDigito(c) ) sigEstado(2);
             else
             if ( esEspacio(c) ) { /* se ignora, permanece en estado 0 */ }
             else
             if ( c == '$' ) aceptacion(6);
             else
              aceptacion(5); //caracter no reconocido
             break;

        case 1: //identificador = letra (letra|digito)*
             if ( esLetra(c) || esDigito(c) ) sigEstado(1);
             else retroceso();
             break;

        case 2: //parte entera de un numero: entero
             if ( esDigito(c) ) sigEstado(2);
             else
             if ( c == '.' ) sigEstado(3);
             else retroceso();
             break;

        case 3: //se leyo el punto, real = entero.entero+ exige al menos un digito
             if ( esDigito(c) ) sigEstado(4);
             else retroceso(); //punto sin digitos despues => error
             break;

        case 4: //parte decimal de un real: entero+
             if ( esDigito(c) ) sigEstado(4);
             else retroceso();
             break;

      }

   }
    //Fin del Automata

      switch (estado){

         case 1:
              tipo= TipoSimbolo::IDENTIFICADOR;
              break;

         case 2:
              tipo= TipoSimbolo::ENTERO;
              break;

         case 4:
              tipo= TipoSimbolo::REAL;
              break;

         case 6:
              tipo= TipoSimbolo::PESOS;
              break;

         default:
              tipo= TipoSimbolo::ERROR;
      }

	return tipo;
}

 char Lexico::sigCaracter(){
      if (terminado()) return '$';

      return fuente[ind++];
 }

  void Lexico::sigEstado(int estado){
       this->estado= estado;
       simbolo+= c;
  }

  void Lexico::aceptacion(int estado){
       sigEstado(estado);
       continua= false;
  }

  bool Lexico::terminado(){//fin de cadena
         return ind >= fuente.length();
  }

   bool Lexico::esLetra(char c){
        return isalpha((unsigned char)c) || c=='_';
   }


   bool Lexico::esDigito(char c){
        return isdigit((unsigned char)c);
   }

   bool Lexico::esEspacio(char c){
        return c== ' ' || c== '\t';
   }


   void Lexico::retroceso(){
      if ( c != '$') ind--;
        continua= false;
    }
