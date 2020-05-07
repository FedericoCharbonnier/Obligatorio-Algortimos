#include <iostream>
#include <cassert>
using namespace std;

struct Lista
{
public:
    int dato;
    Lista *sig;
    bool operator< (Lista comp) { return this->dato < comp.dato; }
    bool operator<= (Lista comp) { return this->dato <= comp.dato; }
    bool operator> (Lista comp) { return this->dato > comp.dato; }
    bool operator>= (Lista comp) { return this->dato >= comp.dato; }
    bool operator== (Lista comp) { return this->dato == comp.dato; }

    void Insertar(int valor, Lista* &lista){
        if (!lista){
            lista = new Lista();
            lista->dato = valor;
            lista->sig = NULL;
        }
        else {
            Lista* aux = lista;
            while(aux){
                aux = aux->sig;
            }
            aux = new Lista();
            aux->dato = valor;
            aux->sig = NULL;
        }
    }
};

template <class T>
class Heap
{
    private:
        T * array;
        int ultimoLibre;
        int capacidad;

        // pre: no llamar en root
        // retorna la posicion del padre del elemento que se encuentra en i
        int padre(int i){ return (i - 1) / 2;}
        // retorna el indice del hijo izq
        int izq(int i){ return (2 * i + 1); }
        // retorna el indice del hijo der
        int der(int i){ return (2 * i + 2); }

        void undir(int i) {
            if (izq(i)<ultimoLibre){
                if (der(i)<ultimoLibre){
                    if (array[izq(i)]>array[der(i)]){
                        if (array[i]>array[der(i)]){
                            cambiarPos(i,der(i));
                            undir(der(i));
                        }
                    }
                }
                if (array[i]>array[izq(i)]){
                    cambiarPos(i,izq(i));
                    undir(izq(i));
                }
            }
        }

        void flotar(int i) {
            if (i!=0){
                if (array[i]<array[padre(i)]){
                    cambiarPos(i,padre(i));
                    flotar(padre(i));
                }
            }
        }

        void cambiarPos(int i, int j) {
            T aux = array[j];
            array[j] = array[i];
            array[i] = aux;
        }

    public:
        Heap(int capacidad) {
            array = new T[capacidad];
            ultimoLibre = 0;
            this->capacidad = capacidad;
        }
        ~Heap() { delete[] array; }
        bool esVacio() { return ultimoLibre == 0; }
        bool esLleno() { return ultimoLibre == capacidad; }

        // pre: no esta lleno
        void insertar(T elemento) {
            assert(!esLleno());
            array[ultimoLibre] = elemento;
            ultimoLibre++;
            flotar(ultimoLibre-1); 
        }

        // pre: no esta vacio
        // retorna el mayor elemento y lo saca del heap
        T pop() {
            assert(!esVacio());
            T aux = array[0];
            if(array[0]->sig){
                array[0] = array[0]->sig;
                undir(0);
            }
            else {
                array[0] = array[ultimoLibre-1];
                ultimoLibre--;
            if (ultimoLibre!=0)
                undir(0);
            }
            return aux;
        }


        // pre: no esta vacio
        // retorna el mayor elemento y NO lo saca del heap
        T top() {
            assert(!esVacio());
            return array[0];
        }

        T pos(int i) {
            assert(!esVacio());
            return array[i];
        }
};

int main(){
    int cantListas; //Cantidad de listas
    int maxDeLista; //Maximo de una lista
    int valor;
    cin >> cantListas;
    Heap<Lista*> *heap = new Heap<Lista*>(cantListas); 
    for(int i=0; i < cantListas; i++){
        Lista *lista = new Lista();
        lista = NULL;
        cin >> maxDeLista;
        for(int j=0; j < maxDeLista; j++){
            cin >> valor;
            lista->Insertar(valor, lista);
        }
        heap->insertar(lista);
    }
    for(int i=0; i<cantListas; i++){
        cout << heap->pos(i)->dato << endl;
    }
    //while(!heap->esVacio()){
    //    cout << heap->pop()->dato << endl;
    //}
    return 0;
}