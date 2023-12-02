#include "SW.h"
#include <algorithm>
#include <random>
#include <vector>


// Inicjalizacja generatora liczb losowych
random_device rd;
mt19937 gen(rd());

int oblicz_koszt_drogi_SW2(const vector<int> rozwionzanie, vector<vector<int>> macierz) {
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



int calculateSum(const vector<int>& path, vector<vector<int>> macierz) {
    int value = 0;
    for (int i = 0; i < macierz.size() - 1; i++) {
        value += macierz[path[i]][path[i + 1]];
    }
    // Koszt powrotu do miasta początkowego
    value += macierz[path[macierz.size() - 1]][path[0]];
    return value;
}




vector<int> neighbourSwapTwoCities(vector<int> currentPath, int dimension) {
    vector<int> neighbourPath = currentPath;

    uniform_int_distribution<> dis(0,dimension-1);

    int city1 = dis(gen);
    int city2 = dis(gen);

    swap(neighbourPath[city1], neighbourPath[city2]);
    return neighbourPath;
}


void SW2(vector<vector<int>> macierz){
    // Inicjalizacja (wygenerowanie początkowego rozwiązania i ustawienie temperatury początkowej)
    vector<int> currentPath;
    vector<int> najlepsze_rozwionzanie;
    double alfa = 0.99;
    int eraLength = macierz.size()*5;

    // Generowanie początkowego rozwiązania poprzez losowanie
    //currentPath = randomPathGenerator(currentPath, macierz.size());



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

    int najlepsza_droga = INT_MAX;

    while(temperatura > 0.000000001){
        for(int i = 0; i < eraLength; i++){
            // Wygeneruj sąsiada poprzrez zmianę kolejności połowy miast
            // vector<int> nowe_rozwionzanie = neighbourSwapHalfCities(currentPath, dimension);

            // Wygeneruj sąsiada poprzez odwrócenie fragmentu trasy
            // vector<int> nowe_rozwionzanie = neighbourInvertCities(currentPath, dimension);

            // zamiana 2 miast miejscami
            vector<int> nowe_rozwionzanie = rozwionzanie;

            uniform_int_distribution<> dis(1, macierz.size() - 2);
            int elem1 = dis(gen);
            int elem2 = dis(gen);
            swap(nowe_rozwionzanie[elem1], nowe_rozwionzanie[elem2]);

            int obecny_koszt = oblicz_koszt_drogi_SW2(rozwionzanie, macierz);
            int nowy_koszt = oblicz_koszt_drogi_SW2(nowe_rozwionzanie, macierz);

            // Oblicz prawdopodobieństwo zaakceptowania gorszego rozwiązania
            double probability = exp((obecny_koszt - nowy_koszt) / temperatura);

            // Jeżeli trasa-sąsiad jest krótsza, zaakceptuj ją
            // jeżeli jest dłuższa to zaakceptuj ją z prawdpodobieństwem probability


            uniform_real_distribution<double> distribution(0.0, 1.0);

            double rand = distribution(gen);
            if((nowy_koszt < obecny_koszt) or (rand < probability)){
                rozwionzanie = nowe_rozwionzanie;

                //  aktualizacja najkrótszej trasy
                if(obecny_koszt < najlepsza_droga){
                    najlepsza_droga = obecny_koszt;
                    najlepsze_rozwionzanie = rozwionzanie;
                }
            }
        }
        temperatura = temperatura * alfa;
    }
    cout<<"Droga: ";
    for (int elem : najlepsze_rozwionzanie){
        cout<<elem<<" ";
    }
    cout<<endl;
    cout << najlepsza_droga << endl;
}