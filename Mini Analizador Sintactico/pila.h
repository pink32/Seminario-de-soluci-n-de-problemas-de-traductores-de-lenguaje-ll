#ifndef _PILA
#define _PILA

#include <vector>
using namespace std;

class Pila{
      private:
       vector<int> datos;

      public:
       void push(int valor);
       int pop();
       int top();
       bool vacia();
       void muestra();
};

#endif
