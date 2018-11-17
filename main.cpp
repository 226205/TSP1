#include <fstream>
#include <iostream>
#include <cmath>
//#include "BruteForce.h"

int cityamount=0;
int **distances;
int shortestdistance=INT_MAX;
std::string shortestpath= "";


void wypisz();
void BruteForce();
void BFrecurence(bool*, int*, int, int);
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
    plik.open("plik.txt",std::ios::in);
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
    int visited = 0, distance = 0;
    bool* city = new bool[cityamount];
    int* sequence = new int[cityamount + 1];
    for (int i = 1; i < cityamount; ++i){
        city[i] = false;
        sequence[i] = 0;}
    city[cityamount - 1] = true;
    sequence[0] = sequence[cityamount] = cityamount - 1;

    BFrecurence(city, sequence, distance, visited);
}

void BFrecurence(bool city[],int sequence[], int distance, int visited)
{
    visited++;
    if(visited < cityamount){
        for(int i = 0; i < cityamount; i++){
            if(city[i] != true){
                city[i] = true;
                sequence[visited] = i;
                BFrecurence(city, sequence, distance, visited);
                city[i] = false;
            }
        }
    }
    if(visited == cityamount){
        for(int i = 0; i < cityamount; i++)
            distance += distances[sequence[i]][sequence[i+1]];
        if(shortestdistance > distance){
            shortestdistance = distance;
            shortestpath = "";
            for(int i = 0; i < cityamount + 1; i++){
                shortestpath += 'A' + sequence[i];
            }
        }
    }
}
