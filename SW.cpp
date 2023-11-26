#include "SW.h"
#include <random>

using namespace std;

vector<int> najlepsze_rozwionzanie;
int najlepsza_droga;


double temperatura;

double alfa;

int p;


int oblicz_koszt_drogi(const vector<int>& rozwionzanie, vector<vector<int>> macierz) {
    int suma = 0;
    for (int i = 0; i < rozwionzanie.size() - 1; ++i) {
        suma += macierz[rozwionzanie[i]][rozwionzanie[i + 1]];
    }
    suma += macierz[rozwionzanie[rozwionzanie.size() - 1]][rozwionzanie[0]]; // dodatkowy koszt powrotu do startu
    return suma;
}

void SW(vector<int> &x, vector<vector<int>> macierz){

    int obecne_dlugosc = oblicz_koszt_drogi(x, macierz);

    random_device rd;
    mt19937 gen(rd());

    vector<int> y; // loswy kandydat na rozwionzanie

    for (int i = 1; i <= 10; ++i) {
        y.push_back(i);
    }

    shuffle(y.begin(), y.end(), gen); // genroanie losowego rozwiązania

    // koniec etapu 2


    int koszt_y = oblicz_koszt_drogi(y,macierz);

    int koszt_x = oblicz_koszt_drogi(x,macierz);

    if (koszt_y <= koszt_x){
        koszt_x = koszt_y;
    }



    // etap 9
    cout<<"Najlepsza droga wynosi: "<<koszt_x<<endl;
}