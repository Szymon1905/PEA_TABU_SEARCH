#include "SW.h"
#include <algorithm>
#include <random>
#include <vector>
#include <chrono>


// Inicjalizacja generatora liczb losowych
random_device rd;
mt19937 gen(rd());

double minimalna_temperatura = 0.000001;


int oblicz_koszt_drogi_SW2(const vector<int>& rozwionzanie, vector<vector<int>> macierz) {
    int suma = 0;
    for (int i = 0; i < rozwionzanie.size() - 1; i++) {
        suma += macierz[rozwionzanie[i]][rozwionzanie[i + 1]];
    }
    return suma;
}

void generuj_zachlannie_rozwionzanie2(vector<vector<int>> macierz, vector<int> &nieodwiedzone, vector<int> &rozwionzanie, int miasto_badane, int &dlugosc_drogi ) {
    int najmniejsza=INFINITY;
    int wybrane_miasto = -1;  // kolejne miasto do którego ide

    if (nieodwiedzone.empty()){
        dlugosc_drogi = dlugosc_drogi + macierz[miasto_badane][0];
        return;
    }

    for (int miasto : nieodwiedzone){

        if(miasto_badane == miasto){  // pomijam miasto samo do siebie
            continue;
        }

        if (macierz[miasto_badane][miasto] < najmniejsza){   // szukam nowej drogi krótkiej z nieodwiedzonych miast
            najmniejsza = macierz[miasto_badane][miasto];
            wybrane_miasto = miasto;
        }
    }

    dlugosc_drogi = dlugosc_drogi + najmniejsza;

    // dodaje miasto do rozwiązania
    rozwionzanie.push_back(wybrane_miasto);

    // usuwam miasto bo jest odwiedzone
    nieodwiedzone.erase(remove(nieodwiedzone.begin(), nieodwiedzone.end(), wybrane_miasto), nieodwiedzone.end());

    //rekurencja
    generuj_zachlannie_rozwionzanie2(macierz, nieodwiedzone, rozwionzanie, wybrane_miasto, dlugosc_drogi);
}

void SW2(vector<vector<int>> macierz){
    // Inicjalizacja (wygenerowanie początkowego rozwiązania i ustawienie temperatury początkowej)
    vector<int> currentPath;
    vector<int> najlepsze_rozwionzanie;
    double alfa = 0.99;
    int iteracja = macierz.size() * 5;

    uniform_real_distribution<double> dis_r(0.0, 1.0);
    uniform_int_distribution<> dis(1, macierz.size() - 2);


    // Generowanie początkowego rozwiązania metodą zachłanną
    vector<int> rozwionzanie;
    rozwionzanie.push_back(0);
    vector<int> nieodwiedzone;
    for (int i = 1; i < macierz.size(); i++) {
        nieodwiedzone.push_back(i);
    }

    int dlugosc_drogi = 0;
    generuj_zachlannie_rozwionzanie2(macierz, nieodwiedzone, rozwionzanie, 0, dlugosc_drogi);
    rozwionzanie.push_back(0);

    najlepsze_rozwionzanie = rozwionzanie;

    // Oblicz początkową temperaturę
    double temperatura = oblicz_koszt_drogi_SW2(rozwionzanie, macierz) * alfa;
    cout<<"Temperatura początkowa: "<<temperatura<<endl;
    cout<<endl;

    int najlepszy_koszt = oblicz_koszt_drogi_SW2(rozwionzanie, macierz);

    auto start = chrono::high_resolution_clock::now();

    while(temperatura > minimalna_temperatura){
        for(int i = 0; i < iteracja; i++){

            // zamiana 2 miast miejscami
            vector<int> nowe_rozwionzanie = rozwionzanie;

            int elem1 = dis(gen);
            int elem2 = dis(gen);
            swap(nowe_rozwionzanie[elem1], nowe_rozwionzanie[elem2]);

            int obecny_koszt = oblicz_koszt_drogi_SW2(rozwionzanie, macierz);
            int nowy_koszt = oblicz_koszt_drogi_SW2(nowe_rozwionzanie, macierz);

            // Oblicz prawdopodobieństwo zaakceptowania gorszego rozwiązania
            double p = exp((obecny_koszt - nowy_koszt) / temperatura);

            // Jeżeli nowe rozwiązanie jest krótsze, to akceptuje je
            // jeżeli jest dłuższe to akceptuje je ale z prawdpodobieństwem r <=p

            double r = dis_r(gen);
            if((nowy_koszt < obecny_koszt) or (r <= p)){
                rozwionzanie = nowe_rozwionzanie;

                //  aktualizacja najkrótszej trasy
                if(obecny_koszt < najlepszy_koszt){
                    najlepszy_koszt = obecny_koszt;
                    najlepsze_rozwionzanie = rozwionzanie;

                    auto t = chrono::high_resolution_clock::now();
                    auto minelo_czasu = chrono::duration_cast<chrono::seconds>(t - start).count();
                    auto minelo_czasu_mili = chrono::duration_cast<chrono::milliseconds>(t - start).count();
                    cout << "Nowe najlepsze rozwiazanie znalezione po " << minelo_czasu << " sekundach." << endl;
                    cout << "Nowe najlepsze rozwiazanie znalezione po " << minelo_czasu_mili << " milisekundach." << endl;
                    cout << "Tempertura: " << temperatura << endl;
                    cout <<"najlepszy_koszt: " <<najlepszy_koszt << endl;
                }
            }
        }
        temperatura = temperatura * alfa; // schładzanie
    }

    cout<<"Tempertura końcowa: "<<temperatura<<endl;
    cout<<"Droga: ";
    for (int elem : najlepsze_rozwionzanie){
        cout<<elem<<" ";
    }
    cout<<endl;
    cout << najlepszy_koszt << endl;
}