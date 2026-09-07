#ifndef _PILA
#define _PILA

#include <iostream>
#include <string>
#include <list>
using namespace std;

// Clase base: no se instancia directamente, solo sirve para que la Pila
// pueda guardar cualquiera de sus 3 clases hijas en un mismo contenedor.
class ElementoPila{
      protected:
       int valor; //el mismo entero que antes se guardaba directo en la pila

      public:
       int getValor(){ return valor; }
       virtual void muestra(){}
};

// Un terminal de la gramatica (identificador, +, $), guarda ademas el
// texto real que reconocio el lexico (ej. "a", "+"), para que al imprimir
// la pila se vea igual que en un analisis hecho a mano.
class Terminal: public ElementoPila{
      private:
       string simbolo;

      public:
       Terminal(int valor, string simbolo){
            this->valor= valor;
            this->simbolo= simbolo;
       }

       void muestra(){
            cout << simbolo << " ";
       }
};

// Un no terminal de la gramatica (en esta practica, solo E).
class NoTerminal: public ElementoPila{
      private:
       string nombre;

      public:
       NoTerminal(int valor, string nombre){
            this->valor= valor;
            this->nombre= nombre;
       }

       void muestra(){
            cout << nombre << " ";
       }
};

// Un estado del automata LR (los numeros que antes se intercalaban en la pila de enteros).
class Estado: public ElementoPila{
      public:
       Estado(int valor){
            this->valor= valor;
       }

       void muestra(){
            cout << valor << " ";
       }
};


class Pila{
      private:
       list<ElementoPila*> lista;

      public:
       void push(ElementoPila *x);
       ElementoPila* pop();
       ElementoPila* top();
       bool vacia();
       void muestra();
};

#endif
