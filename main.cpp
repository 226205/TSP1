#include <fstream>
#include <iostream>
#include <conio.h>
#include <cmath>
#include <queue>
#include <vector>
//#include "BruteForce.h"


int cityamount=0;
int **distances;
int shortestdistance=INT_MAX;
std::string shortestpath= "";


void chosingfile();
bool fileread(std::string);
void Writetab(int**);
void BruteForce();
void BFrecurence(bool*, int*, int, int);
void BranchBounds();
int BBreductor(int**);


int  main()
{
    chosingfile();
//    BruteForce();
    BranchBounds();
    std::cout<<"Najkrotsza droga przez wszystkie miasta to: "<<shortestpath<<'\n';
    std::cout<<"Jej calkowity dystans wynosi: "<<shortestdistance;
}


void chosingfile()
{
    std::string filename = "2plik.txt";
    int choice = 0;
    while(choice != 49 && choice != 50){  // 49 oraz 50 to odpowiednio 1 oraz 2 w kodach ascii, będących wynikami getche()
        std::cout << "Wybierz plik do wczytania: \n 1.Wpisz nazwe pliku do wczytania \n 2.Wczytaj domyslny plik tekstowy \"plik.txt\" \nTwoj wybor: ";
        choice = getche(); // pobieranie jednego znaku z klawiatury w formie kodu ascii
        switch (choice){
            case 49: // w kodach ascii za znak '1' odpowiada liczba 49
                std::cout << "\nPodaj nazwe pliku: ";
                std::cin >> filename;
                if(fileread(filename) == 0){
                    std::cout << "Nie udalo sie wczytac pliku o podanej nazwie, sproboj ponownie! \n";
                    choice = 0;
                }
                break;
            case 50: // w kodach ascii za znak '2' odpowiada liczba 50
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
    std::cout << "\n\nPlik " << filename << " zostal wczytany poprawnie. \nZawarte w nim dane to: \n";
    Writetab(distances);
}

bool fileread(std::string filename)         // funkcja do odczytyawnia danych z pliku
{
    std::fstream plik;
    plik.open(filename,std::ios::in);
    if(plik.good())                         // sprawdzenie poprawności wczytania pliku, w razie niepowodzenia zwracamy błąd
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
            std::cout<<distances[i][j]<<" ";
        std::cout<<"\n";
    }
}

void BruteForce()
{
    int visited = 0, distance = 0;              // visited - pokazuje ile miast zostało już odwiedzonych/ distance - dystans który został pokonany od początku
    bool* city = new bool[cityamount];          // city* - tablica przechowująca informację czy dane miasto zostało już odwiedzone
    int* sequence = new int[cityamount + 1];    // sequence* - tablica zapisująca sekwencję kolejnych miast przez które podróżujemy
    for (int i = 1; i < cityamount; ++i){       // zerowanie tablic
        city[i] = false;
        sequence[i] = 0;}
    city[cityamount - 1] = true;                         // zaznaczenie miasta startowego jako odwiedzone
    sequence[0] = sequence[cityamount] = cityamount - 1; // ustawienie miasta startowego na ostatnie - przeszukiwanie zaczynamy od
                                                         // pierwszego, jest to najbardziej optymalne rozwiązanie
    BFrecurence(city, sequence, distance, visited);
    delete [] city;
}

void BFrecurence(bool city[],int sequence[], int distance, int visited)
{
    visited++;
    if(visited < cityamount){                                       // nie odwiedziliśmy jeszcze wszystkich miast
        for(int i = 0; i < cityamount; i++){
            if(city[i] != true){                                    // sprawdzanie kolejno wszystkich miast czy zostały odwiedzone
                city[i] = true;                                     // kiedy znajdziemy nieodwiedzone, to zmieniamy jego status...
                sequence[visited] = i;                              //...dopisujemy do sekwencji...
                BFrecurence(city, sequence, distance, visited);     //... i wywołujemy rekurencję
                city[i] = false;    //po wykonaniu rekurencji cofamy zmianę statusu, pozostałe parametry będą takie jak przed rekurencją albo zostaną nadpisane
            }
        }
    }
    if(visited == cityamount){                                     // wszystkie miasta zostały już odwiedzone, wracamy
        for(int i = 0; i < cityamount; i++)                        // obliczanie dystansu pokonanej drogi...
            distance += distances[sequence[i]][sequence[i+1]];     // ...przez dodanie dystansów dzielących kolejne miasta w sekwencji
        if(shortestdistance > distance){                           // sprawdzanie czy otrzymaliśmy najkrótszy dystans, jeśli tak, to zapisujemy jego dane
            shortestdistance = distance;                           // wypisywanie ewentualnego wyniku
            shortestpath = "";
            for(int i = 0; i < cityamount + 1; i++){               // wypisywanie sekwencji miast z tablicy operując na kodach znaków (np: 'A' + 2 = 'C')
                shortestpath += 'A' + sequence[i];
            }
        }
    }
}

struct Vertex
{
    int** reducedmatrix;    // dwuwymiarowa tablica zredukowana dla konkretnego vertexa
    int* sequence;          // tablica zbierająca informacje o kolejności miast już odwiedzonych
    int reducedcost;        // koszt wszystkich poprzedzających redukcji tablicy oraz przejść
    int visited;            // ilość już odwiedzonych miast
};

Vertex* newVertex(Vertex* prev, int citynumber)
{


}

void BranchBounds()
{

    int rcc;
    rcc = BBreductor(distances);
    std:: cout << "\n\n" << "redukcja:  " << rcc << "\n\n";
    Writetab(distances);
}

int BBreductor(int** costtab)
{
    int reductioncost = 0;           // koszt redukcji danej tabeli
    int currentcost;                 // zmienna zachowująca koszt danej kolumny/ rzędu

    for(int i = 0; i < cityamount; i++)
    {
        currentcost = 9999;                      // przy przejściu każdego rzędu/ kolumny wracamy do wartości maksymalnej
        for(int j = 0; j < cityamount; j++)      // znajdujemy najmniejszą wartość w rzędzie/ kolumnie i zapisujemy ją w postaci zmiennej
            if(costtab[i][j] < currentcost)
                currentcost = costtab[i][j];

        if(currentcost != 0 && currentcost != 9999) // jeśli wartość w/w zmiennej jest różna od zera oraz wartości maksymalnej
        {                                           // wtedy dodajemy ja do kosztu redukcji tabeli, jeśli nie to na jej miejscu występuje pomijalne zero
            reductioncost += currentcost;              // dodajemy koszt redukcji rzędu/ kolumny do sumy wszystkich redukcji
            for(int j = 0; j < cityamount; j++)                         // dla wszystkich wartości rzędu/ kolumny niebędącymi wartościami...
                if(costtab[i][j] != 0 && costtab[i][j] != 9999)         // ...maksymalnymi bądź zerem, odejmujemy wartość redukcji
                    costtab[i][j] -= currentcost;
        }


        currentcost = 9999;                         // blok identyczny jak powyższy, tylko do obsługi kolumn
        for(int j = 0; j < cityamount; j++)         // muszą występować osobno, bo inaczej mogłyby wystąpić podwójne redukcje
            if(costtab[j][i] < currentcost)
                currentcost = costtab[j][i];

        if(currentcost != 0 && currentcost != 9999)
        {
            reductioncost += currentcost;
            for(int j = 0; j < cityamount; j++)
                if(costtab[j][i] != 0 && costtab[j][i] != 9999)
                    costtab[j][i] -= currentcost;
        }
    }

    return reductioncost;
}
