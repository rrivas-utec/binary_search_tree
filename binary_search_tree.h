//
// Created by rrivas on 3/07/2025.
//

#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H
#include <functional>
#include <utility>

enum class Color {Red, Black};


template <
    typename KeyType,
    typename ValueType>
using ItemType = std::pair<KeyType, ValueType>;

template <
    typename KeyType,
    typename ValueType>
struct Node {
    ItemType<KeyType, ValueType> item;
    Color color = Color::Red;
    Node<KeyType, ValueType>* parent = nullptr;
    Node<KeyType, ValueType>* left = nullptr;
    Node<KeyType, ValueType>* right = nullptr;
    Node(ItemType<KeyType, ValueType> item, const Color color, Node<KeyType, ValueType>* parent) :
        item(item),
        color(color),
        parent(parent),
        left(nullptr),
        right(nullptr){}
};

template <
    typename KeyType,
    typename ValueType,
    typename Compare = std::less<KeyType>
    >
class BinarySearchTree {
public:
    BinarySearchTree() = default;
    ~BinarySearchTree() = default;

    class iterator {
        Node<KeyType, ValueType>* node_;
    public:
        explicit iterator(Node<KeyType, ValueType>* node) : node_(node) {}
        bool operator==(const iterator& other) const {
            return node_ == other.node_;
        }
        bool operator!=(const iterator& other) const { return node_ != other.node_; }
        const ItemType<KeyType, ValueType>& operator*() const { return node_->item; }
        const ItemType<KeyType, ValueType>* operator->() const { return &node_->item; }
        iterator& operator++() {
            if (node_->right != nullptr) {
                node_ = node_->right;
                while (node_->left != nullptr) {
                    node_ = node_->left;
                }
            } else {
                auto parent = node_->parent;
                while (parent != nullptr && node_ == parent->right) {
                    node_ = parent;
                    parent = parent->parent;
                }
                node_ = parent;
            }
            return *this;
        }
        iterator& operator--() {
            if (node_->left != nullptr) {
                node_ = node_->left;
                while (node_->right != nullptr) {
                    node_ = node_->right;
                }
            } else {
                auto parent = node_->parent;
                while (parent != nullptr && node_ == parent->left) {
                    node_ = parent;
                    parent = parent->parent;
                }
                node_ = parent;
            }
            return *this;
        }
    };

    iterator insert(ItemType<KeyType, ValueType> item) {
        Node<KeyType, ValueType>** walker = &root_;
        Node<KeyType, ValueType>* parent = nullptr;
        while (*walker != nullptr) {
            parent = *walker;
            if (comp_(item.first, (*walker)->item.first) == true) {
                walker = &((*walker)->left);
            }
            else if (comp_((*walker)->item.first, item.first) == true) {
                walker = &((*walker)->right);
            }
            else {
                (*walker)->item.second = item.second;
                return iterator(*walker);
            }
        }
        *walker = new Node<KeyType, ValueType>(item, Color::Red, parent);
        ++size_;
        fix_insert(*walker);
        return iterator(*walker);
    }

    iterator find(KeyType key) {
        Node<KeyType, ValueType>** walker = &root_;
        while (*walker != nullptr) {
            if (comp_(key, (*walker)->item.first) == true) {
                walker = &((*walker)->left);
            }
            else if (comp_((*walker)->item.first, key) == true) {
                walker = &((*walker)->right);
            }
            else {
                return iterator(*walker);
            }
        }
        return iterator(*walker);
    }

    iterator begin() {
        auto walker = root_;
        while (walker != nullptr && walker->left != nullptr) {
            walker = walker->left;
        }
        return iterator(walker);
    }

    iterator end() {
        return iterator{nullptr};
    }

    void clear() {
        destroy(root_);
        root_ = nullptr;
        size_ = 0;
    }

    [[nodiscard]] size_t size() const {
        return size_;
    }

    bool empty() {
        return root_ == nullptr;
    }
private:
    Node<KeyType, ValueType>* root_ = nullptr;
    Compare comp_;
    size_t size_ = 0;
    void destroy(Node<KeyType, ValueType>* walker) {
        if (walker == nullptr) return;
        destroy(walker->left);
        destroy(walker->right);
        delete walker;
    }
    void fix_insert(Node<KeyType, ValueType>* walker) {
        while (walker != root_ && walker->parent->color == Color::Red) {
            if (walker->parent == walker->parent->parent->left) {
                auto uncle = walker->parent->parent->right;
                // Regla del tio ROJO
                if (uncle != nullptr && uncle->color == Color::Red) {
                    uncle->color = Color::Black;
                    walker->parent->color = Color::Black;
                    walker->parent->parent->color = Color::Red;
                    walker = walker->parent->parent;
                }
                else {
                    if (walker == walker->parent->right) {
                        walker = walker->parent;
                        rotate_left(walker);
                    }
                    walker->parent->color = Color::Black;
                    walker->parent->parent->color = Color::Red;
                    rotate_right(walker->parent->parent);
                }
            }
            else {
                auto uncle = walker->parent->parent->left;
                // Regla del tio ROJO
                if (uncle != nullptr && uncle->color == Color::Red) {
                    uncle->color = Color::Black;
                    walker->parent->color = Color::Black;
                    walker->parent->parent->color = Color::Red;
                    walker = walker->parent->parent;
                }
                else {
                    if (walker == walker->parent->left) {
                        walker = walker->parent;
                        rotate_right(walker);
                    }
                    walker->parent->color = Color::Black;
                    walker->parent->parent->color = Color::Red;
                    rotate_left(walker->parent->parent);
                }
            }
        }
        root_->color = Color::Black;
    }
    void rotate_left(Node<KeyType, ValueType>* x) {
        // Paso #0
        auto y = x->right;
        // Paso #1
        x->right = y->left;
        // Paso #2
        if (y->left != nullptr) {
            y->left->parent = x;
        }
        // Paso #3
        y->parent = x->parent;
        // Paso #4
        if (x->parent == nullptr) {
            root_ = y;
        }
        // Paso #5
        else if (x == x->parent->left) {
            x->parent->left = y;
        }
        else {
            x->parent->right = y;
        }
        // Paso #6
        y->left = x;
        // Paso #7
        x->parent = y;
    }
    void rotate_right(Node<KeyType, ValueType>* x) {
        // Paso #0
        auto y = x->left;
        // Paso #1
        x->left = y->right;
        // Paso #2
        if (y->right != nullptr) {
            y->right->parent = x;
        }
        // Paso #3
        y->parent = x->parent;
        // Paso #4
        if (x->parent == nullptr) {
            root_ = y;
        }
        // Paso #5
        else if (x == x->parent->right) {
            x->parent->right = y;
        }
        else {
            x->parent->left = y;
        }
        // Paso #6
        y->right = x;
        // Paso #7
        x->parent = y;
    }
};


#endif //BINARY_SEARCH_TREE_H
