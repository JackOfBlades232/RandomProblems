#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#define MAX(_a, _b) (_a > _b ? _a : _b)
#define ABS(_a) (_a >= 0 ? _a : -_a)

template <typename T>
struct AVLTreeNode {
    T key;
    AVLTreeNode<T> *left, *right, *parent;
    int height;

    int disbalance();
    void recalculate_height();
    AVLTreeNode<T> *rotate_left();
    AVLTreeNode<T> *rotate_right();
    AVLTreeNode<T> *rotate_right_left();
    AVLTreeNode<T> *rotate_left_right();
};

#define H(_node) (_node ? _node->height : 0)

template <typename T>
int AVLTreeNode<T>::disbalance()
{
    return H(right) - H(left);
}

template <typename T>
void AVLTreeNode<T>::recalculate_height()
{
    height = MAX(H(right), H(left)) + 1;
}

template <typename T>
AVLTreeNode<T> *AVLTreeNode<T>::rotate_left()
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

    AVLTreeNode<T> *tmp = right->left;
    right->left = this;

    right = tmp;
    if (tmp)
        tmp->parent = this;

    recalculate_height();
    parent->recalculate_height();

    return parent;
}

template <typename T>
AVLTreeNode<T> *AVLTreeNode<T>::rotate_right()
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

    AVLTreeNode<T> *tmp = left->right;
    left->right = this;

    left = tmp;
    if (tmp)
        tmp->parent = this;

    recalculate_height();
    parent->recalculate_height();

    return parent;
}

template <typename T>
AVLTreeNode<T> *AVLTreeNode<T>::rotate_right_left()
{
    if (!right || !right->left)
        return nullptr;

    right->rotate_right();
    return rotate_left();
}

template <typename T>
AVLTreeNode<T> *AVLTreeNode<T>::rotate_left_right()
{
    if (!left || !left->right)
        return nullptr;

    left->rotate_left();
    return rotate_right();
}

template <typename T>
class AVLTree {
private:
    AVLTreeNode<T> *m_root;

    AVLTreeNode<T> **m_search_ptr(T key, AVLTreeNode<T> **parent_out);
    void m_remove_one_child(AVLTreeNode<T> **nodep);
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
    if (!base)
        return;

    int prev_height = base->height;
    AVLTreeNode<T> *grandparent = base->parent;

    base->recalculate_height();
    int disbalance = base->disbalance();
    if (ABS(disbalance) > 1) {
        AVLTreeNode<T> *new_base;

        if (disbalance > 0) {
            if (base->right->disbalance() < 0)
                new_base = base->rotate_right_left();
            else
                new_base = base->rotate_left();
        }
        else {
            if (base->left->disbalance() > 0)
                new_base = base->rotate_left_right();
            else
                new_base = base->rotate_right();
        }

        if (new_base && !new_base->parent)
            m_root = new_base;
    } 

    if (prev_height == base->height)
        return;

    m_rebalance(grandparent);
}

template <typename T>
void AVLTree<T>::m_print_subtree(AVLTreeNode<T> *root)
{
    if (!root)
        return;

    m_print_subtree(root->left);
    printf("%d, h%d, d%d\n", root->key, root->height, root->disbalance());
    m_print_subtree(root->right);
}

template <typename T>
void AVLTree<T>::m_remove_one_child(AVLTreeNode<T> **nodep)
{
    AVLTreeNode<T> *node = *nodep;
    AVLTreeNode<T> *next = node->right ? node->right : node->left;

    if (next) {
        if (node->parent)
            next->parent = node->parent;
        else
            m_root = next;
    }

    *nodep = next;
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

    m_rebalance(parent);
}

template <typename T>
void AVLTree<T>::remove(T key)
{
    AVLTreeNode<T> *parent;
    AVLTreeNode<T> **nodep = m_search_ptr(key, &parent);
    if (!*nodep)
        return;

    if (!(*nodep)->right || !(*nodep)->left)
        m_remove_one_child(nodep);
    else {
        AVLTreeNode<T> *node = *nodep;
        AVLTreeNode<T> *repl = node->right;
        while (repl->left)
            repl = repl->left;

        node->key = repl->key;
        node = repl;
        parent = node->parent;
        nodep = node == parent->right ? &parent->right : &parent->left;

        m_remove_one_child(nodep);
    }

    m_rebalance(parent);
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
