#include <iostream>
#include <assert.h>
using namespace std;

class Asociacion{
private:
    int x; //posicion
    int y; //altura
public:
    Asociacion() {}
    Asociacion(int unaX, int unY) : x(unaX), y(unY) {}
    int getPos() { return this->x; };
    int getAltura() { return this->y; };
    bool operator<(Asociacion comp) { return this->getPos() < comp.getPos(); }
    bool operator<=(Asociacion comp) { return this->getPos() <= comp.getPos(); }
    bool operator>(Asociacion comp) { return this->getPos() > comp.getPos(); }
    bool operator>=(Asociacion comp) { return this->getPos() >= comp.getPos(); }
    bool operator==(Asociacion comp) { return this->getPos() == comp.getPos(); }
};

int max(int x, int y){
    if (x < y)
        return y;
    else
        return x;
}

Asociacion *juntar(Asociacion *arr1, Asociacion *arr2, int largo){
    int i = 0;
    int j = 0;
    int h1 = 0;
    int h2 = 0;
    int pos = 0;
    int alt = 0;
    int ulth = 0;
    Asociacion *ret = new Asociacion[largo * 2];
    for (int k = 0; k < largo * 2; k++){
        Asociacion *nuevo = new Asociacion(0, 0);
        ret[k] = *nuevo;
    }
    while (i < largo && j < largo){
        //fijarse que no sea (0,0)
        if (arr1[i].getPos() == 0 && arr1[i].getAltura() == 0){
            i++;
        }
        else{
            if (arr2[j].getPos() == 0 && arr2[j].getAltura() == 0){
                j++;
            }
            else{
                if (arr1[i] == arr2[j]){
                    if (arr1[i].getAltura() == arr2[j].getAltura()){
                        h1 = h2 = arr1[i].getAltura();
                        i++;
                    } else {
                        if (arr1[i].getAltura() < arr2[j].getAltura()){     
                            h1 = arr1[i].getAltura();                       
                            i++;                                            
                        } else {                                            
                            h2 = arr2[j].getAltura();                       
                            j++;                                            
                        }
                    }
                } else{
                    if (arr1[i] < arr2[j]){
                        h1 = arr1[i].getAltura();
                        pos = arr1[i].getPos();
                        i++;
                    }
                    else{    
                        h2 = arr2[j].getAltura();
                        pos = arr2[j].getPos();
                        j++;
                    }
                    alt = max(h1, h2);
                    if (alt != ulth){
                        Asociacion *nuevo = new Asociacion(pos, alt);
                        ret[i + j - 1] = *nuevo;
                        ulth = alt;
                    }
                }
            }
        }
    }

    while (i < largo){
        h1 = arr1[i].getAltura();
        pos = arr1[i].getPos();
        i++;
        alt = max(h1, h2);
        if (alt != ulth){
            Asociacion *nuevo = new Asociacion(pos, alt);
            ret[i + j - 1] = *nuevo;
            ulth = alt;
        }
    }
    while (j < largo){
        h2 = arr2[j].getAltura();
        pos = arr2[j].getPos();
        j++;
        alt = max(h1, h2);
        if (alt != ulth){
            Asociacion *nuevo = new Asociacion(pos, alt);
            ret[i + j - 1] = *nuevo;
            ulth = alt;
        }
    }
    return ret;
}

Asociacion *divide(Asociacion *sombras, int comienzoIndex, int finIndex, int largo){
    if (comienzoIndex + 1 == finIndex){
        Asociacion *nuevo = new Asociacion[2];
        nuevo[0] = sombras[comienzoIndex];
        nuevo[1] = sombras[finIndex];
        return nuevo;
    }

    int mitadIndex = (comienzoIndex + finIndex) / 2;

    Asociacion *res1 = divide(sombras, comienzoIndex, mitadIndex, largo / 2);
    Asociacion *res2 = divide(sombras, mitadIndex + 1, finIndex, largo / 2);

    return juntar(res1, res2, largo / 2);
}

int main(){
    int cantSombras;
    cin >> cantSombras;
    Asociacion *limites = new Asociacion[cantSombras * 2];
    int pos = 0;
    for (int i = 0; i < cantSombras; i++){
        int origen;
        int destino;
        int altura;
        cin >> origen;
        cin >> destino;
        cin >> altura;
        Asociacion nuevo(origen, altura);
        limites[pos++] = nuevo;
        Asociacion aux(destino, 0);
        limites[pos++] = aux;
    }
    if (cantSombras == 1){
        cout << limites[0].getPos() << " " << limites[0].getAltura() << endl;
        cout << limites[1].getPos() << " " << limites[1].getAltura() << endl;
    } else {
        limites = divide(limites, 0, cantSombras * 2 - 1, cantSombras * 2);
        for (int j = 0; j < cantSombras * 2; j++){
            if (limites[j].getPos() != 0 || limites[j].getAltura() != 0){
                cout << limites[j].getPos() << " " << limites[j].getAltura() << endl;
            }
        }
    }
    return 0;
}