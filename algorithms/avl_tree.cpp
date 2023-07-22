#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#define MAX(_a, b) (_a > b ? _a : b)

template <typename T>
struct AVLTreeNode {
    T key;
    AVLTreeNode<T> *left, *right, *parent;
    int height;

    int disbalance();
    void rotate_left();
    void rotate_right();
    void rotate_right_left();
    void rotate_left_right();
};

#define H(_node) (_node ? _node->height : 0)

template <typename T>
int AVLTreeNode<T>::disbalance()
{
    return H(right) - H(left);
}

template <typename T>
void AVLTreeNode<T>::rotate_left()
{
    if (!right)
        return; // @TODO: error

    right->parent = parent;
    parent = right;

    AVLTreeNode<T> tmp = right->left;
    right->left = this;

    right = tmp;
    if (tmp)
        tmp->parent = this;

    height = MAX(H(right), H(left));
    parent->height = MAX(H(parent->right), H(parent->left));
}

template <typename T>
void AVLTreeNode<T>::rotate_right()
{
    if (!left)
        return; // @TODO: error

    left->parent = parent;
    parent = left;

    AVLTreeNode<T> tmp = left->right;
    left->right = this;

    left = tmp;
    if (tmp)
        tmp->parent = this;

    height = MAX(H(right), H(left));
    parent->height = MAX(H(parent->right), H(parent->left));
}

template <typename T>
void AVLTreeNode<T>::rotate_right_left()
{
    if (!right || !right->left)
        return; // @TODO: error

    right->rotate_right();
    rotate_left();
}

template <typename T>
void AVLTreeNode<T>::rotate_left_right()
{
    if (!left || !left->right)
        return; // @TODO: error

    left->rotate_left();
    rotate_right();
}

template <typename T>
class AVLTree {
private:
    AVLTreeNode<T> *m_root;

    AVLTreeNode<T> **m_search_ptr(T key, AVLTreeNode<T> **parent_out);
    void m_rebalance(AVLTreeNode<T> *base);
    void m_print_subtree(AVLTreeNode<T> *root);

public:
    AVLTree();
    bool search(T key);
    void add(T key);
    void remove(T key);
    void print();
};

template <typename T>
AVLTreeNode<T> **AVLTree<T>::m_search_ptr(T key, AVLTreeNode<T> **parent_out)
{
    AVLTreeNode<T> **nodep = &m_root;
    if (parent_out) *parent_out = nullptr;
    while (*nodep) {
        if ((*nodep)->key == key)
            break;

        if (parent_out) *parent_out = *nodep;
        nodep = key < (*nodep)->key ? &((*nodep)->left) : &((*nodep)->right);
    }
    
    return nodep;
}

template <typename T>
void AVLTree<T>::m_rebalance(AVLTreeNode<T> *base)
{
    // @TODO: implement
}

template <typename T>
void AVLTree<T>::m_print_subtree(AVLTreeNode<T> *root)
{
    if (!root)
        return;

    m_print_subtree(root->left);
    std::cout << root->key << '\n';
    m_print_subtree(root->right);
}

template <typename T>
AVLTree<T>::AVLTree()
{
    m_root = nullptr;
}

template <typename T>
bool AVLTree<T>::search(T key)
{
    AVLTreeNode<T> **nodep = m_search_ptr(key, nullptr);
    return *nodep != nullptr;
}

template <typename T>
void AVLTree<T>::add(T key)
{
    AVLTreeNode<T> *parent;
    AVLTreeNode<T> **nodep = m_search_ptr(key, &parent);
    if (*nodep)
        return;

    AVLTreeNode<T> *new_node = (AVLTreeNode<T> *) malloc(sizeof(AVLTreeNode<T>));
    new_node->key = key;
    new_node->left = nullptr;
    new_node->right = nullptr;
    new_node->parent = parent;
    new_node->height = 1;
    *nodep = new_node;

    m_rebalance(new_node);
}

template <typename T>
void AVLTree<T>::remove(T key)
{
    AVLTreeNode<T> **nodep = m_search_ptr(key, nullptr);
    if (!*nodep)
        return;

    // @TODO: regular removal
}

template <typename T>
void AVLTree<T>::print()
{
    m_print_subtree(m_root);
}

int main()
{
    AVLTree<int> tree;
    char c;
    int n;
    for (;;) {
        std::cin >> c;
        if (c == 'p')
            tree.print();
        else {
            std::cin >> n;
            if (c == 'a')
                tree.add(n);
            else if (c == 'r')
                tree.remove(n);
            else if (c == 's')
                printf(tree.search(n) ? "Found\n" : "Not found\n");
        }
    }

    return 0;
}
