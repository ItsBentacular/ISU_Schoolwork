#include "bst.hpp"
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>

int main(int argc, char *argv[]) {
    bst<int> tree;

    srand(time(NULL));

    for(int i = 0; i < 10000; i++) {
        tree.insert(rand());
    }
    for(int i = -1; i < 10; i++) {
        std::cout << tree.contains(i) << std::endl;
    }

    bst<std::string> sTree;

    sTree.insert("foo");
    sTree.insert("bar");
    sTree.insert("faz");
    sTree.insert("fod");

    std::cout << sTree.contains("foo") << std::endl;
    std::cout << sTree.contains("bar") << std::endl;
    std::cout << sTree.contains("foobar") << std::endl;

    bst<std::vector<int>> vt;
    std::vector<int> x;
    vt.insert(x);
    std::cout << vt.contains(x) << "vector int" << std::endl;
}