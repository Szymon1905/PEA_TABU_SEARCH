#include "SW.h"
#include <random>

using namespace std;

double temperatura = 1;
// koszt tej sciezki razy alfa = tempertura

// todo obliczyć temperturę dynamicznie

double alfa = 0.99;

vector<int> najlepsze;
int koszt_najlepsze = 9999999;

int era;

void reset(auto &zmienna, auto wartosc){
    zmienna = wartosc;
}


int oblicz_koszt_drogi_SW(const vector<int> rozwionzanie, vector<vector<int>> macierz) {
    int suma = 0;
    for (int i = 0; i < rozwionzanie.size() - 1; i++) {
        suma += macierz[rozwionzanie[i]][rozwionzanie[i + 1]];
    }
    return suma;
}

void SW(vector<int> x, vector<vector<int>> macierz){

    int koszt_y;
    int koszt_x;



    // początek etapu 1
    koszt_x = oblicz_koszt_drogi_SW(x, macierz);

    temperatura = temperatura * koszt_x * alfa;

    koszt_najlepsze = koszt_x;
    najlepsze = x;

    cout<<"Zachłanna początkowa droga wynosi: "<<koszt_x<<endl;
    for (int elem : x){
        cout<<elem<<" ";
    }
    cout<<endl;
    cout<<"-------------"<<endl;

    random_device rd;
    mt19937 gen(rd());

    vector<int> y; // losowy kandydat na rozwionzanie

    for (int i = 0; i < macierz.size(); i++) {
        y.push_back(i);
    }
    y.push_back(0);


    era = macierz.size() * 5;

    // koniec etapu 1




    while (temperatura > 0.00000001){
        for (int i = 0; i < era; i++) {

            //shuffle(y.begin()+1, y.end()-1, gen); // etap 2
            // swap 2 miast zamiast tego

            uniform_int_distribution<> dis(1, y.size() - 2);
            int elem1 = dis(gen);
            int elem2 = dis(gen);
            swap(y[elem1], y[elem2]);

            // etap 3
            koszt_y = oblicz_koszt_drogi_SW(y,macierz);

            if (koszt_y <= koszt_x ){
                koszt_x = koszt_y;   // etap 4
                x = y;
                if(koszt_najlepsze > koszt_y){
                    najlepsze = y;
                    koszt_najlepsze = koszt_y;
                }

            } else {
                double p = exp((-(koszt_y - koszt_x) / temperatura ));  // etap 6
                uniform_real_distribution<double> distribution(0.0, 1.0);
                double r = distribution(gen);
                //cout<<"prawdo: "<<p<<endl;

                if (r <= p){ // etap 7
                    koszt_x = koszt_y; // etap 4
                    x = y;
                    //break;
                }
            }
        }
        temperatura = temperatura * alfa; // etap 5
    }

    // insert losować elemt do ywjęcia i wstawic na losowe miejsce



    for (int elem : x){
        cout<<elem<<" ";
    }
    cout<<endl;
    cout<<"Najlepsza droga wynosi: "<<koszt_x<<endl;
    cout<<endl;
    cout<<endl;

    reset(temperatura,10000);
    reset(koszt_najlepsze,9999999);
    najlepsze.clear();
}