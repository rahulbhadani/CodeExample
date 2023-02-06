#include <iostream>
#include <random>

int main()
{
    std::random_device rd;
    std::mt19937 e2(rd());
    std::normal_distribution<double> dist(70, 10);

    for (int n = 0; n < 100000; ++n) {
        double number = dist(e2);
        std::cout << number << std::endl;
    }
}
