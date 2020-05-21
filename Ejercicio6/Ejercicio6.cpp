#include <iostream>
#include <assert.h>
using namespace std;
#define INF 99999

template <class V>
class Grafo
{
private:
    bool **matrizAdy; // matriz de adyacencia, si matrizAdy[i][j] == true entonces hay una arista i->j
    int **matrizCosto;
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
        this->matrizAdy = new bool *[numeroDeVertices];
        this->matrizCosto = new int *[numeroDeVertices];
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
        }
    }
    ~Grafo()
    {
        delete[] vArrList;
        for (int i = 0; i < this->max; i++)
        {
            delete[] matrizAdy[i];
            delete[] matrizCosto[i];
        }
        delete[] matrizAdy;
        delete[] matrizCosto;
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

    void AniadirArista(V origen, V destino, int costo) //Para grafos no dirigidos
    {
        matrizAdy[this->Pos(origen)][this->Pos(destino)] = true;
        matrizCosto[this->Pos(origen)][this->Pos(destino)] = costo;
        matrizAdy[this->Pos(destino)][this->Pos(origen)] = true;
        matrizCosto[this->Pos(destino)][this->Pos(origen)] = costo;
        cantDeAristas++;
    }

    void EliminarArista(V origen, V destino)
    {
        matrizAdy[this->Pos(origen)][this->Pos(destino)] = false;
        matrizCosto[this->Pos(origen)][this->Pos(destino)] = INF;
        cantDeAristas--;
    }

    void prim(){
        int costo = 0;
        int veces;
        bool *vis = new bool[max];
        for (int i = 0; i < max; i++)
        {
            vis[i] = false;
        }
        veces = 0;
        vis[0] = true;
        int posI;
        int posJ;
        while (veces < max - 1){
            int min = INF;
            posI = -1;
            posJ = -1;
            for (int i = 0; i < max; i++){
                if (vis[i]){
                    for (int j = 0; j < max; j++){
                        if (!vis[j] && matrizAdy[i][j] && min > matrizCosto[i][j]){
                            min = matrizCosto[i][j];
                            posI = i;
                            posJ = j;
                        }
                    }
                }
            }
            costo += matrizCosto[posI][posJ];
            vis[posJ] = true;
            veces++;
        }
        cout << costo;
    }


    void primOtro(int posO){
        int costo =0;
        bool *vis = new bool[max];
        vis[posO] = true;
        // Hago V-1 veces el recorrido
        int min, posI, posJ;

        for (int k = 0; k < max; k++){
            min = INF;
            posI = -1;
            posJ = -1;
            // Recorro los visitados
            for (int i = 0; i < max; i++){
                if (vis[i]){
                    // Recorro los no visitados
                    for (int j = 0; j < max; j++){
                        if (!vis[j] && matrizAdy[i][j] && matrizCosto[i][j] < min){
                            min = matrizCosto[i][j];
                            posI = i;
                            posJ = j;
                        }
                    }
                    if (min == INF){
                        cout << costo;
                        return;
                    }
                    vis[posJ] = true;
                    costo += min;
                }
            }
        }

        delete[] vis;
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
    //grafo->prim(0);
    return 0;
}