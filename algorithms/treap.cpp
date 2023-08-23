#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

float randf()
{
    return (float)rand() / RAND_MAX;
}

template <typename T>
struct TreapNode {
    T key;
    TreapNode<T> *left, *right, *parent;
    float priority;

    TreapNode<T> *rotate_left();
    TreapNode<T> *rotate_right();
};

template <typename T>
TreapNode<T> *TreapNode<T>::rotate_left()
{
    if (!right)
        return nullptr;

    right->parent = parent;
    if (parent) {
        if (this == parent->right)
            parent->right = right;
        else
            parent->left = right;
    }
    parent = right;

    TreapNode<T> *tmp = right->left;
    right->left = this;

    right = tmp;
    if (tmp)
        tmp->parent = this;

    return parent;
}

template <typename T>
TreapNode<T> *TreapNode<T>::rotate_right()
{
    if (!left)
        return nullptr;

    left->parent = parent;
    if (parent) {
        if (this == parent->right)
            parent->right = left;
        else
            parent->left = left;
    }
    parent = left;

    TreapNode<T> *tmp = left->right;
    left->right = this;

    left = tmp;
    if (tmp)
        tmp->parent = this;

    return parent;
}

template <typename T>
class Treap {
private:
    TreapNode<T> *m_root;

    TreapNode<T> **m_search_ptr(T key, TreapNode<T> **parent_out);
    void m_remove_one_child(TreapNode<T> **nodep);
    void m_move_node_up(TreapNode<T> *node);
    void m_move_node_down(TreapNode<T> *node);
    void m_print_subtree(TreapNode<T> *root, int depth);

public:
    Treap();
    bool search(T key);
    void add(T key);
    void remove(T key);
    void print();
};

template <typename T>
TreapNode<T> **Treap<T>::m_search_ptr(T key, TreapNode<T> **parent_out)
{
    TreapNode<T> **nodep = &m_root;
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
void Treap<T>::m_move_node_up(TreapNode<T> *node)
{
    if (!node)
        return;

    while (node->parent && node->parent->priority > node->priority) {
        TreapNode<T> *base;
        if (node == node->parent->right)
            base = node->parent->rotate_left();
        else
            base = node->parent->rotate_right();

        if (!base->parent)
            m_root = base;
    }
}


template <typename T>
inline bool node_is_ligter(TreapNode<T> *node1, TreapNode<T> *node2)
{
    return node1 && (!node2 || node1->priority < node2->priority);
}

template <typename T>
void Treap<T>::m_move_node_down(TreapNode<T> *node)
{
    if (!node)
        return;

    while (node->left || node->right) {
        TreapNode<T> *base;
        if (node_is_ligter(node->right, node->left))
            base = node->rotate_left();
        else
            base = node->rotate_right();

        if (!base->parent)
            m_root = base;
    }
}

template <typename T>
void Treap<T>::m_print_subtree(TreapNode<T> *root, int depth)
{
    if (!root)
        return;

    m_print_subtree(root->left, depth+1);
    printf("%*c%d, p%.2f\n", 4*depth, ' ', root->key, root->priority);
    m_print_subtree(root->right, depth+1);
}

template <typename T>
Treap<T>::Treap()
{
    m_root = nullptr;
}

template <typename T>
bool Treap<T>::search(T key)
{
    TreapNode<T> **nodep = m_search_ptr(key, nullptr);
    return *nodep != nullptr;
}

template <typename T>
void Treap<T>::add(T key)
{
    TreapNode<T> *parent;
    TreapNode<T> **nodep = m_search_ptr(key, &parent);
    if (*nodep)
        return;

    TreapNode<T> *new_node = (TreapNode<T> *) malloc(sizeof(TreapNode<T>));
    new_node->key = key;
    new_node->left = nullptr;
    new_node->right = nullptr;
    new_node->parent = parent;
    new_node->priority = randf();
    *nodep = new_node;

    m_move_node_up(new_node);
}

template <typename T>
void Treap<T>::remove(T key)
{
    TreapNode<T> *parent;
    TreapNode<T> **nodep = m_search_ptr(key, &parent);
    if (!*nodep)
        return;

    TreapNode<T> *node = *nodep;
    m_move_node_down(node);
    parent = node->parent;

    if (!parent)
        m_root = nullptr;
    else if (parent->left == node)
        parent->left = nullptr;
    else
        parent->right = nullptr;

    free(node);
}

template <typename T>
void Treap<T>::print()
{
    m_print_subtree(m_root, 0);
}

int main()
{
    Treap<int> tree;
    char c;
    int n;

    srand(time(nullptr));

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
