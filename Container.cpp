#include "Container.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>

using namespace std;
using namespace std::chrono;

SplayNode::SplayNode(int k, double w)
    : key(k), weight(w), sum(w), l(nullptr), r(nullptr) {}

SplayTree::SplayTree() : root(nullptr) {}

SplayTree::~SplayTree() {
    deleteTree(root);
}

void SplayTree::deleteTree(SplayNode* t) {
    if (!t) return;
    deleteTree(t->l);
    deleteTree(t->r);
    delete t;
}

SplayNode* SplayTree::rotateLeft(SplayNode* t) {
    SplayNode* newRoot = t->r;
    t->r = newRoot->l;
    newRoot->l = t;
    t->sum = t->weight + (t->l ? t->l->sum : 0) + (t->r ? t->r->sum : 0);
    newRoot->sum = newRoot->weight + (newRoot->l ? newRoot->l->sum : 0) + (newRoot->r ? newRoot->r->sum : 0);
    return newRoot;
}

SplayNode* SplayTree::rotateRight(SplayNode* t) {
    SplayNode* newRoot = t->l;
    t->l = newRoot->r;
    newRoot->r = t;
    t->sum = t->weight + (t->l ? t->l->sum : 0) + (t->r ? t->r->sum : 0);
    newRoot->sum = newRoot->weight + (newRoot->l ? newRoot->l->sum : 0) + (newRoot->r ? newRoot->r->sum : 0);
    return newRoot;
}

SplayNode* SplayTree::splay(SplayNode* t, int key) {
    if (!t) return nullptr;
    if (key < t->key) {
        if (!t->l) return t;
        if (key < t->l->key) {
            t->l->l = splay(t->l->l, key);
            t = rotateRight(t);
        } else if (key > t->l->key) {
            t->l->r = splay(t->l->r, key);
            if (t->l->r) t->l = rotateLeft(t->l);
        }
        return (t->l == nullptr) ? t : rotateRight(t);
    } else if (key > t->key) {
        if (!t->r) return t;
        if (key > t->r->key) {
            t->r->r = splay(t->r->r, key);
            t = rotateLeft(t);
        } else if (key < t->r->key) {
            t->r->l = splay(t->r->l, key);
            if (t->r->l) t->r = rotateRight(t->r);
        }
        return (t->r == nullptr) ? t : rotateLeft(t);
    }
    return t;
}

SplayNode* SplayTree::insert(SplayNode* t, int key, double weight) {
    if (!t) return new SplayNode(key, weight);
    t = splay(t, key);
    if (t->key == key) {
        t->weight = weight;
        t->sum = t->weight + (t->l ? t->l->sum : 0) + (t->r ? t->r->sum : 0);
        return t;
    }
    SplayNode* newNode = new SplayNode(key, weight);
    if (key < t->key) {
        newNode->r = t;
        newNode->l = t->l;
        t->l = nullptr;
    } else {
        newNode->l = t;
        newNode->r = t->r;
        t->r = nullptr;
    }
    t->sum = t->weight + (t->l ? t->l->sum : 0) + (t->r ? t->r->sum : 0);
    newNode->sum = newNode->weight + (newNode->l ? newNode->l->sum : 0) + (newNode->r ? newNode->r->sum : 0);
    return newNode;
}

void SplayTree::insert(int key, double weight) {
    root = insert(root, key, weight);
}

SplayNode* SplayTree::erase(SplayNode* t, int key) {
    if (!t) return nullptr;
    t = splay(t, key);
    if (t->key != key) return t;
    if (!t->l) {
        SplayNode* temp = t->r;
        delete t;
        return temp;
    } else {
        SplayNode* temp = t->l;
        t = splay(t->r, key);
        t->l = temp;
        return t;
    }
}

void SplayTree::erase(int key) {
    root = erase(root, key);
}

void SplayTree::print_inorder(SplayNode* t) {
    if (!t) return;
    print_inorder(t->l);
    cout << "Key: " << t->key << " (Weight: " << t->weight << ", Sum: " << t->sum << ")\n";
    print_inorder(t->r);
}

void SplayTree::print() {
    if (!root) cout << "Container is empty.\n";
    else print_inorder(root);
}
SplayNode* SplayTree::select_by_probability(SplayNode* t, double random_value) {
    if (!t) return nullptr;
    double left_sum = (t->l ? t->l->sum : 0);
    
    if (random_value <= left_sum)
        return select_by_probability(t->l, random_value);
    else if (random_value <= left_sum + t->weight)
        return t;
    else
        return select_by_probability(t->r, random_value - left_sum - t->weight);
}

void SplayTree::select_random_by_weight() {
    if (!root) {
        cout << "Container is empty.\n";
        return;
    }
    
    double random_value = (rand() % (int)(root->sum * 100)) / 100.0;
    SplayNode* selected = select_by_probability(root, random_value);
    
    if (selected) {
        cout << "Selected element: Key: " << selected->key << " (Weight: " << selected->weight << ")\n";
    } else {
        cout << "No element selected.\n";
    }
}

void SplayTree::add_random_elements(int n) {
    auto start = high_resolution_clock::now();
    
    for (int i = 0; i < n; ++i) {
        int key = rand() % 1000000;
        double weight = (rand() % 1000 + 1) / 10.0;
        root = insert(root, key, weight);
    }
    
    auto end = high_resolution_clock::now();
    cout << "Added " << n << " random elements in "
         << duration_cast<milliseconds>(end - start).count() << " ms\n";
}
