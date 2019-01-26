#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <math.h>

using namespace std;
void zapisz_do_pliku(double dt, double dr, double dx, double liczba_punktow_czasu, double liczba_punktow_dlugosci, double liczba_punktow_promienia, double** tab_curr);

int main()
{
	double l = 4.95e-3;//!0.001;//1.5e-3;    //d³ugoœæ badanego `walca
   // double tau=1e-5;   //czas ca³kowity symulacji
	double r = 4.5e-3; //! 1.0e-3; //5.6e-4.0;    //promieñ badanego walca
	double tau = 120;
	int czas_terminacji = 24;

    //double liczba_przedzialow_dlugosci=27.0;   //liczba przedzia³ów d³ugoœci walca     n=10/ 20/50 -> m=10/ 40/ 250
	double liczba_przedzialow_czasu = 36000; //!20;
	double liczba_przedzialow_promienia = 120; //! 20.0;      //liczba przedzia³ów promienia walca

	double dt = tau / liczba_przedzialow_czasu; //!1e-5;//0.1*(dx*dx)*1e7;
	double dr = r / liczba_przedzialow_promienia; //224e-7;//2.24e-5.0;// 5.6*pow(10.0, -5.0); // r/d_o;       //szerokoœæ przedzia³ów promiania z warunku stabilnoœci (5.68)???
	double dx = dr;// l / d_n;            //szerokoœæ przedzia³ów d³ugoœci
	 //tau/liczba_przedzialow_czasu;      //szerokoœæ przedzia³ów czasowych z warunku stabilnoœci (5.68)
    //double dt=tau/m;
	double liczba_przedzialow_dlugosci = l / dx;
	//double liczba_przedzialow_czasu = tau / dt;	//12755102040.0;
   // d_m=tau/dt;


	int liczba_punktow_dlugosci = int(liczba_przedzialow_dlugosci) + 1;    //liczba punktów pomiarowych dlugosci
    long liczba_punktow_czasu = long(liczba_przedzialow_czasu)+1 ;    //liczba punktów pomiarowych czasu,wychodza huje muje -2147483647 niezależnie od formuły O.o
	int liczba_punktow_promienia = int(liczba_przedzialow_promienia) + 1;    //liczba punktów pomiarowych promienia
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
        tab_curr[i][liczba_punktow_dlugosci] =0;
    }

	for (int i = 0; i < liczba_punktow_dlugosci; i++)
	{
		for (int k = 0; k < liczba_punktow_promienia; k++)
		{
			tab_curr[k][i] = 37;
			tab_prev[k][i] = 37;
		}
	}

	//parametry skory wlasciwej
	double przewodnosc_skora = 0.37;
	double gestosc_skora = 1116;
	double cieplo_skora = 3300;
	double grubosc_skora = 15e-6;//1.5e-5;

	//parametry krwii
	double gestosc_krew = 1100;
	double cieplo_krew = 3300;
	double temp_krew = 37;
	double wsp_perf_krew = 2e-3;//0.002;

	//zrodla ciepla
	double q_met = 3681e-1;// 368.1;	//[J/m3s]

	double wsp_absorpcji = 2000;//60000; //2000;// 180;	//[1/m]
	double glebokosc_penetracji = 2.8e-4;// 17e-6;	//[m]
	double moc_lasera = 40;	//[W]
	double r_lasera = 1.5e-3;
	double powierzchnia_wiazki = 3.14*r_lasera *r_lasera;
	double natezenie0 = moc_lasera / powierzchnia_wiazki;

	double a = dt/(gestosc_skora*cieplo_skora);

	cout << "dr: " << dr << ", dt: " << dt << ", dx: " << dx << endl;
	cout <<"natezenie0:		" <<natezenie0 << endl;
	//const double x_max = DBL_EPSILON;
	//cout <<"x_max " <<x_max << endl;

    for(long czas=0; czas<liczba_punktow_czasu; czas++)
    {
        for(int wysokosc=0; wysokosc<liczba_punktow_dlugosci; wysokosc++)
        {
			int ile_razy_wchodzi = 0;
			int ile_razy_wchodzi2 = 0;
			for (int szerokosc = 0; szerokosc < liczba_punktow_promienia; szerokosc++)
			{

				if (czas!=0)
                {
					if (szerokosc == 0 || wysokosc == 0)continue;
					else if ((szerokosc * dr < r_lasera))// && (wysokosc * dx < glebokosc_penetracji))      //obszar penetracji wiazki, przy odkomentowaniu, z ograniczeniem glebokosci penetracji
					{
						ile_razy_wchodzi++;
						//cout<<"xxxxxxxxx"<<endl;

						//wstawic (a*przewodnosc_skora) zamiast dt
						tab_curr[szerokosc][wysokosc] = tab_prev[szerokosc][wysokosc] + (przewodnosc_skora *dt / (gestosc_skora*cieplo_skora))
							* ((tab_prev[szerokosc + 1][wysokosc] - 2 * tab_prev[szerokosc][wysokosc]
								+ tab_prev[szerokosc - 1][wysokosc]) / (dr*dr) + (1 / (dr*szerokosc))*(tab_prev[szerokosc + 1][wysokosc] - tab_prev[szerokosc - 1][wysokosc]) / (2 * dr)
								+ (tab_prev[szerokosc][wysokosc + 1] - 2 * tab_prev[szerokosc][wysokosc] + tab_prev[szerokosc][wysokosc - 1]) / (dx*dx))
							+ a * gestosc_krew*wsp_perf_krew*cieplo_krew*(temp_krew - tab_prev[szerokosc][wysokosc])
							+ a * q_met + a * wsp_absorpcji * natezenie0 * exp(-wsp_absorpcji * wysokosc * dx);
					}


                    else if(szerokosc!=liczba_punktow_promienia-1 && wysokosc != liczba_punktow_dlugosci-1)
                    {
						//wstawic (a*przewodnosc_skora) zamiast dt
						tab_curr[szerokosc][wysokosc] = tab_prev[szerokosc][wysokosc] + (a*przewodnosc_skora)
							* ((tab_prev[szerokosc + 1][wysokosc] - 2 * tab_prev[szerokosc][wysokosc]
								+ tab_prev[szerokosc - 1][wysokosc]) / (dr*dr) + (1 / (dr*szerokosc))*(tab_prev[szerokosc + 1][wysokosc] - tab_prev[szerokosc - 1][wysokosc]) / (2 * dr)
								+ (tab_prev[szerokosc][wysokosc + 1] - 2 * tab_prev[szerokosc][wysokosc] + tab_prev[szerokosc][wysokosc - 1]) / (dx*dx))
						 + a*gestosc_krew*wsp_perf_krew*cieplo_krew*(temp_krew-tab_prev[szerokosc][wysokosc])
						 +a*q_met;

						ile_razy_wchodzi2++;
						/*tab_curr[szerokosc][wysokosc] = tab_prev[szerokosc][wysokosc] + (dt)
							* ((tab_prev[szerokosc + 1][wysokosc] - 2 * tab_prev[szerokosc][wysokosc]
								+ tab_prev[szerokosc - 1][wysokosc]) / (dr*dr) + (1 / (dr*szerokosc))*(tab_prev[szerokosc + 1][wysokosc] - tab_prev[szerokosc - 1][wysokosc]) / (2 * dr)
								+ (tab_prev[szerokosc][wysokosc + 1] - 2 * tab_prev[szerokosc][wysokosc] + tab_prev[szerokosc][wysokosc - 1]) / (dx*dx));
                    */
					}
                    else if(szerokosc==liczba_punktow_promienia-1)
                        tab_curr[szerokosc][wysokosc] = tab_curr[szerokosc-1][wysokosc];//nie na wieczkach i nie na poczatku czasu, ale na jego obrzeżach
					else if (wysokosc==liczba_punktow_dlugosci-1)
						tab_curr[szerokosc][wysokosc] = tab_curr[szerokosc][wysokosc-1];

					//if (szerokosc == 1 && wysokosc == 1) tab_curr[szerokosc - 1][wysokosc - 1] = (tab_curr[szerokosc][wysokosc]+) / 2.0;
					if (szerokosc == 1 && wysokosc == 1) tab_curr[szerokosc - 1][wysokosc - 1] = tab_curr[szerokosc][wysokosc];
					if (wysokosc == 1) tab_curr[szerokosc][wysokosc - 1] = tab_curr[szerokosc][wysokosc];
					if (szerokosc == 1)tab_curr[szerokosc - 1][wysokosc] = tab_curr[szerokosc][wysokosc];

                }

            }
        }



		for (int i = 0; i < liczba_punktow_dlugosci; i++)
		{
			for (int k = 0; k < liczba_punktow_promienia; k++)
			{
				//cout.precision(6);// <----------------------------------------------to usunac jak juz bedzie dobrze/ tylko do poprawienia czytelnosci
				cout.precision(10);
				//if (czas == liczba_punktow_czasu-1)
				if (czas == czas_terminacji)//  (int(0.08 / dt))) //0.08s 
				{
					cout << tab_curr[k][i] << "\t";
				}
				tab_prev[k][i] = tab_curr[k][i];
				//cout << tab_prev[k][i]<<"\t";

			}
			//if (czas == liczba_punktow_czasu - 1) cout << endl << endl;
			if (czas == czas_terminacji) cout << endl << endl;
		}

		if (czas == czas_terminacji)
		{
			zapisz_do_pliku(dt, dr, dx, liczba_punktow_czasu, liczba_punktow_dlugosci, liczba_punktow_promienia, tab_curr);
			break;
		}
		//cout << tab_prev[k][i] << "  " << tab_curr[k][i]<<"\t" << k << " " << i << endl;}
            //memcpy( &tab_prev, &tab_curr, sizeof(tab_prev) );
    }



   

    //cout << dr << " " <<dx<<"   "<< dt<< "  "<<liczba_przedzialow_czasu<<"    "<<liczba_punktow_czasu<<"   " << endl;
	cout << endl<< "Program zakonczyl prace." << endl;

	getchar();
    return 0;
}

void zapisz_do_pliku(double dt, double dr, double dx, double liczba_punktow_czasu, double liczba_punktow_dlugosci, double liczba_punktow_promienia, double** tab_curr)
{
	//wypisywanie
	fstream plik("XXXXX.txt", ios::out);
	if (plik.good())
	{
		plik << "czas:	" << dt * liczba_punktow_czasu << endl;
		plik << "#promien     #dlugosc     #T" << endl;

		for (int wysokosc = 0; wysokosc < liczba_punktow_dlugosci; wysokosc++)
		{
			for (int szerokosc = 0; szerokosc < liczba_punktow_promienia; szerokosc++)
			{
				//cout << dr*szerokosc <<"    "<< dx*wysokosc <<"    "<< tab_curr[szerokosc][wysokosc]<<endl;
				plik << dr * szerokosc << "    " << dx * wysokosc << "    " << tab_curr[szerokosc][wysokosc] << endl;
			}
			plik << endl;
		}
		//cout << "bla" << endl;
	}

	plik.close();
}
