#include <iostream>
#include <assert.h>
using namespace std;
#define INF 99999

template <class T>
class Heap
{
    // Nota: MIN Heap
private:
    // array para guardar los elementos
    T *array;
    // guarda la ultima posicion libre
    int ultimoLibre;
    // guarda la maxima capacidad de elementos que puede guardar el heap
    int capacidad;

    // pre: no llamar en root
    // retorna la posicion del padre del elemento que se encuentra en i
    int padre(int i) { return (i - 1) / 2; }
    // retorna el indice del hijo izq
    int izq(int i) { return (2 * i + 1); }
    // retorna el indice del hijo der
    int der(int i) { return (2 * i + 2); }

    void hundir(int i)
    {
        if (izq(i) < ultimoLibre)
        {
            if (der(i) < ultimoLibre)
            {
                if (array[izq(i)] > array[der(i)])
                {
                    if (array[i] > array[der(i)])
                    {
                        cambiarPos(i, der(i));
                        hundir(der(i));
                    }
                }
            }
            if (array[i] > array[izq(i)])
            {
                cambiarPos(i, izq(i));
                hundir(izq(i));
            }
        }
    }

    void flotar(int i)
    {
        if (i != 0)
        {
            if (array[i] < array[padre(i)])
            {
                cambiarPos(i, padre(i));
                flotar(padre(i));
            }
        }
    }

    void cambiarPos(int i, int j)
    {
        T aux = array[j];
        array[j] = array[i];
        array[i] = aux;
    }

public:
    Heap(int capacidad)
    {
        array = new T[capacidad];
        ultimoLibre = 0;
        this->capacidad = capacidad;
    }
    ~Heap() { delete[] array; }
    bool esVacio() { return ultimoLibre == 0; }
    bool esLleno() { return ultimoLibre == capacidad; }
    int getUltimoLibre() { return ultimoLibre; }
    void setUltimoLibre(int valor) { this->ultimoLibre = valor; }
    // pre: no esta lleno
    void insertar(T elemento)
    {
        assert(!esLleno());
        // TODO_1.1
        // wiki: https://es.wikipedia.org/wiki/Mont%C3%ADculo_binario#Inserci%C3%B3n_de_un_elemento (OJO que este es para MAX heap)
        array[ultimoLibre] = elemento;
        ultimoLibre++;
        flotar(ultimoLibre - 1);
    }
    // pre: no esta vacio
    // retorna el menor elemento y lo saca del heap
    T pop()
    {
        /*T aux = array[ultimoLibre];
        array[ultimoLibre] = array[ultimoLibre - 1];
        ultimoLibre--;
        if (ultimoLibre != 0)
            hundir(0);
        return aux;*/

        T aux = array[0];
        array[0] = array[ultimoLibre-1];
        ultimoLibre--;
        if (ultimoLibre!=0)
            hundir(0);
        return aux;
    }
    // pre: no esta vacio
    // retorna el mayor elemento y NO lo saca del heap
    T top()
    {
        assert(!esVacio());
        return array[0];
    }
};

class Asociacion
{
private:
    int origen;  //elemento
    int destino; // numero de array
    int costo;   //indice dentro del array
public:
    Asociacion() {}
    Asociacion(int unOrigen, int unDestino, int unCosto) : origen(unOrigen), destino(unDestino), costo(unCosto) {}
    int getCosto() { return this->costo; };
    int getOrigen() { return this->origen; };
    int getDestino() { return this->destino; };
    bool operator<(Asociacion comp) { return this->getCosto() < comp.getCosto(); }
    bool operator<=(Asociacion comp) { return this->getCosto() <= comp.getCosto(); }
    bool operator>(Asociacion comp) { return this->getCosto() > comp.getCosto(); }
    bool operator>=(Asociacion comp) { return this->getCosto() >= comp.getCosto(); }
    bool operator==(Asociacion comp) { return this->getCosto() == comp.getCosto(); }
};

class NodoLista
{
private:
    int destino;
    int costo;
    NodoLista *sig;

public:
    NodoLista() {}
    NodoLista(int d, int c) : destino(d), costo(c), sig(NULL) {}
    int getDestino() { return this->destino; };
    int getCosto() { return this->costo; };
    NodoLista *getSig() { return this->sig; };
    void insertarOrdenado(NodoLista *&l, int v, int c)
    {
        NodoLista *nuevo = new NodoLista(v, c);
        nuevo->sig = l;
        l = nuevo;
    }
};

template <class V>
class Grafo
{
public:
    NodoLista **listaAdy;
    V *vArrList;        // mapeo de indice interno a vertice
    int max;            // max cantidad de vertices
    int ultimo;         // ultimo disponible (indice interno)
    int cantDeAristas;  // cantidad de aristas ingresadas
    int cantDeVertices; // cantidad de vertices ingresados

    Grafo(int numeroDeVertices)
    {
        this->ultimo = 0;
        this->cantDeAristas = 0;
        this->cantDeVertices = 0;
        this->max = numeroDeVertices;
        this->vArrList = new V[numeroDeVertices];
        this->listaAdy = new NodoLista *[max];
        for (int i = 0; i < max; i++)
        {
            listaAdy[i] = NULL;
        }
    }

    ~Grafo()
    {
        delete[] vArrList;
        for (int i = 0; i < cantDeVertices; i++)
        {
            delete[] listaAdy[i];
        }
        delete[] listaAdy;
    }

    void AniadirVertice(V newV)
    {
        vArrList[ultimo] = newV;
        ultimo++;
        cantDeVertices++;
    }

    void AniadirArista(V origen, V destino, int costo) //Para grafos no dirigidos
    {
        listaAdy[origen - 1]->insertarOrdenado(listaAdy[origen - 1], destino, costo);
        listaAdy[destino - 1]->insertarOrdenado(listaAdy[destino - 1], origen, costo);
        cantDeAristas++;
    }

    void prim()
    {
        int costo = 0;
        Heap<Asociacion> *cola = new Heap<Asociacion>(cantDeAristas);
        bool* visitados = new bool[cantDeVertices];
        for (int j = 0; j < cantDeVertices; j++){
            visitados[j] = false;
        }
        visitados[0] = true;
        NodoLista *aux = listaAdy[0];
        while (aux != NULL){
            Asociacion nuevo(0, aux->getDestino(), aux->getCosto());
            cola->insertar(nuevo);
            aux = aux->getSig();
        }
        while (!cola->esVacio()){
            Asociacion min = cola->pop();
            int posDest = min.getDestino()-1;
            if (!visitados[posDest]){
                visitados[posDest] = true;
                costo += min.getCosto();
                NodoLista *aux = listaAdy[posDest];
                while (aux != NULL){
                    if (!visitados[aux->getDestino() - 1]){
                        Asociacion nuevo(posDest, aux->getDestino(), aux->getCosto());
                        cola->insertar(nuevo);
                    }
                    aux = aux->getSig();
                }
            }
        }
        cout << costo;
    }
};

int main()
{
    int cantVertices;
    int cantAristas;
    int cantPedidos;
    cin >> cantVertices;
    Grafo<int> *grafo = new Grafo<int>(cantVertices);
    for (int j = 0; j < cantVertices; j++)
    {
        grafo->AniadirVertice(j + 1);
    }
    cin >> cantAristas;
    for (int i = 0; i < cantAristas; i++)
    {
        int origen;
        int destino;
        int costo;
        cin >> origen;
        cin >> destino;
        cin >> costo;
        grafo->AniadirArista(origen, destino, costo);
    }
    grafo->prim();
    return 0;
}