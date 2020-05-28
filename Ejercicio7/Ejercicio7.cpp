#include <iostream>
#include <cassert>
#define INF 99999;
using namespace std;

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
        /*int array = aux->getArray();
            int index = aux->getIndex();
            if (index == vecLength(vec[array])-1){
                T nuevo(INT_MAX, array, index+1);
                array[0] = this->insertar(nuevo);
                ultimoLibre--;
            } else {
                T nuevo(vec[array][index], array, index+1);
                array[0] = this->insertar(nuevo);
            }*/
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
    int origen;
    int destino;

public:
    Asociacion() {}
    Asociacion(int unOrigen, int unDestino) : origen(unOrigen), destino(unDestino) {}
    int getOrigen() { return this->origen; };
    int getDestino() { return this->destino; };
    bool operator<(Asociacion comp) { return this->getOrigen() < comp.getOrigen(); }
    bool operator<=(Asociacion comp) { return this->getOrigen() <= comp.getOrigen(); }
    bool operator>(Asociacion comp) { return this->getOrigen() > comp.getOrigen(); }
    bool operator>=(Asociacion comp) { return this->getOrigen() >= comp.getOrigen(); }
    bool operator==(Asociacion comp) { return this->getOrigen() == comp.getOrigen(); }
};

template <class V>
class Grafo
{
private:
    bool **matrizAdy; // matriz de adyacencia, si matrizAdy[i][j] == true entonces hay una arista i->j
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
        for (int i = 0; i < this->max; i++)
        {
            matrizAdy[i] = new bool[numeroDeVertices];
        }
        for (int i = 0; i < this->max; i++)
        {
            for (int j = 0; j < this->max; j++)
            {
                matrizAdy[i][j] = false;
            }
        }
    }
    ~Grafo()
    {
        delete[] vArrList;
        for (int i = 0; i < this->max; i++)
        {
            delete[] matrizAdy[i];
        }
        delete[] matrizAdy;
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
    void AniadirArista(V origen, V destino)
    {
        matrizAdy[origen-1][destino-1] = true;
        cantDeAristas++;
    }

    void EliminarArista(V origen, V destino)
    {
        matrizAdy[this->Pos(origen)][this->Pos(destino)] = false;
        cantDeAristas--;
    }

    void kruskal()
    {
        int cantTrue = 0;
        // La asociacion se usa con el dominio como el origen y el rango como el destino
        //   -> Evitar utilizar operadores ==, ya que no aplica!
        Heap<Asociacion> *cp = new Heap<Asociacion>(cantDeAristas);
        for (int i = 0; i < max; i++)
        {
            for (int j = 0; j < max; j++)
            {
                if (matrizAdy[i][j])
                {
                    Asociacion a(i, j);
                    cp->insertar(a);
                }
            }
        }
        // inicializar la estructura de las componentes conexas, creando un vector para cada uno, conteniendo un sólo número.
        bool **compConexas = new bool*[cantDeVertices]();
        for (int i = 0; i < cantDeVertices; i++)
        {
            compConexas[i] = new bool[cantDeVertices];
            for (int j = 0; j < cantDeVertices; j++)
            {
                if (i == j)
                {
                    compConexas[i][j] = true;
                }
                else
                {
                    compConexas[i][j] = false;
                }
            }
        }

        int cantAristas = 0, posOri, posDes;
        
        while (cantAristas < max - 1 && !cp->esVacio())
        {
            Asociacion a = cp->pop();
            posOri = -1;
            posDes = -1;
            int columnaDestino = 0;
            for (int i = 0; i < cantDeVertices; i++){ //Encuentro la columna del vertice Destino
                if (compConexas[i][a.getDestino()])
                {
                    columnaDestino = i;
                }
            }

            for (int j = 0; j < cantDeVertices; j++)
            {
                if (compConexas[columnaDestino][j])
                {
                    compConexas[columnaDestino][j] = false;
                    compConexas[a.getOrigen()][j] = true;
                }
            }
            cantAristas++;

        }
        for (int i = 0; i < cantDeVertices; i++)
            {
                if (compConexas[i][i])
                {
                    cantTrue++;
                }
            }
        //cout << cantTrue;
        for (int i = 0; i < cantDeVertices; i++){
            for (int j = 0; j < cantDeVertices; j++){
                if(compConexas[i][j]){
                    cout << "T ";
                }
                else{
                    cout << "F ";
                }
            
            }
            cout << endl;
        }   
     }
};

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
        int costo;
        cin >> origen;
        cin >> destino;
        grafo->AniadirArista(origen, destino);
    }
    grafo->kruskal();
    return 0;
}