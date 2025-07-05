#include <iostream>
#include "binary_search_tree.h"

void ejemplo_1() {
    BinarySearchTree<std::string, int> bt1;
    bt1.insert({"R", 1});
    bt1.insert({"D", 2});
    bt1.insert({"A", 3});
    bt1.insert({"C", 4});
    bt1.insert({"Z", 5});
    bt1.insert({"H", 6});
    for (const auto&[key, value]: bt1) {
        std::cout << key << " " << value << std::endl;
    }
    auto it = bt1.find("X");
    if (it == bt1.end()) {
        std::cout << "Not found" << std::endl;
    } else {
        std::cout << it->first << " " << it->second << std::endl;
    }
}

int main() {
    ejemplo_1();
    return 0;
}
