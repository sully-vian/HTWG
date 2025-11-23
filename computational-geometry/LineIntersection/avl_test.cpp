#include "AVLTree.cpp"
#include <cassert>
#include <fstream>
#include <iostream>
#include <random>

#define CHECK(root, expr)                                                      \
    do {                                                                       \
        if (!(expr)) {                                                         \
            std::cerr << "CHECK failed:" << #expr << " at " << __FILE__ << ":" \
                      << __LINE__ << std::endl;                                \
            if (root) {                                                        \
                std::ofstream ofs("tree.dot");                                 \
                ofs << root->toDot();                                          \
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
        Node<int, int> *root = nullptr;
        for (int i = 1; i <= 50; ++i) {
            bool inserted = Node<int, int>::insert(root, i, i);
            CHECK(root, inserted);
            CHECK(root, (Node<int, int>::validate(root)));
        }
        CHECK(root, !(Node<int, int>::insert(root, 10, 10)));
        CHECK(root, (Node<int, int>::validate(root)));
    }

    { // Test 2: Random inserts and removals
        Node<int, int> *root = nullptr;
        std::mt19937 rng = std::mt19937(12345);
        std::uniform_int_distribution<int> dist =
            std::uniform_int_distribution<int>(1, 1000);
        std::vector<int> insertedNums;
        for (int i = 0; i <= 10; ++i) {
            int k = dist(rng);
            const int inserted = Node<int, int>::insert(root, k, k);
            if (inserted) {
                insertedNums.push_back(k);
            }
            CHECK(root, (Node<int, int>::validate(root)));
        }

        // remove half of the inserted keys
        std::shuffle(insertedNums.begin(), insertedNums.end(), rng);
        int numToRemove = insertedNums.size() / 2;
        for (int i = 0; i < numToRemove; ++i) {
            bool removed = Node<int, int>::remove(root, insertedNums[i]);
            CHECK(root, removed);
            CHECK(root, (Node<int, int>::validate(root)));
        }
        // Attemp removing non-existing key
        CHECK(root, !(Node<int, int>::remove(root, -999)));
        CHECK(root, (Node<int, int>::validate(root)));
    }
    { // Test 3: Worst-case pattern (descending inserts)
        Node<int, int> *root = nullptr;
        for (int i = 1000; i >= 1; --i) {
            Node<int, int>::insert(root, i, i);
            CHECK(root, (Node<int, int>::validate(root)));
        }
        // Remove ascending
        for (int i = 1; i <= 1000; ++i) {
            Node<int, int>::remove(root, i);
            CHECK(root, (Node<int, int>::validate(root)));
        }
        // Tree should be empty now (root == nullptr)
        CHECK(root, root == nullptr);
    }
    { // Test 4: Range Search
        Node<int, int> *root = nullptr;
        std::vector<int> allVals;
        for (int i = 10; i <= 100; i += 10) {
            Node<int, int>::insert(root, i, i);
            allVals.push_back(i);
        }
        std::vector<int> res;
        std::vector<int> expected;
        const std::function<void(const int &, const int &)> visit =
            [&](const int &k, const int &) { res.push_back(k); };

        // Case 1: Middle range [35, 75] -> {40, 50, 60, 70}
        Node<int, int>::range(root, 35, 75, visit);
        expected = {40, 50, 60, 70};
        CHECK(root, res == expected);
        res.clear();

        // Case 2: Range includes boundaries [20, 40] -> {20, 30, 40}
        Node<int, int>::range(root, 20, 40, visit);
        expected = {20, 30, 40};
        CHECK(root, res == expected);
        res.clear();

        // Case 3: No overlap (empty)
        Node<int, int>::range(root, 150, 200, visit);
        CHECK(root, res.empty());
        res.clear();

        // Case 4: Full range
        res.clear();
        Node<int, int>::range(root, 0, 200, visit);
        CHECK(root, res == allVals);
    }
    std::cout << "All AVL tests passed." << std::endl;

    { // print tree
        Node<int, int> *root = nullptr;
        for (int k : nums) {
            Node<int, int>::insert(root, k, k);
        }
        CHECK(root, (Node<int, int>::validate(root)));
        std::ofstream ofs("tree.dot");
        ofs << root->toDot();
        ofs.close();
        std::cout << "Dot writtent to tree.dot" << std::endl;
    }

    return 0;
}
