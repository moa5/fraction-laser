#include <iostream>
#include <fstream>

using namespace std;

int main()
{
    double a=1; //parametr opisuj¹cy w³aœciwoœci oœrodka jednorodnego
    double l=10;    //d³ugoœæ badanego `walca
    double tau=1;   //czas ca³kowity symulacji
    double r=10;    //promieñ badanego walca

    double d_n=10;   //liczba przedzia³ów d³ugoœci walca     n=10/ 20/50 -> m=10/ 40/ 250
    double d_m;      //liczba przedzia³ów czasowych
    double d_o=10;      //liczba przedzia³ów promienia walca

    double dx=l/d_n;            //szerokoœæ przedzia³ów d³ugoœci
    double dt=0.1*(dx*dx);      //szerokoœæ przedzia³ów czasowych z warunku stabilnoœci (5.68)
    //double dt=tau/m;
    double dr=r/d_o;      //szerokoœæ przedzia³ów promiania z warunku stabilnoœci (5.68)???

    d_m=tau/dt;
    //d_o=r/dr;

    int n = int (d_n)+1;    //liczba punktów pomiarowych dlugosci
    int m = int (d_m)+1;    //liczba punktów pomiarowych czasu
    int o = int (d_o)+1;    //liczba punktów pomiarowych promienia
 cout<<"#o: "<< o<<", n: "<<n<<", m: "<<m <<endl;
    double tab[o][n][m]={0};


    for(int j=0; j<m; j++)
    {
        for(int i=0; i<n; i++)
        {
            for (int h=0; h<o; h++)
            {
                //warunki brzegowe
                if(i==0 || i==n-1)      //na wieczkach walca chlodzonego z obu stron
                    tab[h][i][j]=0;

                else if(j==0) //if(j==0 && i!=0 && i!=1)        poczatek czasu
                    tab[h][i][j]=10;

                else if (h==0)    //nie na wieczkach i nie na poczatku czasu, ale w środku walca ????
                    tab[h][i][j]= tab[h][i][j-1];   //w czasie poprzednim-proteza
                              //tab[h+1][i][j];

                else if (h==o-1)    //nie na wieczkach i nie na poczatku czasu, ale na jego obrzeżach
                    tab[h][i][j]=tab[h-1][i][j];

                //uzupelnianie
                else tab[h][i][j]= tab[h][i][j-1]+ (a*a*dt)* ((tab[h+1][i][j-1] - 2*tab[h][i][j-1] + tab[h-1][i][j-1])/(dr*dr) + (1/(dr*h*dr))*(tab[h+1][i][j-1]) + (tab[h][i+1][j-1] - 2*tab[h][i][j-1] + tab[h][i-1][j-1])/(dx*dx) );
            }

        }
    }

    //wypisywanie
    fstream plik( "N10.txt", ios::out );
    if( plik.good() )
    {
        plik<<"#promien     #dlugosc     #T"<<endl;

        for(int j=0; j<m; j++)
        {
            for(int i=0; i<n; i++)
            {
                for (int h=0; h<o; h++)
                {
                    if (j==m-1)
                    {
                        cout << dr*h <<"    "<< dx*i <<"    "<< tab[h][i][j]<<endl;
                        plik << dr*h <<"    "<< dx*i <<"    "<< tab[h][i][j]<<endl;
                    }
                }

            }
        }
    }


/*     for(int j=0; j<m; j++)
 *     {
 *         cout<<"t= "<< dt*j<<"   ";
 *
 *         for(int i=0; i<n; i++)
 *         {
 *             if(j==m-1)
 *             {
 *             cout<< tab[i][j]<<" ";
 *
 *             if( plik.good() )
 *             plik << dx*i <<"    "<< tab[i][j]<<endl;
 *             }
 *         }
 *         cout<<endl;
 *     }
 */
plik.close();

    cout <<dx<<"   "<< dt<< "  "<<d_m<<"    "<<m<<"   "<< "Hello world!" << endl;
    return 0;
}





