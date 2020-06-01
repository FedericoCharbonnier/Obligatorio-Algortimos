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

    void AniadirArista(V origen, V destino)
    {
        listaAdy[origen - 1]->insertarOrdenado(listaAdy[origen - 1], destino);
        cantDeAristas++;
    }
};

bool ciclo = false;

void dfsRec(Grafo<int>* g, int vertice, bool *&vis, bool *&encolados){
    vis[vertice] = true;
    encolados[vertice] = true;
    NodoLista* aux = g->listaAdy[vertice];
    while(aux != NULL){
        if (!vis[aux->getDestino()-1]){
            dfsRec(g, aux->getDestino()-1, vis, encolados);
        } else {
            if (encolados[aux->getDestino()-1]){
                ciclo = true;
                return;
            }
        }
        aux = aux->getSig();
    }
    encolados[vertice] = false;
}

void dfs(Grafo<int>* g, int posO)
{
    bool *vis = new bool[g->cantDeVertices];
    bool *enCola = new bool[g->cantDeVertices]; 
    for (int i = 0; i < g->max; vis[i] = false, enCola[i] = false, i++);
    dfsRec(g, posO, vis, enCola);
    delete[] vis;
    delete[] enCola;
    if (ciclo)
        cout << "1";
    else
        cout << "0";
    
}

int main()
{
    int cantVertices;
    int cantAristas;
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
        cin >> origen;
        cin >> destino;
        grafo->AniadirArista(origen, destino);
    }
    dfs(grafo, 0);
    return 0;
}