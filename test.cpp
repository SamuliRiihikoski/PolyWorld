#include <iostream>
#include <stdio.h>
#include <string>
using namespace std;

struct Opiskelija 
{
    string etunimi, sukunimi, opiskelijanumero;
    int opintopisteet;
};

Opiskelija luoOpiskelija()
{
    Opiskelija opiskelija;
    cout << "Anna opiskelijan etunimi: ";
    cin >> opiskelija.etunimi;
    cout << "Anna opiskelijan sukunimi: ";
    cin >> opiskelija.sukunimi;
     cout << "Anna opiskelijan opiskelijanumero: ";
    cin >> opiskelija.opiskelijanumero;
     cout << "Anna opiskelijan opintopisteet: ";
    cin >> opiskelija.opintopisteet;

    return opiskelija;
}

int main()
{
    Opiskelija opiskelijat[20];
    int lkm;
    cout << "Kuinka monelle opiskelijalle luodaan tiedot? (MAX 20): ";
    cin >> lkm;

    for (int i = 0; i < lkm; i++)
        opiskelijat[i] = luoOpiskelija();

    for (int i = 0; i < lkm; i++)
    {
        cout << "Opiskelijan numero " << i + 1 << " tiedot." << endl;
        cout << "Etunimi: " << opiskelijat[i].etunimi << endl;
        cout << "Sukunimi: " << opiskelijat[i].sukunimi << endl;
        cout << "Opiskelijanumero: " << opiskelijat[i].opiskelijanumero << endl;
        cout << "Opintopisteet: " << opiskelijat[i].opintopisteet << endl;
    }
}