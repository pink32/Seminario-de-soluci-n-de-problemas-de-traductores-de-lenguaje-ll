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
           case TipoSimbolo::IDENTIFICADOR: cad= "Identificador";       break;
           case TipoSimbolo::ENTERO:        cad= "Entero";              break;
           case TipoSimbolo::REAL:          cad= "Real";                break;
           case TipoSimbolo::CADENA:        cad= "Cadena";              break;
           case TipoSimbolo::TIPO:          cad= "Tipo";                break;
           case TipoSimbolo::OPSUMA:        cad= "Op. Suma";            break;
           case TipoSimbolo::OPMUL:         cad= "Op. Multiplicacion";  break;
           case TipoSimbolo::OPRELAC:       cad= "Op. Relacional";      break;
           case TipoSimbolo::OPOR:          cad= "Op. Or";              break;
           case TipoSimbolo::OPAND:         cad= "Op. And";             break;
           case TipoSimbolo::OPNOT:         cad= "Op. Not";             break;
           case TipoSimbolo::OPIGUALDAD:    cad= "Op. Igualdad";        break;
           case TipoSimbolo::PYCOMA:        cad= "Punto y Coma";        break;
           case TipoSimbolo::COMA:          cad= "Coma";                break;
           case TipoSimbolo::PARIZQ:        cad= "Parentesis Izq.";     break;
           case TipoSimbolo::PARDER:        cad= "Parentesis Der.";     break;
           case TipoSimbolo::LLAIZQ:        cad= "Llave Izq.";          break;
           case TipoSimbolo::LLADER:        cad= "Llave Der.";          break;
           case TipoSimbolo::ASIGNACION:    cad= "Asignacion";          break;
           case TipoSimbolo::IF:            cad= "if";                  break;
           case TipoSimbolo::WHILE:         cad= "while";               break;
           case TipoSimbolo::RETURN:        cad= "return";              break;
           case TipoSimbolo::ELSE:          cad= "else";                break;
           case TipoSimbolo::PESOS:         cad= "Fin de la Entrada";   break;
           default:                         cad= "Error";
         }

         return cad;
  }

  //Revisa si un identificador ya leido es en realidad una palabra reservada.
  int Lexico::tipoPalabraReservada(string palabra){
      if (palabra == "int" || palabra == "float" || palabra == "void")
           return TipoSimbolo::TIPO;
      if (palabra == "if")     return TipoSimbolo::IF;
      if (palabra == "while")  return TipoSimbolo::WHILE;
      if (palabra == "return") return TipoSimbolo::RETURN;
      if (palabra == "else")   return TipoSimbolo::ELSE;

      return TipoSimbolo::IDENTIFICADOR;
  }

  void Lexico::entrada(string fuente){
       ind= 0;
      this->fuente= fuente;
  }

/*
 Automata de estados finitos:

  0 --letra-----------> 1  --letra|digito--> 1        (retroceso => IDENTIFICADOR o palabra reservada)
  0 --digito----------> 2  --digito--> 2
                          2 --'.'---> 3 --digito--> 4 --digito--> 4  (retroceso => REAL)
                          2 --otro--------------------------------> (retroceso => ENTERO)
                          3 --otro (sin digito)-------------------> (retroceso => ERROR, real mal formado)
  0 --'"'--------------> 5  --caracter--> 5, --'"'--> 6 (=> CADENA), --'$'--> (=> ERROR, sin cerrar)
  0 --'+'|'-'----------> 7                              (=> OP. SUMA)
  0 --'*'|'/'----------> 8                              (=> OP. MULTIPLICACION)
  0 --'<'--------------> 9  --'='--> 10 (=> OP. RELAC "<="),  otro--> retroceso (=> OP. RELAC "<")
  0 --'>'--------------> 11 --'='--> 12 (=> OP. RELAC ">="),  otro--> retroceso (=> OP. RELAC ">")
  0 --'='--------------> 13 --'='--> 14 (=> OP. IGUALDAD "=="), otro--> retroceso (=> ASIGNACION "=")
  0 --'!'--------------> 15 --'='--> 16 (=> OP. IGUALDAD "!="), otro--> retroceso (=> OP. NOT "!")
  0 --'&'--------------> 17 --'&'--> 18 (=> OP. AND "&&"),      otro--> retroceso (=> ERROR, "&" solo)
  0 --'|'--------------> 19 --'|'--> 20 (=> OP. OR "||"),       otro--> retroceso (=> ERROR, "|" solo)
  0 --';'--------------> 21 (=> PUNTO Y COMA)
  0 --','--------------> 22 (=> COMA)
  0 --'('--------------> 23 (=> PARENTESIS IZQ.)
  0 --')'--------------> 24 (=> PARENTESIS DER.)
  0 --'{'--------------> 25 (=> LLAVE IZQ.)
  0 --'}'--------------> 26 (=> LLAVE DER.)
  0 --'$'--------------> 27 (=> FIN DE LA ENTRADA)
  0 --espacio-----------> 0 (se ignora)
  0 --otro--------------> 28 (=> ERROR, caracter no reconocido)
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
             if ( c == '"' ) sigEstado(5);
             else
             if ( esEspacio(c) ) { /* se ignora */ }
             else
             if ( c == '+' || c == '-' ) aceptacion(7);
             else
             if ( c == '*' || c == '/' ) aceptacion(8);
             else
             if ( c == '<' ) sigEstado(9);
             else
             if ( c == '>' ) sigEstado(11);
             else
             if ( c == '=' ) sigEstado(13);
             else
             if ( c == '!' ) sigEstado(15);
             else
             if ( c == '&' ) sigEstado(17);
             else
             if ( c == '|' ) sigEstado(19);
             else
             if ( c == ';' ) aceptacion(21);
             else
             if ( c == ',' ) aceptacion(22);
             else
             if ( c == '(' ) aceptacion(23);
             else
             if ( c == ')' ) aceptacion(24);
             else
             if ( c == '{' ) aceptacion(25);
             else
             if ( c == '}' ) aceptacion(26);
             else
             if ( c == '$' ) aceptacion(27);
             else
              aceptacion(28); //caracter no reconocido
             break;

        case 1: //identificador o palabra reservada
             if ( esLetra(c) || esDigito(c) ) sigEstado(1);
             else retroceso();
             break;

        case 2: //parte entera de un numero
             if ( esDigito(c) ) sigEstado(2);
             else
             if ( c == '.' ) sigEstado(3);
             else retroceso();
             break;

        case 3: //se leyo el punto, real exige al menos un digito despues
             if ( esDigito(c) ) sigEstado(4);
             else retroceso(); //punto sin digitos despues => error
             break;

        case 4: //parte decimal de un real
             if ( esDigito(c) ) sigEstado(4);
             else retroceso();
             break;

        case 5: //dentro de una cadena, esperando la comilla de cierre
             if ( c == '"' ) aceptacion(6);
             else
             if ( c == '$' ) retroceso(); //fin de archivo sin cerrar la cadena => error
             else sigEstado(5);
             break;

        case 9: // se leyo '<'
             if ( c == '=' ) aceptacion(10);
             else retroceso();
             break;

        case 11: // se leyo '>'
             if ( c == '=' ) aceptacion(12);
             else retroceso();
             break;

        case 13: // se leyo '='
             if ( c == '=' ) aceptacion(14);
             else retroceso();
             break;

        case 15: // se leyo '!'
             if ( c == '=' ) aceptacion(16);
             else retroceso();
             break;

        case 17: // se leyo '&'
             if ( c == '&' ) aceptacion(18);
             else retroceso(); //'&' solo no es un simbolo valido => error
             break;

        case 19: // se leyo '|'
             if ( c == '|' ) aceptacion(20);
             else retroceso(); //'|' solo no es un simbolo valido => error
             break;

      }

   }
    //Fin del Automata

      switch (estado){

         case 1:
              tipo= tipoPalabraReservada(simbolo);
              break;

         case 2:
              tipo= TipoSimbolo::ENTERO;
              break;

         case 4:
              tipo= TipoSimbolo::REAL;
              break;

         case 6:
              tipo= TipoSimbolo::CADENA;
              break;

         case 7:
              tipo= TipoSimbolo::OPSUMA;
              break;

         case 8:
              tipo= TipoSimbolo::OPMUL;
              break;

         case 9:  //"<" solo
         case 10: //"<="
              tipo= TipoSimbolo::OPRELAC;
              break;

         case 11: //">" solo
         case 12: //">="
              tipo= TipoSimbolo::OPRELAC;
              break;

         case 13: //"=" solo
              tipo= TipoSimbolo::ASIGNACION;
              break;

         case 14: //"=="
         case 16: //"!="
              tipo= TipoSimbolo::OPIGUALDAD;
              break;

         case 15: //"!" solo
              tipo= TipoSimbolo::OPNOT;
              break;

         case 18: //"&&"
              tipo= TipoSimbolo::OPAND;
              break;

         case 20: //"||"
              tipo= TipoSimbolo::OPOR;
              break;

         case 21:
              tipo= TipoSimbolo::PYCOMA;
              break;

         case 22:
              tipo= TipoSimbolo::COMA;
              break;

         case 23:
              tipo= TipoSimbolo::PARIZQ;
              break;

         case 24:
              tipo= TipoSimbolo::PARDER;
              break;

         case 25:
              tipo= TipoSimbolo::LLAIZQ;
              break;

         case 26:
              tipo= TipoSimbolo::LLADER;
              break;

         case 27:
              tipo= TipoSimbolo::PESOS;
              break;

         default:
              //incluye estado 3 (real mal formado), 5 (cadena sin cerrar),
              //17 ("&" solo), 19 ("|" solo), 28 (caracter no reconocido)
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
        return c== ' ' || c== '\t' || c== '\n' || c== '\r';
   }


   void Lexico::retroceso(){
      if ( c != '$') ind--;
        continua= false;
    }
