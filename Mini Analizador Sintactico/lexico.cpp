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

           case TipoSimbolo::OPADIC:
                cad= "Op. Adicion";
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

  0 --letra-------> 1 --letra|digito--> 1  (retroceso => IDENTIFICADOR)
  0 --'+'---------> 2                      (=> OP. ADICION)
  0 --espacio-----> 0 (se ignora)
  0 --'$'---------> 3                      (=> PESOS, fin de la entrada)
  0 --otro--------> 4                      (=> ERROR)
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
             if ( c == '+' ) aceptacion(2);
             else
             if ( esEspacio(c) ) { /* se ignora, permanece en estado 0 */ }
             else
             if ( c == '$' ) aceptacion(3);
             else
              aceptacion(4); //caracter no reconocido
             break;

        case 1: //identificador = letra (letra|digito)*
             if ( esLetra(c) || esDigito(c) ) sigEstado(1);
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
              tipo= TipoSimbolo::OPADIC;
              break;

         case 3:
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
