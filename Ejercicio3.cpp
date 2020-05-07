#include <iostream>
#include <cassert>
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
    int getUltimoLibre() {return ultimoLibre; }
    void setUltimoLibre(int valor) {this->ultimoLibre = valor; }
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
    T pop(int **vec)
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
    int clave; //elemento
    int array; // numero de array
    int index; //indice dentro del array
public:
    Asociacion() {}
    Asociacion(int unaClave, int unArray, int unIndice) : clave(unaClave), array(unArray), index(unIndice) {}
    int getClave() { return this->clave; };
    int getArray() { return this->array; };
    int getIndex() { return this->index; };
    bool operator<(Asociacion comp) { return this->getClave() < comp.getClave(); }
    bool operator<=(Asociacion comp) { return this->getClave() <= comp.getClave(); }
    bool operator>(Asociacion comp) { return this->getClave() > comp.getClave(); }
    bool operator>=(Asociacion comp) { return this->getClave() >= comp.getClave(); }
    bool operator==(Asociacion comp) { return this->getClave() == comp.getClave(); }
};

int main()
{
    int cantVec;
    cin >> cantVec;
    Heap<Asociacion> *cola = new Heap<Asociacion>(cantVec);
    int **vectores = new int *[cantVec];
    int* vecLength = new int[cantVec];
    int largoVec;
    for (int i = 0; i < cantVec; i++)
    {
        cin >> largoVec;
        vecLength[i] = largoVec;
        vectores[i] = new int[largoVec];
        for (int j = 0; j < largoVec; j++)
        {
            int dato;
            cin >> dato;
            vectores[i][j] = dato;
            if (j == 0)
            {
                Asociacion nuevo(dato, i, j);
                cola->insertar(nuevo);
            }
        }
    }

    while (!cola->esVacio())
    {
        Asociacion aux = cola->pop(vectores);
        int array = aux.getArray();
        int index = aux.getIndex();
        if (index == vecLength[array] - 1)
        {
            Asociacion nuevo(INT_MAX, array, index + 1);
            cola->insertar(nuevo);
            cola->setUltimoLibre(cola->getUltimoLibre()-1);
        }
        else
        {
            Asociacion nuevo(vectores[array][index+1], array, index + 1);
            cola->insertar(nuevo);
        }
        cout << aux.getClave() << endl;
    }
    for (int k = 0; k < cantVec; k++)
    {
        delete[] vectores[k];
    }
    delete[] vectores;
    delete[] vecLength;
    return 0;
}
