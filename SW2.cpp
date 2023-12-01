#include "SW.h"
#include <algorithm>
#include <random>
#include <vector>


// Inicjalizacja generatora liczb losowych
random_device rd;
mt19937 generator(rd());

int oblicz_koszt_drogi_SW2(const vector<int> rozwionzanie, vector<vector<int>> macierz) {
    int suma = 0;
    for (int i = 0; i < rozwionzanie.size() - 1; i++) {
        suma += macierz[rozwionzanie[i]][rozwionzanie[i + 1]];
    }
    return suma;
}


vector<int> randomPathGenerator(vector<int> currentPath, int dimension){

    uniform_int_distribution<> distribution(0,dimension-1);

    for(int i = 0; i < dimension; i++){
        int rand = distribution(generator);
        while(find(currentPath.begin(), currentPath.end(), rand) != currentPath.end()){
            rand = distribution(generator);
        }
        currentPath.push_back(rand);
    }
    return currentPath;
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

    uniform_int_distribution<> distribution(0,dimension-1);

    int city1 = distribution(generator);
    int city2 = distribution(generator);

    swap(neighbourPath[city1], neighbourPath[city2]);
    return neighbourPath;
}


void SW2(vector<vector<int>> macierz){
    // Inicjalizacja (wygenerowanie początkowego rozwiązania i ustawienie temperatury początkowej)
    vector<int> currentPath;
    vector<int> bestPath;
    double coolingRate = 0.99;
    int eraLength = macierz.size()*5;

    // Inicjalizacja generatora liczb losowych
    random_device rd;
    mt19937 generator(rd());

    // Generowanie początkowego rozwiązania poprzez losowanie
    currentPath = randomPathGenerator(currentPath, macierz.size());



    // Generowanie początkowego rozwiązania metodą zachłanną
    // currentPath = greedyPathGenerator(currentPath, dimension);

    // Oblicz początkową temperaturę
    double temperature = oblicz_koszt_drogi_SW2(currentPath,macierz) * coolingRate;

    int minimum = INT_MAX;

    while(temperature > 0.000000001){
        for(int i = 0; i < eraLength; i++){
            // Wygeneruj sąsiada poprzrez zmianę kolejności połowy miast
            // vector<int> neighbourPath = neighbourSwapHalfCities(currentPath, dimension);

            // Wygeneruj sąsiada poprzez odwrócenie fragmentu trasy
            // vector<int> neighbourPath = neighbourInvertCities(currentPath, dimension);

            // Wygeneruj sąsiada poprzez swapowanie dwóch miast, z upewnieniem się, że nie zostanie wylosowane to samo miasto
            vector<int> neighbourPath = neighbourSwapTwoCities(currentPath, macierz.size());

            int currentPathCost = calculateSum(currentPath, macierz);
            int neighbourPathCost = calculateSum(neighbourPath, macierz);

            // Oblicz prawdopodobieństwo zaakceptowania gorszego rozwiązania
            double probability = exp((currentPathCost - neighbourPathCost) / temperature);

            // Jeżeli trasa-sąsiad jest krótsza, zaakceptuj ją
            // jeżeli jest dłuższa to zaakceptuj ją z prawdpodobieństwem probability


            uniform_real_distribution<double> distribution(0.0, 1.0);

            double rand = distribution(generator);
            if((neighbourPathCost < currentPathCost) || (rand < probability)){
                currentPath = neighbourPath;

                //  aktualizacja najkrótszej trasy
                if(currentPathCost < minimum){
                    minimum = currentPathCost;
                    bestPath = currentPath;
                }
            }
        }
        temperature *= coolingRate;
    }
    cout<<"Droga: ";
    for (int elem : bestPath){
        cout<<elem<<" ";
    }
    cout<<endl;
    cout << minimum << endl;
}