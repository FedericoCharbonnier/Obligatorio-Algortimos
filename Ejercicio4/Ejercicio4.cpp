#include <iostream>
#include <assert.h>
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

/*template <class V>
struct Lista
{
public:
    V dato;
    Lista<V> *sig;

    void Insertar(V valor, Lista<V> *&lista)
    {
        if (!lista)
        {
            lista = new Lista<V>();
            lista->dato = valor;
            lista->sig = NULL;
        }
        else
        {
            Lista<V> *aux = lista;
            while (aux)
            {
                aux = aux->sig;
            }
            aux = new Lista<V>();
            aux->dato = valor;
            aux->sig = NULL;
        }
    }
};*/

template <class V>
class Grafo
{
private:
    bool **matrizAdy; // matriz de adyacencia, si matrizAdy[i][j] == true entonces hay una arista i->j
    V *vArrList;      // mapeo de indice interno a vertice
    int *nivel;
    int max;            // max cantidad de vertices
    int ultimo;         // ultimo disponible (indice interno)
    int cantDeAristas;  // cantidad de aristas ingresadas
    int cantDeVertices; // cantidad de vertices ingresados
    //bool *visitados;
    Heap<Asociacion<int>> *heap;

public:
    Grafo(int numeroDeVertices)
    {
        this->ultimo = 0;
        this->cantDeAristas = 0;
        this->cantDeVertices = 0;
        this->max = numeroDeVertices;
        this->heap = new Heap<Asociacion<int>>(max);
        this->vArrList = new V[numeroDeVertices];
        this->matrizAdy = new bool *[numeroDeVertices];
        //this->visitados = new bool[max];
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
        this->nivel = new int[max];
        for (int i = 0; i < this->max; i++)
        {
            nivel[i] = 0;
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
        assert(this->ultimo < this->max);
        vArrList[ultimo] = newV;
        ultimo++;
        cantDeVertices++;
    }
    void AniadirArista(V origen, V destino)
    {
        matrizAdy[this->Pos(origen)][this->Pos(destino)] = true;
        cantDeAristas++;
    }

    void EliminarArista(V origen, V destino)
    {
        matrizAdy[this->Pos(origen)][this->Pos(destino)] = false;
        cantDeAristas--;
    }

    // Retorna una lista de V desde las que se puede llegar desde el vertice origen (parametro)
    /*Lista<V> Anteriores(V &origen){
        Lista<V> *ret = new Lista<V>();
        ret = NULL;
        for (int i = 0; i< this->cantDeVertices; i++){
            if(this->matrizAdy[i][this->Pos(origen)]){
                ret->Insertar(this->vArrList[i], ret);
            }
        }
                return ret;
    }*/

    int *obtenerCantIncidentes()
    {
        int *vec = new int[max];
        for (int j = 0; j < max; j++)
        {
            int cont = 0;
            for (int i = 0; i < max; i++)
            {
                if (this->matrizAdy[i][j])
                {
                    cont++;
                }
            }
            vec[j] = cont;
        }
        return vec;
    }

    int obtenerPosSinIncidentes(int *cantIncidentes)
    {
        for (int i = 0; i < this->max; i++)
            if (cantIncidentes[i] == 0)
                return i;
        return -1;
    }

    void ImprimirOrdenTopologico()
    {
        int *cantIncidentes = obtenerCantIncidentes();
        for (int i = 0; i < max; i++)
        {
            int posSinIncidentes = obtenerPosSinIncidentes(cantIncidentes);
            cantIncidentes[posSinIncidentes] = -1;
            for (int j = 0; j < max; j++)
            {
                if (matrizAdy[posSinIncidentes][j])
                {
                    cantIncidentes[j]--;
                    int aux = nivel[posSinIncidentes] + 1;
                    if (nivel[j] < aux)
                      nivel[j] = aux;
                }
            }
            Asociacion<int> nuevo(nivel[posSinIncidentes], vArrList[posSinIncidentes]);
            this->heap->insertar(nuevo);
        }
        for (int k = 0; k < this->max; k++)
        {
            cout << this->heap->pop().getDato() << endl;
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
        grafo->AniadirVertice(j+1);
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
    grafo->ImprimirOrdenTopologico();
    return 0;
}
