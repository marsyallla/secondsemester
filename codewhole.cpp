#include <bits/stdc++.h>
#include <chrono>
#include <cstdlib>
using namespace std;
using namespace std::chrono;

struct Node {
    int key;
    double weight;
    double sum;
    int prior;
    Node *l, *r;

    Node(int k, double w) : key(k), weight(w), sum(w), prior(rand()), l(nullptr), r(nullptr) {}
};

double getSum(Node* t) {
    return t ? t->sum : 0.0;
}

void recalc(Node* t) {
    if (t)
        t->sum = t->weight + getSum(t->l) + getSum(t->r);
}

void split(Node* t, int key, Node*& l, Node*& r) {
    if (!t) {
        l = r = nullptr;
    } else if (key < t->key) {
        split(t->l, key, l, t->l);
        r = t;
    } else {
        split(t->r, key, t->r, r);
        l = t;
    }
    recalc(t);
}

Node* merge(Node* l, Node* r) {
    if (!l || !r) return l ? l : r;
    if (l->prior > r->prior) {
        l->r = merge(l->r, r);
        recalc(l);
        return l;
    } else {
        r->l = merge(l, r->l);
        recalc(r);
        return r;
    }
}

Node* insert(Node* t, int key, double weight) {
    Node *l, *r;
    split(t, key, l, r);
    Node *tl, *tr;
    split(r, key + 1, tl, tr);
    if (tl) delete tl;
    Node* newNode = new Node(key, weight);
    return merge(merge(l, newNode), tr);
}

Node* erase(Node* t, int key) {
    Node *l, *r;
    split(t, key, l, r);
    Node *tl, *tr;
    split(r, key + 1, tl, tr);
    if (tl) delete tl;
    return merge(l, tr);
}

int random_pick(Node* t, double rnd) {
    if (!t) return -1;
    double lsum = getSum(t->l);
    if (rnd < lsum)
        return random_pick(t->l, rnd);
    rnd -= lsum;
    if (rnd < t->weight)
        return t->key;
    rnd -= t->weight;
    return random_pick(t->r, rnd);
}

int pick(Node* t) {
    double total = getSum(t);
    if (total == 0) return -1;
    double rnd = ((double) rand() / RAND_MAX) * total;
    return random_pick(t, rnd);
}

void print_inorder(Node* t) {
    if (!t) return;
    print_inorder(t->l);
    cout << "x" << t->key << " (w=" << t->weight << ")" << endl;
    print_inorder(t->r);
}

void addRandomElements(Node*& root, int n) {
    for (int i = 0; i < n; ++i) {
        int key = rand() % 1000000;
        double weight = (rand() % 1000 + 1) / 10.0;
        root = insert(root, key, weight);
    }
}

int main() {
    srand(time(0));
    Node* root = nullptr;

    int choice;
    while (true) {
        cout << "\n1. Add an element\n";
        cout << "2. Remove an element\n";
        cout << "3. Random choice\n";
        cout << "4. Show the elements\n";
        cout << "5. Add random elements\n";
        cout << "0. Exit\n";
        cout << "Choice: ";
        cin >> choice;

        if (choice == 0) break;
        else if (choice == 1) {
            int x;
            double w;
            cout << "Key (x): ";
            cin >> x;
            cout << "Weight: ";
            cin >> w;
            auto start = high_resolution_clock::now();
            root = insert(root, x, w);
            auto end = high_resolution_clock::now();
            cout << "Insertion took " << duration_cast<microseconds>(end - start).count() << " us\n";
        }
        else if (choice == 2) {
            int x;
            cout << "Key (x): ";
            cin >> x;
            auto start = high_resolution_clock::now();
            root = erase(root, x);
            auto end = high_resolution_clock::now();
            cout << "Erase took " << duration_cast<microseconds>(end - start).count() << " us\n";
        }
        else if (choice == 3) {
            auto start = high_resolution_clock::now();
            int res = pick(root);
            auto end = high_resolution_clock::now();
            if (res == -1) cout << "Container is empty.\n";
            else cout << "X is chosen: " << res << endl;
            cout << "Pick took " << duration_cast<microseconds>(end - start).count() << " us\n";
            }
        else if (choice == 4) {
            if (!root) cout << "Container is empty.\n";
            else print_inorder(root);
        }
        else if (choice == 5) {
            int n;
            cout << "Number of random elements: ";
            cin >> n;
            auto start = high_resolution_clock::now();
            addRandomElements(root, n);
            auto end = high_resolution_clock::now();
            cout << "Added " << n << " random elements in "
                 << duration_cast<milliseconds>(end - start).count() << " ms\n";
        }
        else {
            cout << "Invalid choice, try again.\n";
        }
    }

    return 0;
}
