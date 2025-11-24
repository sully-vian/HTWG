#include "AVLTree.cpp"
#include <cassert>
#include <fstream>
#include <iostream>
#include <random>

#define CHECK(tree, expr)                                                      \
    do {                                                                       \
        if (!(expr)) {                                                         \
            std::cerr << "CHECK failed:" << #expr << " at " << __FILE__ << ":" \
                      << __LINE__ << std::endl;                                \
            if (tree) {                                                        \
                std::ofstream ofs("tree.dot");                                 \
                ofs << tree->toDot();                                          \
                ofs.close();                                                   \
                std::cout << "Dot writtent to tree.dot" << std::endl;          \
            }                                                                  \
            std::abort();                                                      \
        }                                                                      \
    } while (0)

// pseudo-random ints
const int nums[] = {413, 87,  589, 221, 774, 9,   669, 805, 144, 963, 352,
                    291, 827, 462, 68,  936, 112, 647, 306, 745, 428, 93,
                    577, 216, 781, 24,  688, 796, 132, 951, 339, 273, 818,
                    449, 62,  928, 107, 654, 312, 739, 422, 81,  561, 232,
                    769, 17,  660, 802, 140, 958, 345, 288, 823, 466, 74};

int main() {
    { // Test 1: Simple inserts
        AVLTree<int, int> *tree = nullptr;
        for (int i = 1; i <= 50; ++i) {
            bool inserted = AVLTree<int, int>::insert(tree, i, i);
            CHECK(tree, inserted);
            CHECK(tree, (AVLTree<int, int>::validate(tree)));
        }
        CHECK(tree, !(AVLTree<int, int>::insert(tree, 10, 10)));
        CHECK(tree, (AVLTree<int, int>::validate(tree)));
    }

    { // Test 2: Random inserts and removals
        AVLTree<int, int> *tree = nullptr;
        std::mt19937 rng = std::mt19937(12345);
        std::uniform_int_distribution<int> dist =
            std::uniform_int_distribution<int>(1, 1000);
        std::vector<int> insertedNums;
        for (int i = 0; i <= 10; ++i) {
            int k = dist(rng);
            const int inserted = AVLTree<int, int>::insert(tree, k, k);
            if (inserted) {
                insertedNums.push_back(k);
            }
            CHECK(tree, (AVLTree<int, int>::validate(tree)));
        }

        // remove half of the inserted keys
        std::shuffle(insertedNums.begin(), insertedNums.end(), rng);
        int numToRemove = insertedNums.size() / 2;
        for (int i = 0; i < numToRemove; ++i) {
            bool removed = AVLTree<int, int>::remove(tree, insertedNums[i]);
            CHECK(tree, removed);
            CHECK(tree, (AVLTree<int, int>::validate(tree)));
        }
        // Attemp removing non-existing key
        CHECK(tree, !(AVLTree<int, int>::remove(tree, -999)));
        CHECK(tree, (AVLTree<int, int>::validate(tree)));
    }
    { // Test 3: Worst-case pattern (descending inserts)
        AVLTree<int, int> *tree = nullptr;
        for (int i = 1000; i >= 1; --i) {
            AVLTree<int, int>::insert(tree, i, i);
            CHECK(tree, (AVLTree<int, int>::validate(tree)));
        }
        // Remove ascending
        for (int i = 1; i <= 1000; ++i) {
            AVLTree<int, int>::remove(tree, i);
            CHECK(tree, (AVLTree<int, int>::validate(tree)));
        }
        // Tree should be empty now (root == nullptr)
        CHECK(tree, tree == nullptr);
    }
    { // Test 4: Range Search
        AVLTree<int, int> *tree = nullptr;
        std::vector<int> allVals;
        for (int i = 10; i <= 100; i += 10) {
            AVLTree<int, int>::insert(tree, i, i);
            allVals.push_back(i);
        }
        std::vector<int> res;
        std::vector<int> expected;
        const std::function<void(const int &, const int &)> visit =
            [&](const int &k, const int &) { res.push_back(k); };

        // Case 1: Middle range [35, 75] -> {40, 50, 60, 70}
        AVLTree<int, int>::range(tree, 35, 75, visit);
        expected = {40, 50, 60, 70};
        CHECK(tree, res == expected);
        res.clear();

        // Case 2: Range includes boundaries [20, 40] -> {20, 30, 40}
        AVLTree<int, int>::range(tree, 20, 40, visit);
        expected = {20, 30, 40};
        CHECK(tree, res == expected);
        res.clear();

        // Case 3: No overlap (empty)
        AVLTree<int, int>::range(tree, 150, 200, visit);
        CHECK(tree, res.empty());
        res.clear();

        // Case 4: Full range
        res.clear();
        AVLTree<int, int>::range(tree, 0, 200, visit);
        CHECK(tree, res == allVals);
    }
    std::cout << "All AVL tests passed." << std::endl;

    { // print tree
        AVLTree<int, int> *tree = nullptr;
        for (int k : nums) {
            AVLTree<int, int>::insert(tree, k, k);
        }
        CHECK(tree, (AVLTree<int, int>::validate(tree)));
        std::ofstream ofs("tree.dot");
        ofs << tree->toDot();
        ofs.close();
        std::cout << "Dot writtent to tree.dot" << std::endl;
    }

    return 0;
}
