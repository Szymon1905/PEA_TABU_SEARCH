#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <limits>
#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <Windows.h>

using namespace std;

const int INF = numeric_limits<int>::max();
int global_liczba_miast;


// Number of cities
const int numCities = 10;

// Cost matrix (replace this with your own)
int costMatrix[numCities][numCities] = {
        {-1 , 81 , 50 , 18 , 75 , 39 ,107 , 77 , 87 , 43  },
        {81 , -1 , 76 , 21 , 37 , 26 , 34 , 58 , 66 , 15  },
        {50 , 76 , -1 , 24 , 14 , 58 ,100 , 68 , 33 , 30  },
        {18 , 21 , 24 , -1 , 19 , 58 , 68 , 62 , 84 , 81  },
        {75 , 37 , 14 , 19 , -1 , 31 , 60 , 65 , 29 , 91  },
        {39 , 26 , 58 , 58 , 31 , -1 , 64 , 21 , 42 , 46  },
        {107,  34, 100,  68,  60,  64,  -1,  15,  55,  16 },
        {77 , 58 , 68 , 62 , 65 , 21 , 15 , -1 , 17 , 34  },
        {87 , 66 , 33 , 84 , 29 , 42 , 55 , 17 , -1 , 68  },
        {43 , 15 , 30 , 81 , 91 , 46 , 16 , 34 , 68 , -1  }

};

int costMatrix20[20][20] = {
        {  -1,  46, 22, 39, 45, 49, 24, 8 , 23, 12, 34, 40, 25, 21, 47, 21, 46, 30, 22, 17         }          ,
        {  41,  -1, 41, 29, 37, 23, 27, 12, 11, 46, 13, 10, 7 , 41, 16, 23, 29, 27, 22, 24         }          ,
        {  30,  44, -1, 43, 11, 7 , 8 , 38, 34, 26, 21, 43, 33, 37, 19, 42, 47, 17, 39, 43          }         ,
        {  26,  39, 18, -1, 36, 25, 10, 47, 23, 6 , 45, 37, 9 , 18, 46, 5 , 10, 29, 30, 10           }        ,
        {  26,  11, 8 , 43, -1, 14, 28, 14, 24, 41, 10, 21, 46, 31, 33, 29, 44, 16, 33, 7          }          ,
        {  33,  13, 17, 29, 31, -1, 23, 25, 9 , 8 , 29, 35, 32, 16, 43, 26, 31, 40, 19, 42          }         ,
        {  20,  29, 7 , 8 , 17, 47, -1,  6, 20, 31, 11, 40, 22, 35, 9 , 31, 42, 40, 30, 26            }       ,
        {  39,  21, 15, 17, 46, 12, 47, -1, 27, 7 , 38, 38, 10, 49, 39, 43, 16,  8, 37, 21          }         ,
        {  16,  30, 38, 13, 5 , 16, 47, 40, -1, 45, 19, 44, 22, 43, 8 , 15, 41,  7, 10, 36           }        ,
        {  16,  19, 45, 11, 41, 38, 29, 8 , 9 , -1, 6 , 14, 13, 24, 8 , 43, 20,  13, 42, 22            }      ,
        {  43,  48, 38, 33, 37, 41, 5 , 7 , 48, 24, -1, 49, 16, 40, 41, 40, 34,   9, 16, 33           }       ,
        {  23,  19, 33, 44, 34, 9 , 37, 5 , 15, 38, 38, -1, 18, 17, 24, 13, 28,   5,  8, 8             }      ,
        {  20,  46, 43, 31, 19, 13, 24, 27, 47, 47, 48, 12, -1, 13, 19, 15, 7 ,  43, 31, 20         }         ,
        {  33,  36, 41, 7 , 29, 22, 35, 38, 16, 42, 31, 25, 43, -1, 39, 32, 44,  26, 16, 8          }         ,
        {  22,  26, 36, 34, 7 , 41, 42, 49, 40, 7 , 40, 40, 38, 9 , -1, 46, 23,  41, 29, 5            }       ,
        {  48,  12, 15, 22, 30, 48, 21, 47, 47, 27, 22, 34, 44, 31, 49, -1, 46,  14, 33, 32        }          ,
        {  10,  30, 19, 39, 21, 22, 45, 20, 33, 23, 34, 8 , 39, 36, 42, 24, -1,   9, 27, 26          }        ,
        {  39,  18, 25, 17, 36, 31, 45, 13, 42, 40, 44, 29, 6 , 46, 21, 31, 28,  -1, 43, 15         }         ,
        {  5 ,  33 ,17 ,13, 37, 25, 43, 41, 21, 33, 26, 42, 36, 9 , 26, 22, 24,   6, -1, 5            }       ,
        {  25,  18, 31, 34, 19, 17, 29, 23, 49, 14, 40, 25, 44, 6 , 38, 24, 37,  13, 28, -1         }

};

vector<vector<int>> wczytaj_macierz(const string& daneWejsciowe, int &liczba_miast) {

    ifstream plikWejsciowy;  // na odczyt pliku

    plikWejsciowy.open(daneWejsciowe);    // otwieram plik
    if (plikWejsciowy.is_open()) {          // sprawdzam czy plik poprawnie otwarty
        cout << "Otwarto plik " << daneWejsciowe << endl;
    } else {
        cout << "Nie udało się otworzyć pliku wejściowego" << endl;
        exit(-1);
    }

    plikWejsciowy >> liczba_miast; // wczytanie liczby miast

    vector<vector<int> > macierz(liczba_miast, vector<int>(liczba_miast));   // macierz na przechowanie danych z pliku

    for (int i = 0; i < liczba_miast; ++i) {    // wpisywanie do macierzy
        for (int j = 0; j < liczba_miast; ++j) {
            plikWejsciowy >> macierz[i][j];
        }
    }
    plikWejsciowy.close();   // zamykam plik

    return macierz;     // zwracam gotowa macierz
}











// Tabu Search parameters
const int maxIterations = 1000;
const int tabuListSize = 5;

// Function to calculate the total cost of a solution
int calculateCost(const vector<int>& solution, vector<vector<int>> macierz) {
    int totalCost = 0;
    for (int i = 0; i < global_liczba_miast - 1; ++i) {
        totalCost += macierz[solution[i]][solution[i + 1]];
    }
    totalCost += macierz[solution[global_liczba_miast - 1]][solution[0]]; // Return to the starting city
    return totalCost;
}

// Function to generate a random initial solution
vector<int> generateRandomSolution() {
    vector<int> solution(global_liczba_miast);
    for (int i = 0; i < global_liczba_miast; ++i) {
        solution[i] = i;
    }
    random_shuffle(solution.begin() + 1, solution.end()); // Shuffle, excluding the starting city
    return solution;
}

void generuj_zachlannie_rozwionzanie(vector<vector<int>> macierz, vector<int> &nieodwiedzone, vector<int> &rozwionzanie, int miasto_badane, int &dlugosc_drogi ) {
    int najmniejsza=INF;
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
    generuj_zachlannie_rozwionzanie(macierz, nieodwiedzone, rozwionzanie, wybrane_miasto, dlugosc_drogi);
}

// Function to perform the Tabu Search
vector<int> tabuSearch(const vector<vector<int>> macierz) {
    ////////////////////////////////////////////////////////////

    vector<int> rozwionzanie;
    rozwionzanie.push_back(0);
    vector<int> nieodwiedzone;
    for (int i = 1; i < global_liczba_miast; i++) {
        nieodwiedzone.push_back(i);
    }

    int dlugosc_drogi = 0;
    generuj_zachlannie_rozwionzanie(macierz, nieodwiedzone, rozwionzanie, 0, dlugosc_drogi);
    rozwionzanie.push_back(0);

    cout<<"Dlugosc drogi zchłannie: "<<dlugosc_drogi<<endl;
    for (int miasto: rozwionzanie){
        cout<<miasto<<" ";
    }
    cout<<endl;
    cout<<endl;


    ///////////////////////////////////////////////////////////
    vector<int> currentSolution = rozwionzanie;
    vector<int> bestSolution = currentSolution;
    int currentCost = dlugosc_drogi;
    int bestCost = currentCost;

    vector<vector<int>> tabuList;

    for (int iteration = 0; iteration < maxIterations; ++iteration) {
        for (int i = 1; i < global_liczba_miast - 1; ++i) {
            for (int j = i + 1; j < global_liczba_miast; ++j) {
                // Swap cities i and j in the solution
                swap(currentSolution[i], currentSolution[j]);

                // Calculate the cost of the new solution
                int newCost = calculateCost(currentSolution, macierz);

                // Check if the move is allowed based on tabu list and aspiration criteria
                if ((newCost < currentCost || iteration == 0) && find(tabuList.begin(), tabuList.end(), currentSolution) == tabuList.end()) {
                    currentCost = newCost;

                    // Update the best solution if a better one is found
                    if (currentCost < bestCost) {
                        bestSolution = currentSolution;
                        bestCost = currentCost;
                    }

                    // Add the current solution to the tabu list
                    tabuList.push_back(currentSolution);
                    if (tabuList.size() > tabuListSize) {
                        tabuList.erase(tabuList.begin());
                    }
                } else {
                    // Revert the swap if the move is not allowed
                    swap(currentSolution[i], currentSolution[j]);
                }
            }
        }
    }

    return bestSolution;
}


void TABU1(){
    cout << "[TABU1] Podaj nazwe pliku do wczytania: ";   // w pliku i tak jest liczba miast,
    cout << endl;

    string nazwa_pliku;
    cin>>nazwa_pliku;

    vector<vector<int> > macierz = wczytaj_macierz(nazwa_pliku, global_liczba_miast);

    auto start = chrono::high_resolution_clock::now(); // start pomiaru czasu

    vector<int> optimalRoute = tabuSearch(macierz);
    cout << "Optimal Route: ";
    for (int city : optimalRoute) {
        cout << city << " ";
    }
    cout << optimalRoute[0] << endl; // Return to the starting city
    cout << "Optimal Cost: " << calculateCost(optimalRoute, macierz) << endl;


    auto koniec = chrono::high_resolution_clock::now(); // koniec pomiaru czasu

    auto czas_wykonania = chrono::duration_cast<chrono::microseconds>(koniec - start);
    cout << "Czas wykonania: " << czas_wykonania.count() << " mikrosekund" << endl;
    cout << "Czas wykonania: " << czas_wykonania.count() / 1000 << " milisekund" << endl;
}

int main() {
    srand(time(0)); // Seed for randomization

    SetConsoleOutputCP(CP_UTF8); // Konsola ustawiona na utf-8 aby były Polskie litery

    int opcja;

    while(true){
        cout<<"Autor: Szymon Borzdyński"<<endl;
        cout << "Opcje:" << endl;
        cout << "0 - Wyjście" << endl;
        cout << "1 - TABU1" << endl;
        cin>>opcja;

        switch (opcja) {
            default:
                system("CLS");
                cout << "Błędna opcja" << endl << endl;
                cin >> opcja;
                break;
            case 0:
                return 0;
            case 1:
                TABU1();
                break;
            case 2:
                //aaaa
                break;
            case 3:
                vector<vector<int> > macierz = wczytaj_macierz("test4.txt", global_liczba_miast);
                vector<int> rozwionzanie;
                rozwionzanie.push_back(0);
                vector<int> nieodwiedzone;
                nieodwiedzone.push_back(1);
                nieodwiedzone.push_back(2);
                nieodwiedzone.push_back(3);

                int dlugosc_drogi;



                generuj_zachlannie_rozwionzanie(macierz, nieodwiedzone, rozwionzanie, 0, dlugosc_drogi);
                rozwionzanie.push_back(0);

                //rozwionzanie.push_back(0);
                for (int var : rozwionzanie) {
                    cout<<var<<" ";
                }
                cout<<dlugosc_drogi<<endl;
                cout<<endl;
                break;
        }
    }
}