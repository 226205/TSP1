#include <fstream>
#include <iostream>
#include <conio.h>
#include <cmath>
#include <queue>
#include <vector>
#include <ctime>
//#include "BruteForce.h"


int cityamount=0;
int **distances;
int shortestdistance=INT_MAX;
std::string shortestpath= "";


void chosingfile();
bool fileread(std::string);
void Writetab(int**);
void menu();
void BruteForce();
void BFrecurence(bool*, int*, int, int);
void BranchBound();
int BBreductor(int**, int, int);



int  main()
{
    chosingfile();
    menu();
//    BruteForce();
//    BranchBound();
//    std::cout<<"Najkrotsza droga przez wszystkie miasta to: "<<shortestpath<<'\n';
//    std::cout<<"Jej calkowity dystans wynosi: "<<shortestdistance;
//    getche();
}


void chosingfile()
{
    std::string filename = "2plik.txt";
    int choice = 0;
    while(choice != '1' && choice != '2'){
        std::cout << "Wybierz plik do wczytania: \n 1.Wpisz nazwe pliku do wczytania \n 2.Wczytaj domyslny plik tekstowy \"plik.txt\" \nTwoj wybor: ";
        choice = getche(); // pobieranie jednego znaku z klawiatury w formie kodu ascii
        switch (choice){
            case '1':
                std::cout << "\nPodaj nazwe pliku: ";
                std::cin >> filename;
                if(fileread(filename) == 0){
                    std::cout << "Nie udalo sie wczytac pliku o podanej nazwie, sproboj ponownie! \n";
                    choice = 0;
                }
                break;
            case '2':
                if(fileread(filename) == 0){
                    std::cout << "\nNie udalo sie wczytac pliku, sproboj ponownie! \n";
                    choice = 0;
                }
                break;
            default:
                std::cout << "\nWybrano zla liczbe, prosze powtorzyc! \n";
                break;
        }
    }
    std::cout << "\nPlik " << filename << " zostal wczytany poprawnie. \nZawarte w nim dane to: \n";
    Writetab(distances);
}

bool fileread(std::string filename)         // funkcja do odczytywania danych z pliku
{
    std::fstream plik;
    plik.open(filename,std::ios::in);
    if(plik.good())                         // sprawdzenie poprawnosci wczytania pliku, w razie niepowodzenia zwracamy blad
    {
        plik>>cityamount;
        distances = new int* [cityamount];   // dwuwymiarowa tablica dynamiczna
        for(int i=0;i<cityamount;++i)
            distances[i]=new int[cityamount];
        for(int i=0;i<cityamount;++i)       // zapelnianie tablicy danymi z pliku
            for(int j=0;j<cityamount;++j)
                plik>>distances[i][j];
        return 1;
    }
    return 0;
}

void Writetab(int** cities)
{
    std::cout << "\n\n";
    for(int i=0;i<cityamount;++i)
    {
        for(int j=0;j<cityamount;++j)
            std::cout<<cities[i][j]<<" ";
        std::cout<<"\n";
    }
}

void menu()
{
    int choice = -1;
    while(choice != '0')
    {
        std::cout << "\nWybierz algorytm do rozwiazania TSP: \n1. BruteForce \n2. Branch & Bound \n3. Zmiana pliku \n0. Wyjscie \nTwoj wybor: ";
        choice = getche();
        switch(choice)
        {
        case '1':
            BruteForce();
            std::cout<<"Najkrotsza droga przez wszystkie miasta to: "<<shortestpath<<'\n';
            std::cout<<"Jej calkowity dystans wynosi: "<<shortestdistance;
            break;
        case '2':
            BranchBound();
            break;
        case '3':
            for(int i=0;i<cityamount;++i)        // zwolnij pamiec
                delete[] distances[i];
            delete[] distances;
            shortestdistance=INT_MAX;
            shortestpath= "";
            chosingfile();
            break;
        default:
            break;
        }
    }
}

void BruteForce()
{
    clock_t begin = clock();
    int visited = 0, distance = 0;              // visited - pokazuje ile miast zostalo juz odwiedzonych/ distance - dystans ktory zostal pokonany od poczatku
    bool* city = new bool[cityamount];          // city* - tablica przechowujaca informacje czy dane miasto zostalo juz odwiedzone
    int* sequence = new int[cityamount + 1];    // sequence* - tablica zapisujaca sekwencje kolejnych miast przez ktore podrozujemy
    for (int i = 1; i < cityamount; ++i){       // zerowanie tablic
        city[i] = false;
        sequence[i] = 0;}
    city[0] = true;                                      // zaznaczenie miasta startowego jako odwiedzone
    sequence[0] = sequence[cityamount] = 0;              // ustawienie miasta startowego na ostatnie

    BFrecurence(city, sequence, distance, visited);
    delete [] city;
    delete [] sequence;
    std::clock_t end = clock();
    std::cout<<"Czas: " << double(end - begin) / CLOCKS_PER_SEC << "\n\n";
}

void BFrecurence(bool city[],int sequence[], int distance, int visited)
{
    visited++;
    if(visited < cityamount){                                       // nie odwiedzilismy jeszcze wszystkich miast
        for(int i = 0; i < cityamount; i++){
            if(city[i] != true){                                    // sprawdzanie kolejno wszystkich miast czy zostaly odwiedzone
                city[i] = true;                                     // kiedy znajdziemy nieodwiedzone, to zmieniamy jego status...
                sequence[visited] = i;                              //...dopisujemy do sekwencji...
                BFrecurence(city, sequence, distance, visited);     //... i wywolujemy rekurencje
                city[i] = false;                                    // po wykonaniu rekurencji cofamy zmiane statusu, pozostale parametry beda takie jak przed rekurencja albo zostana nadpisane
            }
        }
    }
    if(visited == cityamount){                                     // wszystkie miasta zostaly juz odwiedzone, wracamy
        for(int i = 0; i < cityamount; i++)                        // obliczanie dystansu pokonanej drogi...
            distance += distances[sequence[i]][sequence[i+1]];     // ...przez dodanie dystansow dzielacych kolejne miasta w sekwencji
        if(shortestdistance > distance){                           // sprawdzanie czy otrzymalismy najkrotszy dystans, jesli tak, to zapisujemy jego dane
            shortestdistance = distance;                           // wypisywanie ewentualnego wyniku
            shortestpath = "";
            for(int i = 0; i < cityamount + 1; i++){               // wypisywanie sekwencji miast z tablicy operujac na kodach znakow (np: 'A' + 2 = 'C')
                shortestpath += 'A' + sequence[i];
//            std::cout<<"Najkrotsza droga przez wszystkie miasta to: "<<shortestpath<<'\n';
            }
        }
    }
}


struct Vertex
{
    int** reducedmatrix;    // dwuwymiarowa tablica przejsc zredukowana dla konkretnego vertexa
    int* sequence;          // tablica zbierajaca informacje o kolejnosci miast juz odwiedzonych
    int reducedcost;        // koszt wszystkich poprzedzajacych redukcji tablicy oraz przejsc
    int city;               // aktualne miasto w ktorym sie znajduje
    int visited;            // ilosc juz odwiedzonych miast
};

struct comparator                                   // objekt do porownywania node'ow w kolejce priorytetowej
{
    bool operator()(const Vertex* first, const Vertex* second) const
    {
        return first->reducedcost > second->reducedcost;          // zwraca wynik w postaci bool, porownuje czy koszt pierwszego
    }                                                             // przekazanego node'a jest wiekszy niz drugiego
};

Vertex* newVertex(Vertex* parent, int citynumber)
{
    Vertex* vertex = new Vertex;
    vertex->visited = parent->visited+1;                     // zapisanie poziomu w drzewie ( '->' = '=' )
    vertex->sequence = new int[vertex->visited+1];           // ustawienie wskaznika na poprzedniego node'a

    for(int i=0;i<vertex->visited;++i)                  // zapisanie w tablicy dotychczas odwiedzonych miast
        vertex->sequence[i] = parent->sequence[i];

    vertex->sequence[vertex->visited] = citynumber+1;
//    vertex->sequence[vertex->visited] = citynumber;
    vertex->city = citynumber;
//    vertex->sequence[parent->visited] = citynumber;                        // ustawienie numeru miasta node'a
    vertex->reducedmatrix = new int *[cityamount]; // utworzenie tablicy

    for(int i=0;i<cityamount;++i)
        vertex->reducedmatrix[i]=new int[cityamount];

    for(int i=0;i<cityamount;++i)                // skopiowanie zredukowanej tablicy z parent do vertex
        for(int j=0;j<cityamount;++j)
            vertex->reducedmatrix[i][j] = parent->reducedmatrix[i][j];

//    for(int i=0;i<cityamount;++i)                // zabezpieczenie przed powrotem do odwiedzonego miasta
//    {
//        vertex->reducedmatrix[parent->city][i] = 9999;
//        vertex->reducedmatrix[i][vertex->city] = 9999;
//    }
//
//    vertex->reducedmatrix[vertex->city][0] = 9999;

//    vertex->reducedcost = parent->reducedcost + parent->reducedmatrix[parent->sequence[parent->visited]][vertex->sequence[parent->visited]] + BBreductor(vertex->reducedmatrix, vertex->sequence);
    vertex->reducedcost = parent->reducedcost + parent->reducedmatrix[parent->city][vertex->city] + BBreductor(vertex->reducedmatrix, vertex->city, parent->city);
    Writetab(vertex->reducedmatrix);                                                  // policzenie kosztu dla vertex'a
    for(int i = 0; i < vertex->visited; i++) std::cout << " -> " << vertex->sequence[i];

    return vertex;
}

//sequence[best->visited]
void BranchBound()
{
    std::clock_t begin = clock();
    Vertex* firstcity = new Vertex;                      // utworzenie nowej instancji struktury Vertex w sposob dynamiczny
    firstcity->city =0;                                  // przypisanie wartości do struct.city dla dynamicznie deklarowanych instancji struktury( '->' = '=' )
    firstcity->reducedmatrix = new int* [cityamount];
        for(int i=0;i<cityamount;++i)
            firstcity->reducedmatrix[i]=new int[cityamount];
        for(int i=0;i<cityamount;++i)
            for(int j=0;j<cityamount;++j)
                firstcity->reducedmatrix[i][j] = distances[i][j];

    firstcity->reducedcost = BBreductor(firstcity->reducedmatrix, firstcity->city, firstcity->city);
    firstcity->visited = 0;
    firstcity->sequence = new int[firstcity->visited + 1];      // tworzenie tablicy odwiedzonych miast wielkoscia zalezne od ilosci juz odwiedzonych
    firstcity->sequence[firstcity->visited] = 1;

    std::priority_queue <Vertex*, std::vector <Vertex*>, comparator> q; // utworzenie kolejki do przechowywania
                                                    // najbardziej obiecujacych node'ow w odpowiedniej kolejnosci
    q.push(firstcity);                                   // w kolejce znajduje sie korzen, czyli pierwsze miasto

    while(!q.empty())                               // dopoki q nie jest pusta
    {
        Vertex* best = q.top();                     // zapisz node'a we wskazniku
//        Writetab(best->reducedmatrix);
        q.pop();                                    // zdejmij go z kolejki
        if(best->visited == cityamount - 1)        // jezeli to juz ostatni poziom
        {
            std::clock_t end = clock();
            std::cout<<"Czas: " << double(end - begin) / CLOCKS_PER_SEC << "\n\nKoszt sciezki wynosi: "<<best->reducedcost<<", a jej kolejnosc to:\n";
            for(int i=0;i<best->visited+1;++i)        // wypisz potrzebne informacje
                std::cout<<best->sequence[i]<<" -> ";
            std::cout<<best->sequence[0]<<"\n\n";
            for(int i=0;i<cityamount;++i)        // zwolnij pamiec
                delete[] best->reducedmatrix[i];
            delete[] best->reducedmatrix;
            delete[] best->sequence;
            break;                                  // wyjdz z programu
        }

        for(int i=0;i<cityamount;++i)            // dla kazdego nieodwiedzonego miasta
            if(best->reducedmatrix[best->city][i] < 9999)
            {
                Vertex* child = newVertex(best, i);     // utworz nowego node'a
                q.push(child);                      // dodaj go do kolejki
            }

        for(int i=0;i<cityamount;++i)            // skoro wszystkie nieodwiedzone miasta sa w kolejce
            delete[] best->reducedmatrix[i];        // usun obecnie rozwazane node'a z pamieci
        delete[] best->reducedmatrix;
        delete[] best->sequence;
    }
}


int BBreductor(int** costtab, int currentcity, int previouscity)
{
    int reductioncost = 0;           // koszt redukcji danej tabeli
    int currentcost;                 // zmienna zachowujaca koszt danej kolumny/ rzedu

    for(int i = 0; i < cityamount; i++)
    {
        currentcost = 9999;                      // przy przejsciu kazdego rzedu/ kolumny wracamy do wartosci maksymalnej
        for(int j = 0; j < cityamount; j++)      // znajdujemy najmniejsza wartosc w rzedzie/ kolumnie i zapisujemy ja w postaci zmiennej
            if(costtab[i][j] < currentcost)
                currentcost = costtab[i][j];

        if(currentcost != 0 && currentcost != 9999) // jesli wartosc w/w zmiennej jest rozna od zera oraz wartosci maksymalnej
        {                                           // wtedy dodajemy ja do kosztu redukcji tabeli, jesli nie to na jej miejscu wystepuje pomijalne zero
            reductioncost += currentcost;              // dodajemy koszt redukcji rzedu/ kolumny do sumy wszystkich redukcji
            for(int j = 0; j < cityamount; j++)                         // dla wszystkich wartosci rzedu/ kolumny niebedacymi wartosciami...
                if(costtab[i][j] != 0 && costtab[i][j] != 9999)         // ...maksymalnymi badz zerem, odejmujemy wartosc redukcji
                    costtab[i][j] -= currentcost;
        }
    }
    for(int i = 0; i < cityamount; i++)
    {
        currentcost = 9999;                         // blok identyczny jak powyzszy, tylko do obslugi kolumn
        for(int j = 0; j < cityamount; j++)         // musza wystepowac w osobnych petlach, bo inaczej moglyby wystapic podwojne, badz niepotrzebne redukcje...
            if(costtab[j][i] < currentcost)         // tj niezbedne jest wyliczenie najpierw dla jednych, potem dla drugich
                currentcost = costtab[j][i];

        if(currentcost != 0 && currentcost != 9999)
        {
            reductioncost += currentcost;
            for(int j = 0; j < cityamount; j++)
                if(costtab[j][i] != 0 && costtab[j][i] != 9999)
                    costtab[j][i] -= currentcost;
        }
    }

    if(currentcity != 0){
    for(int i=0;i<cityamount;++i)                // zabezpieczenie przed powrotem do odwiedzonego miasta
    {
        costtab[previouscity][i] = 9999;
        costtab[i][currentcity] = 9999;

    }

    costtab[previouscity][currentcity] = 9999;
  }
    return reductioncost;
}
