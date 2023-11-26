#include "SW.h"
#include <random>

using namespace std;

vector<int> najlepsze_rozwionzanie;
int najlepsza_droga;


double temperatura = 100;

double alfa = 0.98;

int era;




int oblicz_koszt_drogi_SW(const vector<int>& rozwionzanie, vector<vector<int>> macierz) {
    int suma = 0;
    for (int i = 0; i < rozwionzanie.size() - 1; ++i) {
        suma += macierz[rozwionzanie[i]][rozwionzanie[i + 1]];
    }
    suma += macierz[rozwionzanie[rozwionzanie.size() - 1]][rozwionzanie[0]]; // dodatkowy koszt powrotu do startu
    return suma;
}

void SW(vector<int> &x, vector<vector<int>> macierz){

    int koszt_y;
    int koszt_x;

    // początek etapu 1
    koszt_x = oblicz_koszt_drogi_SW(x, macierz);

    cout<<"Najlepsza początkowa droga wynosi: "<<koszt_x<<endl;
    for (int elem : x){
        cout<<elem<<" ";
    }
    cout<<endl;
    cout<<"-------------"<<endl;

    random_device rd;
    mt19937 gen(rd());

    vector<int> y; // loswy kandydat na rozwionzanie

    for (int i = 1; i < macierz.size(); ++i) {
        y.push_back(i);
    }
    y.push_back(0);

    era = macierz.size() * 5;

    // koniec etapu 1




    while (temperatura > 1){
        for (int i = 0; i < era; i++) {

            shuffle(y.begin(), y.end(), gen); // etap 2

            // etap 3
            koszt_y = oblicz_koszt_drogi_SW(y,macierz);

            if (koszt_y <= koszt_x){
                koszt_x = koszt_y;   // etap 4
                x = y;
            } else {
                double p = pow(2.718, (-koszt_y - koszt_x) / temperatura  );  // etap 6
                uniform_real_distribution<double> distribution(0.0, 1.0);
                double r = distribution(gen);

                if (r <= p){ // etap 7
                    koszt_x = koszt_y; // etap 4
                    x = y;
                } else {
                    temperatura = temperatura * alfa; // etap 5
                }
            }
        }
    }



    for (int elem : x){
        cout<<elem<<" ";
    }
    cout<<endl;
    cout<<"Najlepsza droga wynosi: "<<koszt_x<<endl;
    cout<<endl;
    cout<<endl;
}