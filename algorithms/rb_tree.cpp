#include <iostream>
#include <stdio.h>
#include <stdlib.h>

enum NodeColor { red, black };
#define IS_RED(_node) ((_node) && (_node)->color == red)
#define IS_BLACK(_node) (!IS_RED(_node))

template <typename T>
struct RBTreeNode {
    T key;
    NodeColor color;
    RBTreeNode<T> *left, *right, *parent;

    void rotate_left();
    void rotate_right();
};

template <typename T>
void RBTreeNode<T>::rotate_left()
{
    if (!right)
        return;

    right->parent = parent;
    if (parent) {
        if (this == parent->right)
            parent->right = right;
        else
            parent->left = right;
    }
    parent = right;

    RBTreeNode<T> *tmp = right->left;
    right->left = this;

    right = tmp;
    if (tmp)
        tmp->parent = this;
}

template <typename T>
void RBTreeNode<T>::rotate_right()
{
    if (!left)
        return;

    left->parent = parent;
    if (parent) {
        if (this == parent->right)
            parent->right = left;
        else
            parent->left = left;
    }
    parent = left;

    RBTreeNode<T> *tmp = left->right;
    left->right = this;

    left = tmp;
    if (tmp)
        tmp->parent = this;
}

template <typename T>
class RBTree {
private:
    RBTreeNode<T> *m_root;

    RBTreeNode<T> **m_search_ptr(T key, RBTreeNode<T> **parent_out);
    void m_remove_one_child(RBTreeNode<T> **nodep);
    void m_rotate_left(RBTreeNode<T> *pivot);
    void m_rotate_right(RBTreeNode<T> *pivot);
    void m_fix_after_add(RBTreeNode<T> *node);
    void m_fix_after_remove(RBTreeNode<T> *node, RBTreeNode<T> *parent);
    void m_print_subtree(RBTreeNode<T> *root, int depth);

public:
    RBTree();
    bool search(T key);
    void add(T key);
    void remove(T key);
    void print();
};

template <typename T>
RBTreeNode<T> **RBTree<T>::m_search_ptr(T key, RBTreeNode<T> **parent_out)
{
    RBTreeNode<T> **nodep = &m_root;
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
void RBTree<T>::m_rotate_left(RBTreeNode<T> *pivot)
{
    pivot->rotate_left();
    if (!pivot->parent->parent)
        m_root = pivot->parent;
}

template <typename T>
void RBTree<T>::m_rotate_right(RBTreeNode<T> *pivot)
{
    pivot->rotate_right();
    if (!pivot->parent->parent)
        m_root = pivot->parent;
}

template <typename T>
void RBTree<T>::m_fix_after_add(RBTreeNode<T> *node)
{
    if (!node)
        return;

    while (node->parent && node->parent->color == red) {
        RBTreeNode<T> *p = node->parent;
        RBTreeNode<T> *g = p->parent;

        if (p == g->left) {
            RBTreeNode<T> *u = g->right;
            if (IS_RED(u)) {
                p->color = black;
                u->color = black;
                g->color = red;
                node = g;
            } else {
                if (node == p->right) {
                    node = p;
                    m_rotate_left(node);
                    p = node->parent;
                }

                p->color = black;
                g->color = red;
                m_rotate_right(g);
                if (m_root == g)
                    m_root = p;
            }
        } else {
            RBTreeNode<T> *u = g->left;
            if (IS_RED(u)) {
                p->color = black;
                u->color = black;
                g->color = red;
                node = g;
            } else {
                if (node == p->left) {
                    node = p;
                    m_rotate_right(node);
                    p = node->parent;
                }

                p->color = black;
                g->color = red;
                m_rotate_left(g);
                if (m_root == g)
                    m_root = p;
            }
        }
    }

    m_root->color = black;
}

template <typename T>
void RBTree<T>::m_fix_after_remove(RBTreeNode<T> *node, RBTreeNode<T> *parent)
{
    if (!node && !parent)
        return;

    while (node != m_root && IS_BLACK(node)) {
        RBTreeNode<T> *p = node ? node->parent : parent;
        
        if (node == p->left) {
            RBTreeNode<T> *b = p->right;
            if (b->color == red) {
                b->color = black;
                p->color = red;
                m_rotate_left(p);
                b = p->right;
            }
            if (IS_BLACK(b->left) && IS_BLACK(b->right)) {
                b->color = red;
                node = p;
            } else {
                if (IS_BLACK(b->right)) {
                    b->color = red;
                    b->left->color = black;
                    m_rotate_right(b);
                    b = p->right;
                }
                b->color = p->color;
                p->color = black;
                b->right->color = black;
                m_rotate_left(p);
                node = m_root;
            }
        } else {
            RBTreeNode<T> *b = p->left;
            if (b->color == red) {
                b->color = black;
                p->color = red;
                m_rotate_right(p);
                b = p->left;
            }
            if (IS_BLACK(b->left) && IS_BLACK(b->right)) {
                b->color = red;
                node = p;
            } else {
                if (IS_BLACK(b->left)) {
                    b->color = red;
                    b->right->color = black;
                    m_rotate_left(b);
                    b = p->left;
                }
                b->color = p->color;
                p->color = black;
                b->left->color = black;
                m_rotate_right(p);
                node = m_root;
            }
        }
    }

    node->color = black;
}

template <typename T>
void RBTree<T>::m_print_subtree(RBTreeNode<T> *root, int depth)
{
    if (!root)
        return;

    m_print_subtree(root->left, depth+1);
    printf("%*c%d%c\n", depth*4, ' ', root->key, root->color == red ? 'r' : 'b');
    m_print_subtree(root->right, depth+1);
}

template <typename T>
void RBTree<T>::m_remove_one_child(RBTreeNode<T> **nodep)
{
    RBTreeNode<T> *node = *nodep;
    RBTreeNode<T> *next = node->right ? node->right : node->left;

    if (next) {
        if (node->parent)
            next->parent = node->parent;
        else
            m_root = next;
    }

    *nodep = next;
}

template <typename T>
RBTree<T>::RBTree()
{
    m_root = nullptr;
}

template <typename T>
bool RBTree<T>::search(T key)
{
    RBTreeNode<T> **nodep = m_search_ptr(key, nullptr);
    return *nodep != nullptr;
}

template <typename T>
void RBTree<T>::add(T key)
{
    RBTreeNode<T> *parent;
    RBTreeNode<T> **nodep = m_search_ptr(key, &parent);
    if (*nodep)
        return;

    RBTreeNode<T> *new_node = (RBTreeNode<T> *) malloc(sizeof(RBTreeNode<T>));
    new_node->key = key;
    new_node->left = nullptr;
    new_node->right = nullptr;
    new_node->parent = parent;
    new_node->color = red;
    *nodep = new_node;

    m_fix_after_add(*nodep);
}

template <typename T>
void RBTree<T>::remove(T key)
{
    RBTreeNode<T> *parent;
    RBTreeNode<T> **nodep = m_search_ptr(key, &parent);
    if (!*nodep)
        return;

    NodeColor deleted_node_color = (*nodep)->color;

    if (!(*nodep)->right || !(*nodep)->left)
        m_remove_one_child(nodep);
    else {
        RBTreeNode<T> *node = *nodep;
        RBTreeNode<T> *repl = node->right;
        while (repl->left)
            repl = repl->left;

        node->key = repl->key;
        node = repl;
        parent = node->parent;
        deleted_node_color = node->color;
        nodep = node == parent->right ? &parent->right : &parent->left;

        m_remove_one_child(nodep);
    }

    if (deleted_node_color == black)
        m_fix_after_remove(*nodep, parent);
}

template <typename T>
void RBTree<T>::print()
{
    m_print_subtree(m_root, 0);
}

int main()
{
    RBTree<int> tree;
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
