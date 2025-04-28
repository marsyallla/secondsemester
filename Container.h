#ifndef CONTAINER_H
#define CONTAINER_H

struct SplayNode {
    int key;
    double weight;
    double sum;
    SplayNode* l;
    SplayNode* r;

    SplayNode(int k, double w);
};

class SplayTree {
private:
    SplayNode* root;

    SplayNode* rotateLeft(SplayNode* t);
    SplayNode* rotateRight(SplayNode* t);
    SplayNode* splay(SplayNode* t, int key);
    SplayNode* insert(SplayNode* t, int key, double weight);
    SplayNode* erase(SplayNode* t, int key);
    void print_inorder(SplayNode* t);
    SplayNode* select_by_probability(SplayNode* t, double random_value);
    void deleteTree(SplayNode* t);
    void updateSum(SplayNode* t);
    SplayNode* updateSums(SplayNode* t);

public:
    SplayTree();
    ~SplayTree();

    void insert(int key, double weight);
    void erase(int key);
    void print();
    void add_random_elements(int n);
    void select_random_by_weight();
};

#endif
