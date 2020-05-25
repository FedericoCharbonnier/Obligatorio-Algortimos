#include <iostream>
#include <assert.h>
using namespace std;

class NodoLista
{
private:
    int destino;
    NodoLista *sig;

public:
    NodoLista() {}
    NodoLista(int d) : destino(d), sig(NULL) {}
    int getDestino() { return this->destino; };
    NodoLista *getSig() { return this->sig; };
    void insertarOrdenado(NodoLista *&l, int v)
    {
        NodoLista *nuevo = new NodoLista(v);
        nuevo->sig = l;
        l = nuevo;
    }
};

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
    // retorna el mayor elemento y lo saca del heap
    T pop()
    {
        T aux = array[0];
        array[0] = array[ultimoLibre - 1];
        ultimoLibre--;
        if (ultimoLibre != 0)
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

template <class V>
class Asociacion
{
private:
    int clave; //nivel
    V dato;    //vertice
public:
    Asociacion() {}
    Asociacion(int unaClave, V unDato) : clave(unaClave), dato(unDato) {}
    int getClave() { return this->clave; };
    V getDato() { return this->dato; };
    bool operator<(Asociacion comp)
    {
        if (this->getClave() == comp.getClave())
        {
            return this->getDato() < comp.getDato();
        }
        else
        {
            return this->getClave() < comp.getClave();
        }
    }

    bool operator>(Asociacion comp)
    {
        if (this->getClave() == comp.getClave())
        {
            return this->getDato() > comp.getDato();
        }
        else
        {
            return this->getClave() > comp.getClave();
        }
    }

    bool operator<=(Asociacion comp)
    {
        if (this->getClave() == comp.getClave())
        {
            return this->getDato() <= comp.getDato();
        }
        else
        {
            return this->getClave() <= comp.getClave();
        }
    }

    bool operator>=(Asociacion comp)
    {
        if (this->getClave() == comp.getClave())
        {
            return this->getDato() >= comp.getDato();
        }
        else
        {
            return this->getClave() >= comp.getClave();
        }
    }
    bool operator==(Asociacion comp) { return (this->getClave() == comp.getClave()) && (this->getDato() == comp.getDato()); }
};

class Grafo
{
private:
    NodoLista **listaAdy;
    int* vArrList; // mapeo de indice interno a vertice
    int* nivel;
    int* vec;           //vector de incidencias
    int max;            // max cantidad de vertices
    int ultimo;         // ultimo disponible (indice interno)
    int cantDeAristas;  // cantidad de aristas ingresadas
    int cantDeVertices; // cantidad de vertices ingresados

public:
    NodoLista** getListaAdy() { return this->listaAdy; };
    int* getVArrList() { return this->vArrList; };
    int* getNivel() { return this->nivel; };
    int* getVec() { return this->vec; };
    int getMax() { return this->max; };
    
    Grafo(int numeroDeVertices)
    {
        this->ultimo = 0;
        this->cantDeAristas = 0;
        this->cantDeVertices = 0;
        this->max = numeroDeVertices;
        this->vArrList = new int[numeroDeVertices];
        this->listaAdy = new NodoLista *[max];
        for (int i = 0; i < max; i++)
        {
            listaAdy[i] = NULL;
        }
        this->nivel = new int[max];
        for (int i = 0; i < this->max; i++)
        {
            nivel[i] = 0;
        }
        this->vec = new int[max];
        for (int k = 0; k < this->max; k++)
        {
            vec[k] = 0;
        }
    }
    ~Grafo()
    {
        delete[] vArrList;
        delete[] vec;
        delete[] listaAdy;
        delete[] nivel;
    }

    void AniadirVertice(int newV)
    {
        assert(this->ultimo < this->max);
        vArrList[ultimo] = newV;
        ultimo++;
        cantDeVertices++;
    }
    
    void AniadirArista(int origen, int destino)
    {
        listaAdy[origen - 1]->insertarOrdenado(listaAdy[origen - 1], destino); 
        vec[destino-1]++;
        cantDeAristas++;
    }
};

    bool *obtenerPosSinIncidentes(int *cantIncidentes, int largo){
        bool *ret = new bool[largo];
        for (int i = 0; i < largo; i++){
            if (cantIncidentes[i] == 0)
                ret[i] = true;
        }
        return ret;
    }

    void ImprimirOrdenTopologico(Grafo* &grafo, int largo){
        Heap<Asociacion<int>> *heap = new Heap<Asociacion<int>>(largo);
        bool* posSinIncidentes = obtenerPosSinIncidentes(grafo->getVec(), largo);
        for (int i=0; i<largo; i++) {
            if (posSinIncidentes[i]){
                Asociacion<int> nuevo(grafo->getNivel()[i], grafo->getVArrList()[i]);
                heap->insertar(nuevo);
            }
        }
        while (!heap->esVacio()){
            
            int procesar = heap->pop().getDato();
            cout << procesar << endl;
            NodoLista *aux = grafo->getListaAdy()[procesar-1];
            while (aux != NULL){
                int dest = aux->getDestino() - 1;
                grafo->getVec()[dest]--;
                int ayuda = grafo->getNivel()[procesar-1] + 1;
                if (grafo->getNivel()[dest] < ayuda)
                    grafo->getNivel()[dest] = ayuda;
                if (grafo->getVec()[dest] == 0){
                    Asociacion<int> nuevo(grafo->getNivel()[dest], grafo->getVArrList()[dest]);
                    heap->insertar(nuevo);
                }
                aux = aux->getSig();
            }
        }
    }

     

int main()
{
    int cantVertices;
    int cantAristas;
    cin >> cantVertices;
    Grafo *grafo = new Grafo(cantVertices);
    for (int j = 0; j < cantVertices; j++)
    {
        grafo->AniadirVertice(j + 1);
    }
    cin >> cantAristas;
    for (int i = 0; i < cantAristas; i++)
    {
        int origen;
        int destino;
        cin >> origen;
        cin >> destino;
        grafo->AniadirArista(origen, destino);
    }
    ImprimirOrdenTopologico(grafo,cantVertices);
    return 0;
}
