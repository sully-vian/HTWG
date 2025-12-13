#include "KDTree.cpp"
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

using IPoint = Point<int>;
using IKDTree = KDTree<int>;

/*
 * >>> import numpy as np
 * >>> np.random.randint(0,1001,size=(100,2))
 */
std::vector<IPoint> points = {
    {387, 548},
    {992, 882},
    {193, 516},
    {301, 863},
    {484, 259},
    {724, 556},
    {127, 206},
    {685, 737},
    {853, 178},
    {908, 804},
    {410, 282},
    {353, 936},
    {274, 443},
    {622, 395},
    {724, 234},
    {606, 510},
    {151, 528},
    {797, 153},
    {770, 117},
    {10,  210},
    {881, 523},
    {599, 263},
    {208, 484},
    {363, 155},
    {64,  454},
    {944, 330},
    {10,  187},
    {33,  71 },
    {958, 436},
    {828, 180},
    {901, 874},
    {569, 656},
    {500, 595},
    {967, 628},
    {479, 261},
    {67,  962},
    {971, 693},
    {919, 668},
    {974, 989},
    {759, 435},
    {896, 707},
    {361, 352},
    {478, 754},
    {756, 161},
    {583, 220},
    {593, 488},
    {801, 790},
    {1,   341},
    {117, 417},
    {714, 815},
    {825, 123},
    {840, 522},
    {886, 13 },
    {626, 475},
    {9,   216},
    {758, 586},
    {950, 370},
    {282, 996},
    {640, 391},
    {630, 287},
    {796, 417},
    {0,   412},
    {756, 798},
    {717, 431},
    {349, 957},
    {195, 723},
    {579, 191},
    {746, 785},
    {879, 562},
    {729, 753},
    {15,  844},
    {554, 176},
    {225, 680},
    {705, 748},
    {547, 947},
    {787, 195},
    {225, 402},
    {501, 61 },
    {529, 902},
    {584, 231},
    {450, 199},
    {315, 362},
    {525, 274},
    {267, 131},
    {876, 104},
    {459, 891},
    {319, 96 },
    {373, 987},
    {476, 644},
    {256, 6  },
    {936, 241},
    {145, 945},
    {461, 206},
    {440, 760},
    {894, 972},
    {489, 246},
    {602, 414},
    {947, 910},
    {303, 533},
    {261, 132}
};

// helper
bool contains(const std::vector<IPoint> &list, IPoint point) {
    for (const IPoint &element : list) {
        if (element == point) {
            return true;
        }
    }
    return false;
}

int main() {
    { // test insert
        IKDTree *tree = nullptr;
        for (int i = 0; i < (int)points.size(); i++) {
            IPoint point = points[i];
            // std::cout << point << std::endl;
            bool inserted = IKDTree::insert(tree, point);
            CHECK(tree, inserted);
            CHECK(tree, IKDTree::validate(tree));
        }
    }
    { // test build
        IKDTree *tree = IKDTree::build(points);
        // for (int i = 0; i < (int)points.size(); i++) {
        // IPoint point = points[i];
        // // std::cout << point << std::endl;
        // CHECK(tree, IKDTree::validate(tree));
        // }

        CHECK(tree, IKDTree::validate(tree));
    }
    { // test range query
        IKDTree *tree = nullptr;

        std::vector<IPoint> points = {
            {2,  3 }, // IN
            {5,  4 }, // IN (Boundary Max Y)
            {9,  6 }, // OUT (Too right, too high)
            {4,  7 }, // OUT (Too high)
            {8,  1 }, // IN (Boundary Max X)
            {7,  2 }, // IN
            {1,  1 }, // IN
            {-2, 3 }, // OUT (Too left)
            {5,  -1}  // OUT (Too low)
        };

        for (int i = 0; i < (int)points.size(); i++) {
            IPoint point = points[i];
            // std::cout << point << std::endl;
            bool inserted = IKDTree::insert(tree, point);
            CHECK(tree, inserted);
            CHECK(tree, IKDTree::validate(tree));
        }
        std::vector<IPoint> found;
        // x in [0, 8] and y in [0, 4]
        IKDTree::range(tree, 0, 0, 8, 4,
                       [&](const IPoint &point) { found.push_back(point); });

        // check results
        CHECK(tree, found.size() == 5);
        CHECK(tree, contains(found, {2, 3}));
        CHECK(tree, contains(found, {5, 4}));
        CHECK(tree, contains(found, {8, 1}));
        CHECK(tree, contains(found, {7, 2}));
        CHECK(tree, contains(found, {1, 1}));

        CHECK(tree, !contains(found, {9, 6}));
        CHECK(tree, !contains(found, {-2, 3}));
    }
    return 0;
    {
        IKDTree *tree = nullptr;
        std::ofstream ofs("tree.dot");
        ofs << (tree->toDot());
        ofs.close();
        std::cout << "Dot writtent to tree.dot" << std::endl;
    }

    return 0;
}
