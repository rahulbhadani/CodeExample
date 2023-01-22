#include <iostream>
#include <cmath>
#include <complex>
using namespace std;
const int N = 10;

void z_trans_PID(double x[], std::complex<double> X[], std::complex<double> U[], int N, double P, double I, double D, double T) 
{
    std::complex<double> z = std::polar(1.0, 2 * M_PI / N);
    cout <<"z ="<< z<<endl;
    std::complex<double> Tz;
    for (int k = 0; k < N; k++) 
    {
        X[k] = 0;
        for (int i = 0; i < N; i++) 
        {
            X[k] += x[i] * std::pow(z, -i * k);
        }
        Tz = (( P + ((I*T)/2) + (D/T) )*(z*z) +  ( -P + ((I*T)/2) - ((2*D)/T) )*z  + (D/T) )/ ( (z*z) - z);
        U[k] = Tz*X[k];
    }
}

void z_transform(double x[], std::complex<double> X[], int n) 
{
    std::complex<double> z = std::polar(1.0, 2 * M_PI / N);
    cout <<"z ="<< z<<endl;
    for (int k = 0; k < n; k++) 
    {
        X[k] = 0;
        for (int i = 0; i < N; i++) 
        {
            X[k] += x[i] * std::pow(z, -i * k);
        }
    }
}

int main() 
{
    double x[N] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::complex<double> X[N];
    std::complex<double> U[N];
    double P = 1;
    double I = 1;
    double D = 1;
    double T = 0.05;

   // z_transform(x, X, N);
    z_trans_PID(x, X, U, N, P, I, D, T);

    for (int i = 0; i < N; i++) 
    {
        cout << "X["<<i<<"]" << X[i] << endl;
        cout << "U["<<i<<"]" << U[i] << endl;
    }

    return 0;
}



// #include <iostream>
// #include <cmath>
// #include <algorithm>
// #include <numeric>
// #include <complex>

// using namespace std;

// const int N = 10;#include <cmath>

// //void z_transform(double x[], double X[], int n) {
// void z_transform(std::vector<double, N> &x, std::vector<double, N> &X)
// {
//     // pre-calculate the powers of z
//     std::vector<double> z_pow(N);
// std::transform(std::begin(x), std::end(x), std::begin(z_pow), std::begin(X),
//                 [](double a, double b) { return a*b; });


// }

// int main() {
//     //double x[N] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
//     //`double X[N];

//     std::vector<double> x = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
//     std::vector<double> X;


//     z_transform(x, X, N);

//     for (int i = 0; i < N; i++) {
//         cout << X[i] << endl;
//     }

//     return 0;
// }

