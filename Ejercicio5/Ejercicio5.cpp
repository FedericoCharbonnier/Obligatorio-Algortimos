#include <iostream>
#include <assert.h>
using namespace std;
#define INF 99999

class NodoLista
{
private:
    int destino;
    int costo;
    NodoLista* sig;
public:
    
    NodoLista() {}
    NodoLista(int d, int c) : destino(d), costo(c), sig(NULL) {}
    int getDestino() { return this->destino; };
    int getCosto() { return this->costo; };
    NodoLista* getSig() {return this->sig;};
    void insertarOrdenado(NodoLista* &l, int v , int c){
            NodoLista* nuevo = new NodoLista(v,c);
            nuevo->sig = l;
            l = nuevo;    
    }
};

class Asociacion
{
private:
    int clave; //costo
    int index; //origen
public:
    Asociacion() {}
    Asociacion(int unaClave, int unIndice) : clave(unaClave), index(unIndice){}
    int getClave() { return this->clave; };
    int getIndex() { return this->index; };
    bool operator<(Asociacion comp) { return this->getClave() < comp.getClave(); }
    bool operator<=(Asociacion comp) { return this->getClave() <= comp.getClave(); }
    bool operator>(Asociacion comp) { return this->getClave() > comp.getClave(); }
    bool operator>=(Asociacion comp) { return this->getClave() >= comp.getClave(); }
    bool operator==(Asociacion comp) { return this->getClave() == comp.getClave(); }
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
class Grafo
{
private:
    NodoLista **listaAdy;
    V *vArrList;        // mapeo de indice interno a vertice
    int max;            // max cantidad de vertices
    int ultimo;         // ultimo disponible (indice interno)
    int cantDeAristas;  // cantidad de aristas ingresadas
    int cantDeVertices; // cantidad de vertices ingresados

public:
    Grafo(int numeroDeVertices)
    {
        this->ultimo = 0;
        this->cantDeAristas = 0;
        this->cantDeVertices = 0;
        this->max = numeroDeVertices;
        this->vArrList = new V[numeroDeVertices];
        this->listaAdy = new NodoLista*[max];
        for (int i = 0; i<max; i++) {
            listaAdy[i] = NULL;
        }
    }
    ~Grafo()
    {
        delete[] vArrList;
        delete[] listaAdy;
    }

    int Pos(V vertice)
    {
        for (int i = 0; i < this->cantDeVertices; i++)
        {
            if (this->vArrList[i] == vertice)
            {
                return i;
            }
        }
        return -1;
    }

    void AniadirVertice(V newV)
    {
        vArrList[ultimo] = newV;
        ultimo++;
        cantDeVertices++;
    }
    void AniadirArista(V origen, V destino, int costo)
    {
        listaAdy[origen-1]->insertarOrdenado(listaAdy[origen-1], destino, costo);
        cantDeAristas++;
    }

    void dijkstra(int posO){
        Heap<Asociacion>* cola = new Heap<Asociacion>(cantDeAristas);
        int *dist = new int[max];
        bool *vis = new bool[max];

        for (int i = 0; i < max; dist[i] = INF, vis[i] = false, i++);

        vis[posO] = true;
        dist[posO] = 0;
        NodoLista* aux = listaAdy[posO];
        while (aux != NULL)
        {
            Asociacion a(aux->getCosto(), aux->getDestino() - 1);
            cola->insertar(a); 
            aux = aux->getSig();
        }
        
        while(!cola->esVacio()){
            Asociacion ayuda = cola->pop();
            if(!vis[ayuda.getIndex()]){
                if (ayuda.getClave() <= dist[ayuda.getIndex()]){
                    dist[ayuda.getIndex()] = ayuda.getClave();
                }
                aux = listaAdy[ayuda.getIndex()];
                while (aux != NULL){
                    Asociacion nuevo(aux->getCosto() +  dist[ayuda.getIndex()], aux->getDestino() - 1);
                    cola->insertar(nuevo);
                    aux = aux->getSig();
                }
                vis[ayuda.getIndex()] = true;
            }
        }
        
        imprmirVector(dist);
        delete[] vis;
        delete[] dist;
    }

    void imprmirVector(int *vector){
        for (int i = 0; i < this->cantDeVertices; i++){
            if (vector[i] == INF || vector[i] == 0){
                cout << -1 << endl;
            }
            else{
                cout << vector[i] << endl;
            }
        }
    }

    /*void displayHash() { 
    for (int i = 0; i < max; i++) { 
      cout << i+1; 
      NodoLista* aux = listaAdy[i];
      while (aux!=NULL) {
        cout << " --> " << aux->getDestino() << "-" << aux->getCosto(); 
        aux= aux->getSig();
      }
      cout << endl; 
    }
  }*/ 
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
    cin >> cantPedidos;
    for (int k=0; k<cantPedidos;k++){
        int insertar;
        cin >> insertar;
        grafo->dijkstra(insertar-1);
    }
    return 0;
}