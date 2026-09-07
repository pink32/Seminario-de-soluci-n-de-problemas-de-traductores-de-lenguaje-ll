#include "pila.h"

void Pila::push(ElementoPila *x){
     lista.push_front(x);
}

ElementoPila* Pila::pop(){
    ElementoPila* x= *lista.begin();
    lista.erase( lista.begin() );

    return x;
}

ElementoPila* Pila::top(){
    return *lista.begin();
}

bool Pila::vacia(){
     return lista.empty();
}

void Pila::muestra(){
     list<ElementoPila*>::reverse_iterator it;
     ElementoPila *x;
     cout << "pila: ";

     for (it= lista.rbegin(); it != lista.rend(); it++){
          x= *it;
          x->muestra();
     }

     cout << endl;
}
