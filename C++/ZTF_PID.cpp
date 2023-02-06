#include <iostream>
#include <valarray>
#include <vector>
#include <chrono>
#include <cmath>
#include <thread>

using namespace std;
using namespace std::chrono;

class ZTF
{

    private:
        vector<double> num;
        vector<double> den;

        double Ts = 0.05;//seconds

        std::valarray<double>  rnum;
        std::valarray<double>  rden;
        std::valarray<double>  zx;
        std::valarray<double>  zy;

        long nsize;
        long dsize;

    public:

        ZTF(const vector<double>& num1, const vector<double>& den1) : num(num1), den(den1)
        {
            nsize=num.size();
            dsize=den.size();

            zx.resize(dsize,0.0);
            zy.resize(dsize,0.0);

            rnum.resize(nsize);
            rden.resize(dsize);

            //reverse order
            for (unsigned int i=0; i<nsize; ++i)
            {
                rnum[i] = num[nsize-1-i];
            }

            //reverse order
            for (unsigned int i=0; i<dsize; ++i)
            {
                rden[i] = den[dsize-1-i];
            }

        }

        void print_coefficients()
        {
            cout << "Numerator Coefficients: ";
            for(unsigned int i = 0; i < nsize; ++i)
            {
                cout << rnum[i]<<" ";
            }
            cout <<endl;

            cout << "Denominator Coefficients: ";
            for(unsigned int i = 0; i < dsize; ++i)
            {
                cout << rden[i]<<" ";
            }
            cout <<endl;
        }

        double processing(double x)
        {
            zx[0] = x;

            std::valarray<double> zx_tmp=zx[std::slice(dsize-nsize,nsize,1)];
            double y = (rnum * zx_tmp).sum();

            //calculate denominator
            //(d0*z**-m + d1*z**-(m-1) + ... + dm)
            y -= (rden * zy).sum();
            y /= rden[0];

            //for the next calculation Z**-1
            zy[0] = y;

            zx = zx.cshift(-1);
            zy = zy.cshift(-1);

            //Z**0
            zy[0] = 0.0;

            return y;
        }
};

class PID
{
    private:
        double P;
        double I;
        double D;
        double N;
        double Ts;


        ZTF ztf1; // Proportional
        ZTF ztf2; // Integrator
        ZTF ztf3; // Derivative

    public:
        PID(double _P, double _I, double _D, double _N, double _Ts): 
            P(_P), I(_I), D(_D), N(_N), Ts(_Ts),  ztf1({_P}, {1.0}), ztf2({_I*_Ts}, {-1.0, 1.0}), ztf3({-_D*_N, _D*_N}, {(_N*_Ts- 1), 1})
        {
          
        }
    
        double processing(double error)
        {
            double y = 0;
            y = ztf1.processing(error) + ztf2.processing(error) + ztf3.processing(error);

            return y;
        }
};

// int main()
// {
    
//     auto start = high_resolution_clock::now();
//     double P = 12.2285752621432;
//     double I = 7.40871870543199;
//     double D = 4.88097496311707;
//     double N = 37.0569659936971;
//     double Ts = 0.05; // seconds
//     PID pid(P, I, D, N, Ts);

//     double out = 0;
//     double x = 0;
    
//     cout <<endl;
//     cout << "Input, Output"<<endl;
//     while(true)
//     {
//         auto elapsed = high_resolution_clock::now() - start;
//         double t = duration_cast<duration<double>>(elapsed).count();

//         // Break out of the loop if we have reached 10 seconds
//         if (t >= 10.0) 
//         {
//               break;
//         }
        

//         x = 3*sin(t);
//         out = pid.processing(x);
//         cout << x << ", "<< out<<endl;
//         std::this_thread::sleep_for(milliseconds(int(Ts*1000)));

//     }

//     return 0;
// }
