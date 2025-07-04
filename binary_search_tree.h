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
    };

    iterator insert(ItemType<KeyType, ValueType> item) {
        Node<KeyType, ValueType>** walker = &root_;
        Node<KeyType, ValueType>* parent = nullptr;
        while (*walker != nullptr) {
            parent = *walker;
            if (comp(item.first, (*walker)->item.first) == true) {
                walker = &((*walker)->left);
            }
            else if (comp((*walker)->item.first, item.first) == true) {
                walker = &((*walker)->right);
            }
            else {
                (*walker)->item.second = item.second;
                return iterator(*walker);
            }
        }
        *walker = new Node<KeyType, ValueType>(item, Color::Red, parent);
        return iterator(*walker);
    }

    iterator find(KeyType key) {
        Node<KeyType, ValueType>** walker = &root_;
        while (*walker != nullptr) {
            if (comp(key, (*walker)->item.first) == true) {
                walker = &((*walker)->left);
            }
            else if (comp((*walker)->item.first, key) == true) {
                walker = &((*walker)->right);
            }
            else {
                return iterator(*walker);
            }
        }
        return iterator(*walker);
    }

    iterator begin() {
        return iterator{};
    }

    iterator end() {
        return iterator{};
    }

    void clear() {
    }

    size_t size() {
        return 0;
    }

    bool empty() {
        return false;
    }
private:
    Node<KeyType, ValueType>* root_;
    Compare comp;
};


#endif //BINARY_SEARCH_TREE_H
