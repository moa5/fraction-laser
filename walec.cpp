#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>

using namespace std;

int main()
{
    double a=1; //parametr opisuj¹cy w³aœciwoœci oœrodka jednorodnego
    double l=10;    //d³ugoœæ badanego `walca
    double tau=1;   //czas ca³kowity symulacji
    double r=10;    //promieñ badanego walca
    
    double d_n=20;   //liczba przedzia³ów d³ugoœci walca     n=10/ 20/50 -> m=10/ 40/ 250
    double d_m;      //liczba przedzia³ów czasowych
    double d_o=20;      //liczba przedzia³ów promienia walca
    
    double dx=l/d_n;            //szerokoœæ przedzia³ów d³ugoœci
    double dt=0.1*(dx*dx);      //szerokoœæ przedzia³ów czasowych z warunku stabilnoœci (5.68)
    //double dt=tau/m;
    double dr=r/d_o;       //szerokoœæ przedzia³ów promiania z warunku stabilnoœci (5.68)???
    
    d_m=tau/dt;
    //d_o=r/dr;
    
    int liczba_punktow_dlugosci = int (d_n)+1;    //liczba punktów pomiarowych dlugosci
    int liczba_punktow_czasu = int (d_m)+1;    //liczba punktów pomiarowych czasu
    int liczba_punktow_promienia = int (d_o)+1;    //liczba punktów pomiarowych promienia
    cout<<"#liczba punktów pomiarowych promienia: "<< liczba_punktow_promienia<<", liczba punktów pomiarowych dlugosci: "<<liczba_punktow_dlugosci<<", liczba punktów pomiarowych czasu: "<<liczba_punktow_czasu <<endl;
    //double tab[o][n][m]={0};
    double** tab_prev;
    double** tab_curr;
    //double** tab_curr = new double[o][n]={0};
    tab_prev = new double*[liczba_punktow_promienia];
    tab_curr = new double*[liczba_punktow_promienia];
    for(int i=0; i<liczba_punktow_promienia; i++)
    {
        tab_prev[i] = new double[liczba_punktow_dlugosci];
        tab_prev[i][liczba_punktow_dlugosci] = 0;
        tab_curr[i] = new double[liczba_punktow_dlugosci];
        tab_curr[i][liczba_punktow_dlugosci] = 0;
    }
    
    for(int czas=0; czas<liczba_punktow_czasu; czas++)
    {
        cout << "czas: " << czas << endl;
        for(int wysokosc=0; wysokosc<liczba_punktow_dlugosci; wysokosc++)
        {
            for (int szerokosc=0; szerokosc<liczba_punktow_promienia; szerokosc++)
            {
                //warunki brzegowe
                if(wysokosc==0 || wysokosc==liczba_punktow_dlugosci-1)      //na wieczkach walca chlodzonego z obu stron
                {
                    tab_curr[szerokosc][wysokosc]=0;
                }
                else if(czas==0)       //poczatek czasu
                {
                    tab_curr[szerokosc][wysokosc]=10;
                }
                //else if(szerokosc==0)continue;
                else
                {
                    if(szerokosc==0)continue;
                    else if(szerokosc!=liczba_punktow_promienia-1)
                    {
                      tab_curr[szerokosc][wysokosc] = tab_prev[szerokosc][wysokosc] + (a*a*dt) * ((tab_prev[szerokosc+1][wysokosc] - 2*tab_prev[szerokosc][wysokosc] + tab_prev[szerokosc-1][wysokosc])/(dr*dr) + (1/(dr*szerokosc))*(tab_prev[szerokosc+1][wysokosc]-tab_prev[szerokosc-1][wysokosc])/(2*dr) + (tab_prev[szerokosc][wysokosc+1] - 2*tab_prev[szerokosc][wysokosc] + tab_prev[szerokosc][wysokosc-1])/(dx*dx));
                    }
                    else if(szerokosc==liczba_punktow_promienia-1)
                        tab_curr[szerokosc][wysokosc] = tab_curr[szerokosc-1][wysokosc];//nie na wieczkach i nie na poczatku czasu, ale na jego obrzeżach
                    
                    if (szerokosc==1)tab_curr[szerokosc-1][wysokosc] = tab_curr[szerokosc][wysokosc];
                }
            }
        }
        

        for(int i=0; i<liczba_punktow_dlugosci; i++){
            for(int k=0; k<liczba_punktow_promienia; k++){
                cout.precision(4);// <----------------------------------------------to usunac jak juz bedzie dobrze/ tylko do poprawienia czytelnosci
                cout << tab_curr[k][i]<<"\t";
                tab_prev[k][i] = tab_curr[k][i];
                //cout << tab_prev[k][i]<<"\t";
            }
            cout<<endl;
        }
                //cout << tab_prev[k][i] << "  " << tab_curr[k][i]<<"\t" << k << " " << i << endl;}
            //memcpy( &tab_prev, &tab_curr, sizeof(tab_prev) );
    }
    
    //wypisywanie
    fstream plik( "N1111.txt", ios::out );
    if( plik.good() )
    {
        cout << " bla bla" << endl;
        plik<<"#promien     #dlugosc     #T"<<endl;
        
        for(int wysokosc=0; wysokosc<liczba_punktow_dlugosci; wysokosc++)
        {
            for (int szerokosc=0; szerokosc<liczba_punktow_promienia; szerokosc++)
            {
                //cout << dr*szerokosc <<"    "<< dx*wysokosc <<"    "<< tab_curr[szerokosc][wysokosc]<<endl;
                plik << dr*szerokosc <<"    "<< dx*wysokosc <<"    "<< tab_curr[szerokosc][wysokosc]<<endl;
            }
            plik<<endl;
        }
        cout << "bla" << endl;
    }
    
    plik.close();
    cout << dr << " " <<dx<<"   "<< dt<< "  "<<d_m<<"    "<<liczba_punktow_czasu<<"   " << endl;
    
    return 0;
}
