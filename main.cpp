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
#include <thread>
#include "SW.h"
#include "SW2.h"
using namespace std;

const int INF = MAXINT;
int global_liczba_miast;

// todo pamietaj najlepsze globalnie


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


// metody dywersyfikacji

void swapowanie(vector<int> &rozwionzanie){
    int miasto1 = rand() % (global_liczba_miast - 1) + 1; // Losowe miasto (pomijam startowe)
    int miasto2 = rand() % (global_liczba_miast - 1) + 1;
    swap(rozwionzanie[miasto1], rozwionzanie[miasto2]);
}

void wstawianie(vector<int> &rozwionzanie){
    int wybrana = rand() % (global_liczba_miast - 1) + 1; // Losowe miasto (pomijam startowe)
    int pozycja = rand() % (global_liczba_miast - 1) + 1;
    int miasto = rozwionzanie[wybrana];

    rozwionzanie.erase(rozwionzanie.begin() + wybrana);

    rozwionzanie.insert(rozwionzanie.begin() + pozycja, miasto);
}

void odwracanie(vector<int> &rozwionzanie){
    int poz1 = rand() % (global_liczba_miast - 1) + 1; // Losowe miasto (pomijam startowe)
    int poz2 = rand() % (global_liczba_miast - 1) + 1;
    reverse(rozwionzanie.begin() + poz1, rozwionzanie.begin() + poz2);
}







// parametry
const int liczba_iteracji = 100000;
const int rozmiar_listy_tabu = 20;

// oblicznaie dorgi
int oblicz_koszt_drogi(const vector<int>& rozwionzanie, vector<vector<int>> macierz) {
    int suma = 0;
    for (int i = 0; i < global_liczba_miast - 1; ++i) {
        suma += macierz[rozwionzanie[i]][rozwionzanie[i + 1]];
    }
    return suma;
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

// Tabu Search
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

    cout<<"Dlugość drogi zachłannie: "<<dlugosc_drogi<<endl;
    for (int miasto: rozwionzanie){
        cout<<miasto<<" ";
    }
    cout<<endl;
    cout<<endl;


    ///////////////////////////////////////////////////////////
    vector<int> obecnie_najlepsze_rozwionzanie = rozwionzanie;
    vector<int> najlepszeRozwionzanie = obecnie_najlepsze_rozwionzanie;
    int obecny_koszt = dlugosc_drogi;
    int najlepszy_koszt = obecny_koszt;

    vector<vector<int>> lista_tabu;

    for (int iteracja = 0; iteracja < liczba_iteracji; iteracja++) {
        int losowe_miasto1 = rand() % (global_liczba_miast - 1) + 1; // Losowe miasto (pomijam startowe)
        int losowe_miasto2 = rand() % (global_liczba_miast - 1) + 1;

        // Zamieniam miasta w rozwiązaniu
        swap(obecnie_najlepsze_rozwionzanie[losowe_miasto1], obecnie_najlepsze_rozwionzanie[losowe_miasto2]);

        // Obliczam koszt nowego rozwiązania
        int nowy_koszt = oblicz_koszt_drogi(obecnie_najlepsze_rozwionzanie, macierz);

        // Sprawdzam czy ruch jest wykonalny wobec listy tabu
        if ((nowy_koszt < obecny_koszt || iteracja == 0) && find(lista_tabu.begin(), lista_tabu.end(), obecnie_najlepsze_rozwionzanie) == lista_tabu.end()) {
            obecny_koszt = nowy_koszt;

            // Aktualizuje najlepsze rozwiązanie jeśli je znajdę
            if (obecny_koszt < najlepszy_koszt) {
                najlepszeRozwionzanie = obecnie_najlepsze_rozwionzanie;
                najlepszy_koszt = obecny_koszt;
            }

            // Dodaje obecne rozwiązanie do listy tabu
            lista_tabu.push_back(obecnie_najlepsze_rozwionzanie);
            if (lista_tabu.size() > rozmiar_listy_tabu) {
                lista_tabu.erase(lista_tabu.begin());
            }
        } else {
            // Cofam zamianę miast jeśli nie mogę tego zrobić
            swap(obecnie_najlepsze_rozwionzanie[losowe_miasto1], obecnie_najlepsze_rozwionzanie[losowe_miasto2]);
        }
    }

    return najlepszeRozwionzanie;
}


void TABU1(){
    cout << "[TABU1] Podaj nazwe pliku do wczytania: ";   // w pliku i tak jest liczba miast,
    cout << endl;

    string nazwa_pliku;
    cin>>nazwa_pliku;

    vector<vector<int> > macierz = wczytaj_macierz(nazwa_pliku, global_liczba_miast);

    auto start = chrono::high_resolution_clock::now(); // start pomiaru czasu

    vector<int> rozwionzanie = tabuSearch(macierz);

    auto koniec = chrono::high_resolution_clock::now(); // koniec pomiaru czasu

    cout << "Droga: ";
    for (int elem : rozwionzanie) {
        cout << elem << " ";
    }
    cout<<endl;
    cout << "Koszt: " << oblicz_koszt_drogi(rozwionzanie, macierz) << endl;

    auto czas_wykonania = chrono::duration_cast<chrono::microseconds>(koniec - start);
    cout << "Czas wykonania: " << czas_wykonania.count() << " mikrosekund" << endl;
    cout << "Czas wykonania: " << czas_wykonania.count() / 1000 << " milisekund" << endl;
    cout<<endl;
}

void TABU_test(){
    string nazwa;
    cout << "Podaj nazwe pliku: " << endl;
    cin >> nazwa;
    vector<vector<int> > macierz = wczytaj_macierz(nazwa, global_liczba_miast);
    for (int i = 0; i < 20; ++i) {
        vector<int> rozwionzanie = tabuSearch(macierz);
        cout << "Droga: ";
        for (int elem: rozwionzanie) {
            cout << elem << " ";
        }
        cout << endl;
        cout << "Koszt: " << oblicz_koszt_drogi(rozwionzanie, macierz) << endl;
        cout<<endl;
    }
}

void odliczanie(int sekundy) {
    auto start = chrono::high_resolution_clock::now();
    auto koniec = start + chrono::seconds(sekundy);

    while (chrono::high_resolution_clock::now() < koniec) {
        cout << "\rPozostały czas: " << chrono::duration_cast<chrono::seconds>(koniec - chrono::high_resolution_clock::now()).count() << " sekund";
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(100));
    }

    cout << "\rPozostały czas: 0 sekund" << endl;
    cout << "Koniec czasu" << endl;
}



vector<int> tabu_time(vector<vector<int>> macierz){

    vector<int> zachlanna;
    zachlanna.push_back(0);
    vector<int> nieodwiedzone;
    for (int i = 1; i < global_liczba_miast; i++) {
        nieodwiedzone.push_back(i);
    }

    int dlugosc_drogi = 0;
    generuj_zachlannie_rozwionzanie(macierz, nieodwiedzone, zachlanna, 0, dlugosc_drogi);
    zachlanna.push_back(0);

    cout<<"Dlugość drogi zachłannie: "<<dlugosc_drogi<<endl;
    for (int miasto: zachlanna){
        cout<<miasto<<" ";
    }
    cout<<endl;
    cout<<endl;

    vector<int> obecnie_najlepsze_rozwionzanie = zachlanna;
    vector<int> najlepszeRozwionzanie = obecnie_najlepsze_rozwionzanie;
    int obecny_koszt = dlugosc_drogi;
    int najlepszy_koszt = obecny_koszt;

    vector<vector<int>> lista_tabu;

    int czas;
    cout<<"Podaj czas w sekundach:"<<endl;
    cin>>czas;

    auto startTime = chrono::high_resolution_clock::now();
    auto endTime = startTime + chrono::seconds(czas);

    thread countdown_thread(odliczanie, czas);

    cout << "TABU czasowe ";
    while (chrono::high_resolution_clock::now() < endTime) {
        int losowe_miasto1 = rand() % (global_liczba_miast - 1) + 1; // Losowe miasto (pomijam startowe)
        int losowe_miasto2 = rand() % (global_liczba_miast - 1) + 1;

        // Zamieniam miasta w rozwiązaniu
        swap(obecnie_najlepsze_rozwionzanie[losowe_miasto1], obecnie_najlepsze_rozwionzanie[losowe_miasto2]);

        // Obliczam koszt nowego rozwiązania
        int nowy_koszt = oblicz_koszt_drogi(obecnie_najlepsze_rozwionzanie, macierz);

        // Sprawdzam czy ruch jest wykonalny wobec listy tabu
        if ((nowy_koszt < obecny_koszt ) && find(lista_tabu.begin(), lista_tabu.end(), obecnie_najlepsze_rozwionzanie) == lista_tabu.end()) {
            obecny_koszt = nowy_koszt;

            // Aktualizuje najlepsze rozwiązanie jeśli je znajdę
            if (obecny_koszt < najlepszy_koszt) {
                najlepszeRozwionzanie = obecnie_najlepsze_rozwionzanie;
                najlepszy_koszt = obecny_koszt;
            }

            // Dodaje obecne rozwiązanie do listy tabu
            lista_tabu.push_back(obecnie_najlepsze_rozwionzanie);
            if (lista_tabu.size() > rozmiar_listy_tabu) {
                lista_tabu.erase(lista_tabu.begin());
            }
        } else {
            // Cofam zamianę miast jeśli nie mogę tego zrobić
            swap(obecnie_najlepsze_rozwionzanie[losowe_miasto1], obecnie_najlepsze_rozwionzanie[losowe_miasto2]);
        }
    }
    countdown_thread.join();
    return najlepszeRozwionzanie;
}

void tabu_time_start(){
    string nazwa;
    cout << "Podaj nazwe pliku: " << endl;
    cin >> nazwa;
    vector<vector<int> > macierz = wczytaj_macierz(nazwa, global_liczba_miast);

    vector<int> rozwionzanie = tabu_time(macierz);

    cout<<endl;
    cout<<endl;
    cout<<endl;
    cout << "Finalna Droga: ";
    for (int elem: rozwionzanie) {
        cout << elem << " ";
    }
    cout << endl;
    cout << "Finalny Koszt: " << oblicz_koszt_drogi(rozwionzanie, macierz) << endl;
    cout<<endl;
}



void SW_start(){
    string nazwa;
    cout << "Podaj nazwe pliku: " << endl;
    cin >> nazwa;
    vector<vector<int> > macierz = wczytaj_macierz(nazwa, global_liczba_miast);

    vector<int> rozwionzanie;
    rozwionzanie.push_back(0);
    vector<int> nieodwiedzone;
    for (int i = 1; i < global_liczba_miast; i++) {
        nieodwiedzone.push_back(i);
    }

    int dlugosc_drogi = 0;
    generuj_zachlannie_rozwionzanie(macierz, nieodwiedzone, rozwionzanie, 0, dlugosc_drogi);
    rozwionzanie.push_back(0);



    // koniec etapu 1

    SW(rozwionzanie, macierz);

    cout<<"koniec SW"<<endl;
}

void SW2_start(){
    string nazwa;
    cout << "Podaj nazwe pliku: " << endl;
    cin >> nazwa;
    vector<vector<int> > macierz = wczytaj_macierz(nazwa, global_liczba_miast);

    vector<int> rozwionzanie;
    rozwionzanie.push_back(0);
    vector<int> nieodwiedzone;
    for (int i = 1; i < global_liczba_miast; i++) {
        nieodwiedzone.push_back(i);
    }

    int dlugosc_drogi = 0;
    generuj_zachlannie_rozwionzanie(macierz, nieodwiedzone, rozwionzanie, 0, dlugosc_drogi);
    rozwionzanie.push_back(0);

    cout<<"Dlugość drogi zachłannie: "<<dlugosc_drogi<<endl;
    for (int miasto: rozwionzanie){
        cout<<miasto<<" ";
    }
    cout<<endl;
    cout<<endl;

    // koniec etapu 1

    SW2(macierz);

    cout<<"koniec SW 2.0"<<endl;
}



int main() {
    srand(time(nullptr)); // Seed for randomization

    SetConsoleOutputCP(CP_UTF8); // Konsola ustawiona na utf-8 aby były Polskie litery

    int opcja;

    while(true){
        cout<<"Autor: Szymon Borzdyński"<<endl;
        cout << "Opcje:" << endl;
        cout << "0 - Wyjście" << endl;
        cout << "1 - TABU1" << endl;
        cout << "2 - TABU_test" << endl;
        cout << "3 - SWt" << endl;
        cout << "4 - TABU_time" << endl;
        cout << "5 - SW 2.0" << endl;
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
                TABU_test();
            case 3:
                SW_start();
            case 4:
                tabu_time_start();
            case 5:
                SW2_start();
        }
    }
}