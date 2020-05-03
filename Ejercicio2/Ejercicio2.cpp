#include <iostream>
using namespace std;
#include <assert.h>

template <class K, class V>
class ClaveValor{
    private:
        K clave;
        V valor;
    public:
        ClaveValor(K &unaClave, V &unValor): clave(unaClave), valor(unValor) {} // Constructor con ambos datos
        ClaveValor(K &unaClave): clave(unaClave) {} // Constructor con solo la clave
        K getClave() {return this->clave;}
        V getValor() {return this->valor;}
        void setClave(K nuevaClave) {this->clave = nuevaClave;}
        void setValor(V nuevoValor) {this->valor = nuevoValor;}
        bool operator==(ClaveValor otraCV) {return this->getClave() == otraCV.getClave(); } // "delego" la igualacion a las claves. Dos ClaveValor son iguales si sus claves son iguales
        bool operator==(K unaClave) {return this->getClave() == unaClave; } // comparador entre ClaveValor y K
};

// Nodo Lista simplemente encadenado
template <class T>
struct NodoLista{
    public:
        T dato; // guarda el dato en si dentro del nodo
        NodoLista<T> *sig; // guarda la referencia al siguiente nodo
        NodoLista(T &unDato, NodoLista<T> *unSiguiente): dato(unDato), sig(unSiguiente) {} // Constructor con ambos datos
        NodoLista(T &unDato): dato(unDato), sig(0) {} // Constructor con solo el dato, por defecto el sig es null
}; 

template <class K, class V>
class TablaHash {
    private:
        // La "Lista" en cada casillero sera un puntero a un NodoLista de ClaveValor
        typedef NodoLista<ClaveValor<K,V>> *NodoPtr;
        // Array principal de la estructura, cada casillero es un puntero a el nodo mencionado anteriormente
        NodoPtr *arrList;
        // Guardamos la funcion hash (ver constructor). Notar que la funcion recibe un K (clave) y retorna un int
        int (*fnHash)(K);
        // tamanio del array
        int tamanio;
        // funcion que dada un clave nos devuelve la posicion en el array donde se esta buscando/insertando/etc
        int posHash(K clave) {
            // usando la funcion fnHash y el tamanio del array se obtiene la posicion.
            // Nota: fnHash puede retornar un numero fuera del array por lo cual se aplica el modulo con el tamanio del array.
            int pos = abs(this->fnHash(clave)) % this->tamanio;
            return pos;
        }
        // funcion que me calcula el siguiente primo.
        int siguientePrimo(int origen) {
          return isPrime(++origen) ? origen : siguientePrimo(origen);
        }
        bool isPrime(int num){
          assert(num>=1);
          if(num==1){
            return false;
          } else {
            for(int i=2; i<num/2; i++){
              if(num%i==0){
                return false;
              }
            }
            return true;
          }
        }
        // funcion recursiva una vez que se encuentra el casillero correspondiente. Observar: void Insertar(K clave,V el)
        void insertarRecursivo(NodoPtr &nodo, K clave, V valor) {
            if(nodo == 0) {
                ClaveValor<K,V> aInsertar(clave,valor);
                nodo = new NodoLista<ClaveValor<K,V>>(aInsertar);
            }else if(nodo->dato == clave) {
                // Si la clave ya se encuentra dentro de la tabla entonces la sustituyo por el nuevo valor
                nodo->dato.setValor(valor);
            }else {
                // Paso recursivo
                insertarRecursivo(nodo->sig, clave, valor);
            }
        }
        V recuperarRecursivo(NodoPtr nodo,K clave) {
            assert(nodo != 0);
            if(nodo->dato == clave) {
                return nodo->dato.getValor();
            }else {
                // Paso recursivo
                return recuperarRecursivo(nodo->sig, clave);
            }
        }

        bool existeRecursivo(NodoPtr nodo, K clave){
          if (!nodo) {
            return false;
          }
          if(nodo->dato == clave) {
                return true;
            }else {
                // Paso recursivo
                return existeRecursivo(nodo->sig, clave);
            }
        }
    public:
        TablaHash(int tamanio, int (*f)(K)) {
            int nuevoTamanio = this->siguientePrimo(tamanio);
            arrList = new NodoPtr[nuevoTamanio]; // instancio el array
            for (int i = 0; i < nuevoTamanio; i++) { arrList[i] = 0; } // inicializo el array
            this->tamanio = nuevoTamanio;
            this->fnHash = f; // IMPORTANTE: me guardo la funcion de hash para luego usar en el resto de las operaciones.
        }
        ~TablaHash() {
          for(int i=0;i<this->tamanio;i++){
          empty(arrList[i]);
          }
          delete[] arrList;
        }
        void empty(NodoPtr &node){
          if(node){
          empty(node->sig);
          delete node;
          }
        }
        void insertar(K clave,V valor) {
            int casillero = this->posHash(clave); // Obtengo la posicion inicial, el "casillero"
            this->insertarRecursivo(arrList[casillero], clave, valor);
        }
      void eliminar(K clave){
        int casillero = this->posHash(clave);
        NodoPtr borrar = arrList[casillero];
        NodoPtr aux = NULL;
        while (borrar != NULL && borrar->dato.getClave() != clave) {
          aux = borrar;
          borrar = borrar->sig;
        }
        if (borrar == NULL) {
          return;
        }
        else {
          if (aux == NULL) {
            arrList[casillero] = borrar->sig;
          } else {
              aux->sig = borrar->sig;
            }
            delete borrar;
          }
      }
        V recuperar(K clave) {
            int casillero = this->posHash(clave); // Obtengo la posicion inicial, el "casillero"
            return recuperarRecursivo(arrList[casillero], clave);
        }

        bool existe(K clave) {
            int casillero = this->posHash(clave); // Obtengo la posicion inicial, el "casillero"
            return existeRecursivo(arrList[casillero], clave);
        }

    /*void displayHash() { 
    for (int i = 0; i < tamanio; i++) { 
      cout << i; 
      NodoPtr aux = arrList[i];
      while (aux!=NULL) {
        cout << " --> " << aux->dato->getClave(); 
        aux= aux->sig;
      }
      cout << endl; 
    }
    }*/
};

int otraFuncionHash(string palabra) {
          int sum = 0;
          for (int k = 0; k < palabra.length(); k++)
          sum = sum + int(palabra[k]) * (k+1); // no solo sumamos la ascii sino que lo multiplicamos por la posicion dentro de la palabra
          return sum;
        }

int main(){
    int cant;
    cin >> cant;
    TablaHash<string,string>* hash = new TablaHash<string, string>(cant, otraFuncionHash);
    string dato;
    for (int i=0; i < cant; i++){
        cin >> dato;
        hash->insertar(dato, dato);
    }
    cin >> cant;
    for (int j=0; j<cant; j++){
        cin >> dato;
        if (hash->existe(dato)){
            cout << "1" << "\n";
        } else {
            cout << "0" << "\n";
        }
    }
   // hash->displayHash();
    return 0;
}