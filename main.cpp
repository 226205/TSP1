#include <fstream>
#include <iostream>
#include <cmath>
#include "BruteForce.h"

int cityamount=0;
int **distances;
int shortestdistance=INT_MAX;
std::string shortestpath= "";


void wypisz();
void BruteForce();
void Counter(bool*, std::string, int, int, int, int, int);
void fileread();

int  main()
{
    bool* c = new bool[cityamount]; //tworze tablice z miastami ktore odwiedzilem
    for (int i=0;i<cityamount;++i)
        c[i]=false;
    fileread();
    wypisz();
    BruteForce();
    std::cout<<"Najkrotsza droga przez wszystkie miasta to: "<<shortestpath<<'\n';
    std::cout<<"Jej calkowity dystans wynosi: "<<shortestdistance;
}

//funkcja do odczytyawnia danych z pliku
void fileread()
{
    std::fstream plik;
    plik.open("oldplik.txt",std::ios::in);
    if(plik.good()) //zabezpieczenie
    {
        plik>>cityamount;
        distances = new int* [cityamount];
        for(int i=0;i<cityamount;++i) //dwuwymiarowa tablica dynamiczna
            distances[i]=new int[cityamount];
        for(int i=0;i<cityamount;++i) //zapelnianie tablicy danymi z pliku
            for(int j=0;j<cityamount;++j)
                plik>>distances[i][j];
    }
}

void wypisz()
{
    for(int i=0;i<cityamount;++i)
    {
        for(int j=0;j<cityamount;++j)
            std::cout<<distances[i][j]<<" ";
        std::cout<<"\n";
    }
}



void BruteForce()
{
    int initial, next, current, distance = 0, lvl = 0;
    bool* city = new bool[cityamount]; //tworze dynamicznej tablicy z miastami ktore odwiedzilem
    std::string path= "";
    for (int j = 0; j < cityamount; ++j)
        city[j] = false;

    for(int i = 0; i < cityamount; i++){
        current = next = initial = i;
 //       for (int j = 0; j < cityamount; ++j)
 //           city[j] = false;

//        path = 'A' + i;
        std::cout<<i<<'\n';
        Counter(city, path, distance, current, next, initial, lvl);
    }
    delete[] city;
}
void Counter(bool cityseen[], std::string currentpath, int distance, int current, int next, int initial, int lvl)
{
    bool* city = new bool[cityamount];
    std::string path = currentpath;
    for(int i=0;i<cityamount;++i)
        city[i]=cityseen[i];//w tym momencie wiem juz gdzie bylem, a gdzie musze wejsc

    city[next] = true;
    distance += distances[current][next];
    path += 'A' + next;
    current = next;
    lvl++;

//    std::cout << lvl << "  " << path << ":  " << distance << '\n';

    if( lvl < cityamount)
        for(int i = 0; i < cityamount; i++)
            if(city[i] != true){
                next = i;
                Counter(city, path, distance, current, next, initial, lvl);
            }
    if(lvl == cityamount){
        distance += distances[current][initial];
        path += 'A' + initial;
//        std::cout << path << ":  " << distance << '\n';
        if(shortestdistance > distance){
            shortestdistance = distance;
            shortestpath = path;
        }
    }
    delete[] city;
}
