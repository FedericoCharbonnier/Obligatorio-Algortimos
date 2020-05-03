#include <iostream>
using namespace std;
#include <assert.h>

template<class T>
struct AVL {
    public:
    T dato;
    int bal;
    AVL* izq;
    AVL* der;
};

bool vario_h = false;

    void InsertarRecursivo(AVL<int>* &pa, const int& x){
        AVL<int> * p1;
        AVL<int> * p2;
        if (pa==NULL){
            pa = new AVL<int>();
            pa->dato = x;
            pa->bal = 0;
            pa-> izq = pa-> der = NULL;
            vario_h = true;
        }
        else if (pa->dato > x){
            InsertarRecursivo(pa->izq, x);
            if (vario_h) {
                switch(pa->bal){
                    case 1:
                        pa->bal = 0;
                        vario_h = false;
                        break;
                    case 0:
                        pa->bal = -1;
                        break;
                    case -1:
                        p1 = pa->izq;
                        if (p1->bal == -1){
                            pa->izq = p1->der;
                            p1->der = pa;
                            pa->bal = 0;
                            pa = p1;
                        } else {
                            p2 = p1->der;
                            p1->der = p2->izq;
                            p2->izq = p1;
                            pa->izq = p2->der;
                            p2->der = pa;
                            pa->bal = p2->bal == -1 ? 1 : 0;
                            p1->bal = p2->bal == 1 ? -1 : 0;
                            pa=p2;
                        }
                        pa->bal = 0;
                        vario_h = false;
                }            
            }
        }
        else if (pa->dato <x) {
            InsertarRecursivo(pa->der, x);
            if (vario_h) {
                switch(pa->bal){
                    case -1:
                        pa->bal = 0;
                        vario_h = false;
                        break;
                    case 0:
                        pa->bal = 1;
                        break;
                    case 1:
                        p1 = pa->der;
                        if (p1->bal == 1){
                            pa->der = p1->izq;
                            p1->izq = pa;
                            pa->bal = 0;
                            pa = p1;
                        } else {
                            p2 = p1->izq;
                            p1->izq = p2->der;
                            p2->der = p1;
                            pa->der = p2->izq;
                            p2->izq = pa;
                            pa->bal = p2->bal == 1 ? -1 : 0;
                            p1->bal = p2->bal == -1 ? 1 : 0;
                            pa=p2;
                        }
                        pa->bal = 0;
                        vario_h = false;
                }            
            }
        }
        else {
            vario_h = false;
        }
    }

    void imprimirRecursivo(AVL<int>* raiz){
        if (!raiz)
            return;
        else{
            imprimirRecursivo(raiz->izq);
            cout << raiz->dato << "\n";
            imprimirRecursivo(raiz->der);
        }
    }


int main() {
    AVL<int>* arbol = new AVL<int>();
    arbol = NULL;
    int cant;
    cin >> cant;
    int dato;
    for (int i = 0 ; i< cant; i++){
        cin >> dato;
        InsertarRecursivo(arbol, dato);
    }
    imprimirRecursivo(arbol);
    return 0;
}