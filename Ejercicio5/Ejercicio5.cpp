#include <iostream>
#include <assert.h>
using namespace std;
#define INF 99999

/*class Par
{
private:
    int vertice;
    int costo;

public:
    Par() {}
    Par(int unVertice, int unCosto) : vertice(unVertice), costo(unCosto) {}
    int getCosto() { return this->costo; };
    bool getVertice() { return this->vertice; };
    void setCosto(int unCosto) { this->costo = unCosto; };
    void setVertice(int unVertice) { this->vertice = unVertice; };
};*/

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
    int clave; //prioridad
    int index; //elemento
public:
    Asociacion() {}
    Asociacion(int unaClave, int unIndice) : clave(unaClave), index(unIndice) {}
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
//    bool **matrizAdy; // matriz de adyacencia, si matrizAdy[i][j] == true entonces hay una arista i->j
//    int **matrizCosto;
    NodoLista **listaAdy;
    V *vArrList;        // mapeo de indice interno a vertice
    int max;            // max cantidad de vertices
    int ultimo;         // ultimo disponible (indice interno)
    int cantDeAristas;  // cantidad de aristas ingresadas
    int cantDeVertices; // cantidad de vertices ingresados
    //Heap<Asociacion<int>> *heap;

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
        //this->heap = new Heap<Asociacion<int>>(max);
        /*  this->matrizAdy = new bool *[numeroDeVertices];
        this->matrizCosto = new int *[numeroDeVertices];
        //this->visitados = new bool[max];
        for (int i = 0; i < this->max; i++)
        {
            matrizAdy[i] = new bool[numeroDeVertices];
            matrizCosto[i] = new int[numeroDeVertices];
        }
        for (int i = 0; i < this->max; i++)
        {
            for (int j = 0; j < this->max; j++)
            {
                matrizAdy[i][j] = false;
                matrizCosto[i][j] = INF;
            }
        }*/
    }
    ~Grafo()
    {
        delete[] vArrList;
       /* for (int i = 0; i < this->max; i++)
        {
            delete[] matrizAdy[i];
            delete[] matrizCosto[i];
        }
        delete[] matrizAdy;
        delete[] matrizCosto;*/
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
       /* matrizAdy[this->Pos(origen)][this->Pos(destino)] = true;
        matrizCosto[this->Pos(origen)][this->Pos(destino)] = costo;*/
        listaAdy[origen-1]->insertarOrdenado(listaAdy[origen-1], destino, costo);
        cantDeAristas++;
    }

    /*void EliminarArista(V origen, V destino)
    {
        matrizAdy[this->Pos(origen)][this->Pos(destino)] = false;
        matrizCosto[this->Pos(origen)][this->Pos(destino)] = INF;
        cantDeAristas--;
    }*/

    int *dijkstra(int posO)
    {
        // Creo vectores auxiliares
        int *dist = new int[max];
        int *ant = new int[max];
        bool *vis = new bool[max];

        // Inicializo los vectores
        for (int i = 0; i < max; dist[i] = INF, ant[i] = -1, vis[i] = false, i++)
            ;

        // Proceso al origen
        vis[posO] = true;
        dist[posO] = 0;
        NodoLista* aux = listaAdy[posO];
        while (aux != NULL)
        {
            int v = aux->getDestino() -1;
            ant[v] = posO;
            dist[v] = aux->getCosto();
            aux = aux->getSig();
        }

        // Evalúo a todos los demás vértices
        for (int k = 1; k < max; k++)
        {
            // Selecciono el vértice de menor distancia no visitado
            int posMin = -1;
            int min = INF;
            for (int i = 0; i < max; i++)
            {
                if (!vis[i] && dist[i] < min)
                {
                    posMin = i;
                    min = dist[i];
                }
            }
            // Visito al vértice intermedio
            if (posMin == -1)
                break;
            vis[posMin] = true;
            aux = listaAdy[posMin];
            while (aux != NULL)
            {
                int j = aux->getDestino() -1;
                if (!vis[j]){
                    int nuevoCosto = dist[posMin] + aux->getCosto();
                    if (nuevoCosto < dist[j]){
                        dist[j] = nuevoCosto;
                        ant[j] = posMin;
                    }
                }
                aux = aux->getSig();
            }
            // Evaluo adyacentes
            /*for (int j = 0; j < max; j++)
            {
                // Actualizo la distancia y el anterior en caso que un adyacente tenga distancia acumulada mejor a la actual
                if (!vis[j] && matrizAdy[posMin][j])
                {
                    int nuevoCosto = dist[posMin] + matrizCosto[posMin][j];
                    if (nuevoCosto < dist[j])
                    {
                        dist[j] = nuevoCosto;
                        ant[j] = posMin;
                    }
                }
            }*/
        }

        delete[] ant;
        delete[] vis;
        return dist;
    }

    void imprmirVector(int *vector, int pos)
    {
        for (int i = 0; i < this->cantDeVertices; i++)
        {
            if (i + 1 == pos || vector[i] == INF)
            {
                cout << -1 << endl;
            }
            else
            {
                cout << vector[i] << endl;
            }
        }
    }

    void displayHash() { 
    for (int i = 0; i < max; i++) { 
      cout << i+1; 
      NodoLista* aux = listaAdy[i];
      while (aux!=NULL) {
        cout << " --> " << aux->getDestino() << "-" << aux->getCosto(); 
        aux= aux->getSig();
      }
      cout << endl; 
    }
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
    cin >> cantPedidos;
    Heap<Asociacion> *cola = new Heap<Asociacion>(cantPedidos);
    for (int i = 0; i < cantPedidos; i++)
    {
        int insertar;
        cin >> insertar;
        Asociacion nuevo(i + 1, insertar);
        cola->insertar(nuevo);
    }

    for (int i = 0; i < cantPedidos; i++)
    {
        int pos = cola->pop().getIndex();
        grafo->imprmirVector(grafo->dijkstra(pos - 1), pos);
    }
    return 0;
}