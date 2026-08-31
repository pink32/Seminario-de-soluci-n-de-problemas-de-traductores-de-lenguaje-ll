#include "pila.h"
#include <iostream>
using namespace std;

void Pila::push(int valor){
     datos.push_back(valor);
}

int Pila::pop(){
    int valor= datos.back();
    datos.pop_back();
    return valor;
}

int Pila::top(){
    return datos.back();
}

bool Pila::vacia(){
     return datos.empty();
}

void Pila::muestra(){
     cout << "pila: ";
     for (size_t i= 0; i < datos.size(); i++)
         cout << datos[i] << " ";
     cout << endl;
}
