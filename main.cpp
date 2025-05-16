#include "Container.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>

using namespace std;
using namespace std::chrono;

void test_sampling() {
    SplayTree tree;
    cout << "\nTest 1: Single element\n";
    tree.insert(1, 1.0);
    for (int i = 0; i < 100; i++) {
        double rv = ((double)rand() / RAND_MAX) * tree.getRoot()->sum;
        SplayNode* selected = tree.select_by_probability(rv);
        if (!selected || selected->key != 1) {
            cout << "Error: selected wrong element or nullptr\n";
        }
    }
    tree.erase(1);

    cout << "\nTest 2: Two equal elements\n";
    tree.insert(1, 1.0);
    tree.insert(2, 1.0);
    int count1 = 0, count2 = 0;
    for (int i = 0; i < 1000; i++) {
        double rv = ((double)rand() / RAND_MAX) * tree.getRoot()->sum;
        SplayNode* selected = tree.select_by_probability(rv);
        if (!selected) continue;
        if (selected->key == 1) count1++;
        else if (selected->key == 2) count2++;
        else cout << "Unexpected key selected: " << selected->key << "\n";
    }
    cout << "Element 1 frequency: " << count1 / 10.0 << "%\n";
    cout << "Element 2 frequency: " << count2 / 10.0 << "%\n";
    tree.erase(1);
    tree.erase(2);

    cout << "\nTest 3: Three elements with different weights\n";
    tree.insert(1, 0.5);
    tree.insert(2, 1.0);
    tree.insert(3, 1.5);
    count1 = count2 = 0;
    int count3 = 0;
    for (int i = 0; i < 1000; i++) {
        double rv = ((double)rand() / RAND_MAX) * tree.getRoot()->sum;
        SplayNode* selected = tree.select_by_probability(rv);
        if (!selected) continue;
        if (selected->key == 1) count1++;
        else if (selected->key == 2) count2++;
        else if (selected->key == 3) count3++;
        else cout << "Unexpected key selected: " << selected->key << "\n";
    }
    cout << "Element 1 frequency: " << count1 / 10.0 << "% (expected ~16.67%)\n";
    cout << "Element 2 frequency: " << count2 / 10.0 << "% (expected ~33.33%)\n";
    cout << "Element 3 frequency: " << count3 / 10.0 << "% (expected ~50.00%)\n";

    tree.erase(1);
    tree.erase(2);
    tree.erase(3);
}

void test_weight_change() {
    SplayTree tree;
    cout << "\nTest: Change weights and sample\n";
    tree.insert(1, 1.0);
    tree.insert(2, 2.0);
    tree.insert(3, 3.0);

    int count1 = 0, count2 = 0, count3 = 0;
    const int samples = 10000;
    for (int i = 0; i < samples; ++i) {
        double p = (rand() % 6000) / 1000.0; // от 0 до 6
        SplayNode* selected = tree.select_by_probability(p);
        if (selected->key == 1) count1++;
        else if (selected->key == 2) count2++;
        else if (selected->key == 3) count3++;
    }
    cout << "Initial weights: 1=1.0, 2=2.0, 3=3.0\n";
    cout << "Frequencies:\n";
    cout << "Key 1: " << (count1 * 100.0 / samples) << "%\n";
    cout << "Key 2: " << (count2 * 100.0 / samples) << "%\n";
    cout << "Key 3: " << (count3 * 100.0 / samples) << "%\n";
    tree.insert(1, 4.0); 
    tree.insert(3, 1.0); 
    count1 = count2 = count3 = 0;
    for (int i = 0; i < samples; ++i) {
        double p = (rand() % 7000) / 1000.0; 
        SplayNode* selected = tree.select_by_probability(p);
        if (selected->key == 1) count1++;
        else if (selected->key == 2) count2++;
        else if (selected->key == 3) count3++;
    }
    cout << "After weight change: 1=4.0, 2=2.0, 3=1.0\n";
    cout << "Frequencies:\n";
    cout << "Key 1: " << (count1 * 100.0 / samples) << "%\n";
    cout << "Key 2: " << (count2 * 100.0 / samples) << "%\n";
    cout << "Key 3: " << (count3 * 100.0 / samples) << "%\n";
    tree.erase(1);
    tree.erase(2);
    tree.erase(3);
}


void test_removal_sampling() {
    SplayTree tree;
    cout << "\nTest 5: Remove a key and check sampling frequencies\n";
    tree.insert(1, 1.0);
    tree.insert(2, 3.0);
    tree.insert(3, 6.0);

    tree.erase(2);

    int count1 = 0, count3 = 0;
    double total_weight = tree.getRoot()->sum;

    for (int i = 0; i < 1000; i++) {
        double rv = ((double)rand() / RAND_MAX) * total_weight;
        SplayNode* selected = tree.select_by_probability(rv);
        if (!selected) continue;
        if (selected->key == 1) count1++;
        else if (selected->key == 3) count3++;
        else cout << "Unexpected key selected: " << selected->key << "\n";
    }

    cout << "Key 1 frequency: " << count1 / 10.0 << "% (expected ~10%)\n";
    cout << "Key 3 frequency: " << count3 / 10.0 << "% (expected ~90%)\n";

    tree.erase(1);
    tree.erase(3);
}

void test_add_random_and_erase() {
    SplayTree tree;
    cout << "\nTest 6: Add random element via add_random_elements(1) and then erase it\n";
    tree.add_random_elements(1);

    int key = tree.getRoot() ? tree.getRoot()->key : -1;
    if (key == -1) {
        cout << "No element added\n";
        return;
    }

    cout << "Randomly added element key: " << key << "\n";

    tree.erase(key);

    if (tree.getRoot() == nullptr) {
        cout << "Successfully removed the only element added randomly.\n";
    } else {
        cout << "Failed to remove the element, tree not empty.\n";
    }
}

void test_performance() {
    SplayTree tree;
    const int min_power = 5;
    const int max_power = 20;
    cout << "N\tInsert Time (ms)\tSample Time (us)\n";

    for (int power = min_power; power <= max_power; power++) {
        int n = 1 << power;
        tree = SplayTree();

        auto start_insert = high_resolution_clock::now();
        tree.add_random_elements(n);
        auto end_insert = high_resolution_clock::now();
        auto insert_time = duration_cast<milliseconds>(end_insert - start_insert).count();
        auto start_sample = high_resolution_clock::now();
        for (int i = 0; i < 1000; i++) {
            tree.select_random_by_weight();
        }
        auto end_sample = high_resolution_clock::now();
        auto sample_time = duration_cast<microseconds>(end_sample - start_sample).count() / 1000.0;
        
        cout << n << "\t" << insert_time << "\t\t\t" << sample_time << "\n";
    }
}

int main() {
    srand(time(0));
    SplayTree tree;

    int choice;
    while (true) {
        cout << "\n1. Add an element\n";
        cout << "2. Remove an element\n";
        cout << "3. Show all elements\n";
        cout << "4. Add random elements\n";
        cout << "5. Select random element by weight probability\n";
        cout << "6. Run sampling tests\n";
        cout << "7. Run edge case tests\n";
        cout << "8. Run performance tests\n";
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
        } else if (choice == 6) {
            test_sampling();
        } else if (choice == 7) {
            test_weight_change();
        } else if (choice == 8) {
            test_performance();
        } else {
            cout << "Invalid choice, try again.\n";
        }
    }

    return 0;
}
