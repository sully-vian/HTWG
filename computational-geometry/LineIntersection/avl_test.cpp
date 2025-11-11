#include "AVLTree.cpp"
#include <fstream>
#include <iostream>

const int nums[] = {1, 2, 5, 33, 57, 49, 11, 12, 7};

int main() {
    AVLTree<int, int> t;
    for (const int num : nums) {
        t.insert(num, num);
    }

    // write dot to tree.dot
    const std::string dot = t.toDot();
    std::ofstream out("tree.dot");
    if (!out) { // failed to open file
        return 1;
    }
    out << dot;
    return 0;
}
