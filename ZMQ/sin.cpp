#include <chrono>
#include <cmath>
#include <iostream>
#include <thread>

int main() {
  using namespace std::chrono;

  // Get the current time
  auto start = high_resolution_clock::now();

  // Print the sine function for 10 seconds
  while (true) 
  {
    // Get the elapsed time
    auto elapsed = high_resolution_clock::now() - start;
    double t = duration_cast<duration<double>>(elapsed).count();

    // Break out of the loop if we have reached 10 seconds
    if (t >= 10.0) {
      break;
    }

    // Print the sine of the elapsed time
    std::cout << "sin(" << t << ") = " << std::sin(t) << std::endl;

    // Sleep for 50 milliseconds
    std::this_thread::sleep_for(milliseconds(50));
  }

  return 0;
}
