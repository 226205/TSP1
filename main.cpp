#include <fstream>
#include <iostream>
#include <conio.h>
#include <cmath>
#include <queue>
#include <vector>
#include <ctime>
//#include "BruteForce.h"
#define pqueue std::priority_queue <Vertex*, std::vector <Vertex*>, comparator>
#define tqueue std::priority_queue <Vertex*, std::vector <Vertex*>, comparator>

int cityamount=0;
int **distances;


void chosingfile();
bool fileread(std::string);
void Writetab(int**);
void menu();
void BruteForce();
void BFrecurence(bool*, int*, int, int, int*, int*);
void BranchBound();
//void BBreductor();



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
    std::string filename = "plik.txt";
    int choice = 0;
    while(choice != '1' && choice != '2'){
        std::cout << "Wybierz plik do wczytania: \n 1.Wpisz nazwe pliku do wczytania \n 2.Wczytaj domyslny plik tekstowy \"plik.txt\" \nTwoj wybor: ";
        choice = getche(); // pobieranie jednego znaku z klawiatury w formie kodu ascii
        switch (choice){
            case '1':
                std::cout << "\nPodaj nazwe pliku: ";
                std::cin >> filename;
                if(fileread(filename) == 0){
                    std::cout << "Nie udalo sie wczytac pliku o podanej nazwie, sprobuj ponownie! \n";
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
    std::cout << cityamount << '\n';
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
            break;
        case '2':
            BranchBound();
            break;
        case '3':
            for(int i=0;i<cityamount;++i)        // zwolnij pamiec
                delete[] distances[i];
            delete[] distances;
            std::cout <<"\n\n";
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
    int shortestdistance = INT_MAX;
    int *pshortestdistance = & shortestdistance;
    int visited = 0, distance = 0;              // visited - pokazuje ile miast zostalo juz odwiedzonych/ distance - dystans ktory zostal pokonany od poczatku
    bool* city = new bool[cityamount];          // city* - tablica przechowujaca informacje czy dane miasto zostalo juz odwiedzone
    int* sequence = new int[cityamount + 1];    // sequence* - tablica zapisujaca sekwencje kolejnych miast przez ktore podrozujemy
    int* bestsequence = new int[cityamount +1];
    for (int i = 1; i < cityamount; ++i){       // zerowanie tablic
        city[i] = false;
        sequence[i] = 0;}
    city[0] = true;                                      // zaznaczenie miasta startowego jako odwiedzone
    sequence[0] = sequence[cityamount] = 0;              // ustawienie miasta startowego na ostatnie

    BFrecurence(city, sequence, distance, visited, pshortestdistance, bestsequence);
    std::clock_t end = clock();
    std::cout<<"\n\nNajkrotsza droga przez wszystkie miasta to: ";
    for(int i = 0; i < cityamount; i++)
        std::cout << bestsequence[i] + 1 << " -> ";
    std::cout<<"1\nJej calkowity dystans wynosi: " <<shortestdistance;
    std::cout<<"\nCzas: " << double(end - begin) / CLOCKS_PER_SEC << "\n\n";
    delete [] city;
    delete [] sequence;
    delete [] bestsequence;
}

void BFrecurence(bool city[],int sequence[], int distance, int visited, int* pshortestdistance, int bestsequence[])
{
    visited++;
    if(visited < cityamount){                                       // nie odwiedzilismy jeszcze wszystkich miast
        for(int i = 0; i < cityamount; i++){
            if(city[i] != true){                                    // sprawdzanie kolejno wszystkich miast czy zostaly odwiedzone
                city[i] = true;                                     // kiedy znajdziemy nieodwiedzone, to zmieniamy jego status...
                sequence[visited] = i;                              //...dopisujemy do sekwencji...
                BFrecurence(city, sequence, distance, visited, pshortestdistance, bestsequence);     //... i wywolujemy rekurencje
                city[i] = false;                                    // po wykonaniu rekurencji cofamy zmiane statusu, pozostale parametry beda takie jak przed rekurencja albo zostana nadpisane
            }
        }
    }
    if(visited == cityamount){                                     // wszystkie miasta zostaly juz odwiedzone, wracamy
        for(int i = 0; i < cityamount; i++)                        // obliczanie dystansu pokonanej drogi...
            distance += distances[sequence[i]][sequence[i+1]];     // ...przez dodanie dystansow dzielacych kolejne miasta w sekwencji

        std::cout << "\nSekwencja: ";
        for(int i = 0; i < cityamount; i++)
                    std::cout << sequence[i] + 1 << " -> ";
        std::cout << '1';


        if(*pshortestdistance > distance){                           // sprawdzanie czy otrzymalismy najkrotszy dystans, jesli tak, to zapisujemy jego dane
            *pshortestdistance = distance;                           // wypisywanie ewentualnego wyniku
            for(int i = 0; i < cityamount + 1; i++)               // wypisywanie sekwencji miast z tablicy operujac na kodach znakow (np: 'A' + 2 = 'C')
                bestsequence[i] = sequence[i];
        }
    }
}


struct Vertex
{
    int* sequence;          // tablica zbierajaca informacje o kolejnosci miast juz odwiedzonych
    int reducedcost = 0;        // koszt wszystkich poprzedzajacych redukcji tablicy oraz przejsc
    int visited;            // ilosc juz odwiedzonych miast
    Vertex() {};
    Vertex(Vertex* parent,bool* city, int newcitynumber)
    {
 //       std::cout << "sadad" << newcitynumber;
        visited = parent->visited + 1;                     // zapisanie poziomu w drzewie ( '->' = '=' )
        sequence = new int[visited];           // ustawienie wskaznika na poprzedniego node'a

        for(int p=0;p<visited - 1;++p){                  // zapisanie w tablicy dotychczas odwiedzonych miast
            sequence[p] = parent->sequence[p];
//            std::cout << " \nparent: " << sequence[p];
        }
//        std::cout << " visited: " << visited;

        sequence[visited - 1] = newcitynumber + 1;
//        std::cout << " child: " << sequence[visited - 1];

        for(int i = 0; i < cityamount; i++)     //ustawianie tablicy booli
            city[i] = false;
//        std::cout << " sssssss " << visited;
//        std::cout << " " << sequence[0]<<"eee";
//        std::cout << " " << sequence[1];
        for(int i = 0; i < visited; i++){
            city[sequence[i]] = true;
//            int a = sequence[i];
//            city[a] = true;
        }

//        {std::cout<<"\n\nwejsciowa sekwencja: ";
//        for(int i = 0; i < visited; i++)
//            std::cout<<" " << sequence[i];}

        for(int i = 1; i < visited; i++)
            reducedcost += distances[sequence[i - 1] - 1][sequence[i] - 1];

//        std::cout<<"\nwejsciowy reducedcost: " << reducedcost;
        for(int i = 0; i < cityamount; i++)     //ustawianie tablicy booli
            city[i] = false;
        for(int i = 0; i < visited; i++)
            city[sequence[i] - 1] = true;

//        std::cout <<" \nco dodajemy: ";
        for(int temp, i = 0; i < cityamount; i++){                      // odtwarzanie wszystkich rzedow
            temp = INT_MAX;
//            std::cout <<" \n\ns:" <<i<< " c:" <<city[i];                                            // ustawienie maksymalnej wartosci w rzedzie do porownan
            if(city[i] == false || i == (sequence[visited - 1]) - 1){         // zabronienie wszystkich rzedow z ktorych miasta znajduja sie w sekwencji poza ostatnim
//                    std::cout <<" weszlo";
                for(int j = 0; j < cityamount; j++){                     // odtwarzanie wszystkich miast w rzedach
                    if(city[j] == false || (j == sequence[0]) - 1){            // zabronienie wszystkich kolumn z ktorych miasta znajduja sie w sekwencji poza pierwszym
//                        std::cout << " a:" << j;
                        if(!(i == (sequence[visited - 1] - 1) && j == (sequence[visited - 2]) - 1)){
//                        std::cout <<" wlaz " << sequence[visited - 2] - 1 << "," <<(sequence[visited - 1]) - 1;
                        if(i != j && distances[i][j] < temp){  // pierwsze dwa to zabronienie powrotu do miasta
                                            // startowego, potem czy nie znajdujemy sie na przekatnej(odwolanie do samego siebie), a na koniec
                                            // porownanie czy mamy aktualne minimum w rzedzie
                            temp = distances[i][j];
//                            std::cout <<" + " << i <<','<<j<<'='<<distances[i][j];
                        }
                    }
                    }
                }
//                std::cout<<" d:" << temp;
                reducedcost += temp;
            }
        }
//        std::cout << " reducedcost: " << reducedcost;
    }
    ~Vertex()
    {
        delete[] sequence;
    }
};

struct comparator                                   // objekt do porownywania node'ow w kolejce priorytetowej
{
    bool operator()(Vertex* first, Vertex* second)
    {
        if((first->reducedcost - first->visited) > (second->reducedcost - second->visited))
            return true;
        else if((first->reducedcost - first->visited) < (second->reducedcost - second->visited))
            return false;
        else
            return first->visited > second->visited;          // zwraca wynik w postaci bool, porownuje czy koszt pierwszego
    }                                                             // przekazanego node'a jest wiekszy niz drugiego
};

void BranchBound()
{
    std::clock_t end, begin = clock();

    int UpperBound = INT_MAX;
    int* solution = new int[cityamount + 1];
    solution[0] = solution[cityamount] = 1;
    bool* city = new bool[cityamount];
    for(int i = 1; i < cityamount; i++)
        city[i] = false;
    city[0] = true;

    Vertex* firstcity = new Vertex;                      // utworzenie nowej instancji struktury Vertex w sposob dynamiczny
//    firstcity->currentcity =0;                                  // przypisanie wartości do struct.city dla dynamicznie deklarowanych instancji struktury( '->' = '=' )
    firstcity->visited = 1;
    firstcity->sequence = new int[firstcity->visited];      // tworzenie tablicy odwiedzonych miast wielkoscia zalezne od ilosci juz odwiedzonych
    firstcity->sequence[firstcity->visited - 1] = 1;

    int tempDist = 0;
    for(int temp, i = 0; i < cityamount; i++){
        temp = INT_MAX;
        for(int j = 0; j < cityamount; j++)
            if(distances[i][j] < temp && i != j)
                temp = distances[i][j];
        tempDist += temp;

    }

    firstcity->reducedcost = tempDist;        // poczatkowym lower boundem jest minimalny koszt przejscia wszystkich rzedow

    pqueue q; // utworzenie kolejki do przechowywania najbardziej obiecujacych node'ow w odpowiedniej kolejnosci
    q.push(firstcity);                                   // w kolejce znajduje sie korzen, czyli pierwsze miasto

    int GreedyAlgorithm = cityamount*cityamount;
    while(!q.empty())                               // dopoki q nie jest pusta
    {
        Vertex* best = q.top();                     // zapisz node'a we wskazniku
        q.pop();                                    // zdejmij go z kolejki
        GreedyAlgorithm++;

        if(GreedyAlgorithm >= cityamount*cityamount)
        {
            GreedyAlgorithm = 0;
            tqueue tempQ;   //tymczasowa kolejka pomocnicza
            Vertex* tempBest = best;

            int repeat = 1;
            do
            {

                for(int i = 0; i < cityamount; i++){
                    for(int i = 0; i < cityamount; i++)     //ustawianie tablicy booli
                        city[i] = false;
                    for(int i = 0; i < tempBest->visited; i++)
                        city[tempBest->sequence[i] - 1] = true;

//                    std::cout << "\nboole w wejsciu ";
//                    for(int i = 0; i < cityamount; i++)
//                        std::cout<< " " << city[i];

                    if(city[i] != true)
                    {
//                        std::cout << " wejscia: " << i;
                        Vertex* child = new Vertex(tempBest, city, i);    // tworzenie potomkow naszego najlepszego vertexa
                        if(child->reducedcost < UpperBound)         // jezeli jest szansa na lepszy wynik niz aktualny upperbound, to zachowujemy potomka
                            tempQ.push(child);
                        else                                        // w przeciwnym razie pozbywamy sie go, gdyz nie uzyskamy lepszego rozwiazania za jego pomoca
                            delete child;
                    }
                }
                delete tempBest;                // usuniecie przeanalizowaniego vertexa

                if(!tempQ.empty()){             // ustawienie znacznika tempBest dla najlepiej rokujacego dziecka, jesli jakies pozostaly
                tempBest = tempQ.top();
                tempQ.pop();
                }
                else
                    repeat = 0;

                //std::cout << " ggggg "  << tempBest->visited;
                while(!tempQ.empty())           // przeniesienie nieuzytych vertexow do glownej kolejki, jesli jakies sa
                {
                    q.push(tempQ.top());
                    tempQ.pop();
                }


                if(tempBest->visited == (cityamount - 1)){
                    best = tempBest;
                    repeat = 0;
//                    std::cout << "\n\n wychodze";
                }
            }while(repeat != 0);
//            std::cout << "\n\n wyszedlem, kolejka posiada: " <<q.size()<< " vertexow\n";
        }
        else if(best->visited < (cityamount - 1))
        {
            for(int i = 0; i < cityamount; i++)     //ustawianie tablicy booli
                city[i] = false;
            int e;
            for(int i = 0; i < best->visited; i++){
                e = best->sequence[i] - 1;
                city[(best->sequence[i] - 1)] = true;}

            for(int i = 0; i < cityamount; i++)
                if(city[i] != true)
                {
                    Vertex* child = new Vertex(best, city, i);    // tworzenie potomkow naszego najlepszego vertexa
                    if(child->reducedcost < UpperBound)         // jezeli jest szansa na lepszy wynik niz aktualny upperbound, to zachowujemy potomka
                        q.push(child);
                    else                                        // w przeciwnym razie pozbywamy sie go, gdyz nie uzyskamy lepszego rozwiazania za jego pomoca
                        delete child;
                }
//            delete best;
        }


        if(best->visited == (cityamount - 1))   // doszlismy do vertexa z ktorego zostala jedna droga, do ostatniego miasta a potem na start
        {
            for(int i = 0; i < cityamount; i++)     //ustawianie tablicy booli
                city[i] = false;
            for(int i = 0; i < best->visited; i++)
                city[(best->sequence[i] - 1)] = true;

            for(int i = 0; i < cityamount; i++) // znalezienie ostatniego miasta
                if(city[i] != true)
                {
//                    std::cout<< "\n\n\nZnalezione ostatnie miasto" << i << " reducedcost: " << best->reducedcost << "sekwencja:";
//                    for(int f = 0; f < best->visited; f++)
//                        std::cout << best->sequence[f] << " ";
                    best->reducedcost = 0;
                    for(int j = 1; j < best->visited; j++){              // dodanie wszystkich skladowych dystansu, kolejno dystanse miedzy miastami w sekwencji
                        best->reducedcost += distances[best->sequence[j-1] - 1][best->sequence[j] - 1];
//                        std::cout << " \ndodajemy dystans od: " << (best->sequence[j-1]) << " do: " << best->sequence[j] << " wiec aktualny reducedcost jest rowny: " << best->reducedcost;
                      }

                    best->reducedcost += distances[best->sequence[best->visited - 1] - 1][i];
                    best->reducedcost += distances[i][0];
//                    std::cout << "     " << best->reducedcost;
                    if(best->reducedcost < UpperBound)
                    {
                        UpperBound = best->reducedcost;     // nadpisanie gornej granicy, jesli osiagniety lisc ma nizszy koszt od gornej granicy
                        for(int j = 1; j < best->visited; j++)
                            solution[j] = best->sequence[j];
                        solution[best->visited] = i + 1;

                        pqueue tQ;
                        Vertex* tBest;
                        while(!q.empty())
                        {
                            tBest = q.top();
                            q.pop();
                            if(tBest->reducedcost < UpperBound)
                                tQ.push(tBest);
                            else
                                delete tBest;
                        }
                        while(!tQ.empty())
                        {
                            tBest = tQ.top();
                            tQ.pop();
                            q.push(tBest);
                        }
                        std::cout << "\nPozostalo " << q.size() << " instancji vertexa w kolejce.";
                    }
//                    delete best;
                }
        }
        delete best;
    }
    std::cout << "Wynikiem BnB jest: " << UpperBound << " ; sekwencja: ";
    for(int i = 0; i < cityamount + 1; i++)
    std::cout << " " << (solution[i] - 1);

    end = clock();
    std::cout << "\nCzas wykonywania: " << double(end - begin) / CLOCKS_PER_SEC;

    tempDist = 0;
    for(int i = 0; i < cityamount; i++)
        tempDist += distances[solution[i] - 1][solution[i + 1] - 1];
    std::cout << "\nsprawdzenie: " << tempDist;



    delete [] solution;
    delete [] city;
}
