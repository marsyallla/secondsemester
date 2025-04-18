#include "Container.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>

using namespace std;
using namespace std::chrono;

int main()
{
    srand(time(0));
    SplayTree tree;

    int choice;
    while (true) {
        cout << "\n1. Add an element\n";
        cout << "2. Remove an element\n";
        cout << "3. Show all elements\n";
        cout << "4. Add random elements\n";
        cout << "5. Select random element by weight probability\n";
        cout << "0. Exit\n";
        cout << "Choice: ";
        cin >> choice;

        if (choice == 0)
            break;
        else if (choice == 1) {
            int x;
            double w;
            cout << "Key (x): ";
            cin >> x;
            cout << "Weight: ";
            cin >> w;
            auto start = high_resolution_clock::now();
            tree.insert(x, w);
            auto end = high_resolution_clock::now();
            cout << "Insertion took "
                 << duration_cast<microseconds>(end - start).count() << " us\n";
        } else if (choice == 2) {
            int x;
            cout << "Key (x): ";
            cin >> x;
            auto start = high_resolution_clock::now();
            tree.erase(x);
            auto end = high_resolution_clock::now();
            cout << "Erase took "
                 << duration_cast<microseconds>(end - start).count() << " us\n";
        } else if (choice == 3) {
            tree.print();
        } else if (choice == 4) {
            int n;
            cout << "Number of random elements: ";
            cin >> n;
            tree.add_random_elements(n);
        } else if (choice == 5) {
            tree.select_random_by_weight();
        }
    }

    return 0;
}
