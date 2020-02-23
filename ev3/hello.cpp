#include <iostream>
#include <chrono>
#include <thread>

using namespace std;
using namespace std::chrono_literals;



int main ()
{
    bool triggered = false;
    string s;
    int n = 0;

    auto start = chrono::steady_clock::now();

    while (true) {
        auto curr = chrono::steady_clock::now();
        if (triggered && 
            chrono::duration_cast<chrono::milliseconds>(curr - start).count() > 1000) {
            break;
        }

        if (s == "a") {
            if (!triggered) triggered = true;
            n++;
            start = chrono::steady_clock::now();
        }

        cin >> s;
    }


    cout <<  "Entries: " << n;

    return 0;
}
