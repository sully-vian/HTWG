#include <algorithm>
#include <concepts>
#include <functional>
#include <iostream>
#include <sstream>
#include <vector>

template <typename T>
concept PointConcept = requires(T t) {
    { t.x() } -> std::totally_ordered;
    { t.y() } -> std::same_as<decltype(t.x())>;
};

template <PointConcept P>
std::ostream &operator<<(std::ostream &os, const P &point) {
    return os << "(" << point.x() << "," << point.y() << ")";
}

template <PointConcept P> class KDTree {
  public:
    using CoordType = std::remove_cvref_t<decltype(std::declval<P>().x())>;

    KDTree(P point) : point(point), left(nullptr), right(nullptr) {}

    template <typename Container>
    KDTree(Container &elements) : left(nullptr), right(nullptr) {
        if (elements.empty()) {
            throw std::invalid_argument(
                "Cannot construct KDTree from an empty container.");
        }
        int depth = 0;

        auto comparator = [](const P &a, const P &b) { // compare x (depth 0)
            return a.x() < b.x();
        };
        std::sort(elements.begin(), elements.end(), comparator);
        size_t mid = elements.size() / 2;
        this->point = elements[mid];

        std::vector<P> leftElts(elements.begin(), elements.begin() + mid);
        std::vector<P> rightElts(elements.begin() + mid + 1, elements.end());

        this->left = buildInternal(leftElts, depth + 1);
        this->right = buildInternal(rightElts, depth + 1);
    }

    static bool insert(KDTree *&tree, P point) {
        return insertInternal(tree, point, 0);
    }

    static void range(KDTree *tree, CoordType minX, CoordType minY,
                      CoordType maxX, CoordType maxY,
                      const std::function<void(const P &)> &visit) {
        if (!tree) {
            return;
        }
        rangeInternal(tree, minX, minY, maxX, maxY, 0, visit);
    }

    static void traversePartition(
        KDTree *tree, CoordType minX, CoordType maxX, CoordType minY,
        CoordType maxY,
        const std::function<void(const P &, int, CoordType, CoordType,
                                 CoordType, CoordType)> &visit) {
        if (!tree) {
            return;
        }
        traversePartitionInternal(tree, minX, maxX, minY, maxY, 0, visit);
    }

    static bool validate(KDTree *tree) {
        if (!tree) {
            return true;
        }
        std::function<bool(KDTree *, int, const CoordType *, const CoordType *,
                           const CoordType *, const CoordType *)>
            validateRec = [&](KDTree *tree, int depth, const CoordType *minX,
                              const CoordType *maxX, const CoordType *minY,
                              const CoordType *maxY) -> bool {
            if (!tree) {
                return true;
            }

            // check current node against x bounds
            if (minX && tree->point.x() < *minX) {
                std::cerr << "fail minX: " << tree->point << std::endl;
                return false;
            }
            if (maxX && tree->point.x() >= *maxX) {
                std::cerr << "fail maxX: " << tree->point << std::endl;
                return false;
            }

            // check current node against y bounds
            if (minY && tree->point.y() < *minY) {
                std::cerr << "fail minY: " << tree->point << std::endl;
                return false;
            }
            if (maxY && tree->point.y() >= *maxY) {
                std::cerr << "fail maxY: " << tree->point << std::endl;
                return false;
            }

            // prepare bounds for children
            CoordType currentCoord = getCoord(tree->point, depth);
            bool okLeft, okRight;
            if (!(depth % 2)) { // splitting on x
                // x must be < currentCoord. Y bounds stay same.
                okLeft = validateRec(tree->left, depth + 1, minX, &currentCoord,
                                     minY, maxY);
                // x must be >= currentCoord. Y bounds stay same.
                okRight = validateRec(tree->right, depth + 1, &currentCoord,
                                      maxX, minY, maxY);
            } else { // splitting on y
                // y must be < currentCoord. X bounds stay same.
                okLeft = validateRec(tree->left, depth + 1, minX, maxX, minY,
                                     &currentCoord);
                // y must be >= currentCoord. X bounds stay same.
                okRight = validateRec(tree->right, depth + 1, minX, maxX,
                                      &currentCoord, maxY);
            }
            return okLeft && okRight;
        };
        return validateRec(tree, 0, nullptr, nullptr, nullptr, nullptr);
    }

    /*
     * Returns the string representing the tree according to the DOT syntax
     */
    static std::string toDot(KDTree *tree) {
        std::ostringstream oss;
        oss << "digraph G {\n";
        oss << "\tnode [style=filled];\n";

        std::function<void(KDTree *, int depth)> traverse = [&](KDTree *tree,
                                                                int depth) {
            if (!tree) {
                return;
            }

            // 1. Determine splitting dimension (Even=X, Odd=Y)
            bool isX = (depth % 2 == 0);

            // 2. Style the CURRENT node based on the split
            oss << "\t\"" << tree->point << "\" [";
            if (isX) {
                // X-split: Blue-ish color, Box shape, label includes (x)
                oss << "fillcolor=lightblue, shape=box, label=\"" << tree->point
                    << "\\n(x-split)\"";
            } else {
                // Y-split: Yellow-ish color, Ellipse shape, label includes (y)
                oss << "fillcolor=lightyellow, shape=ellipse, label=\""
                    << tree->point << "\\n(y-split)\"";
            }
            oss << "];\n";

            // 3. Draw edges to children
            if (tree->left) {
                oss << "\t\"" << tree->point << "\" -> \"" << tree->left->point
                    << "\"[color=red, label=\"L\", fontcolor=red];\n";
                traverse(tree->left, depth + 1);
            }
            if (tree->right) {
                oss << "\t\"" << tree->point << "\" -> \"" << tree->right->point
                    << "\"[color=green, label=\"R\", fontcolor=green];\n";
                traverse(tree->right, depth + 1);
            }
        };
        traverse(tree, 0);
        oss << "}\n";
        return oss.str();
    };

    static void print(const std::string msg) { std::cout << msg << std::endl; }

  private:
    P point;
    KDTree *left;
    KDTree *right;

    /* ------------------- */
    /* Auxiliary functions */
    /* ------------------- */

    static CoordType getCoord(P point, int depth) {
        return (depth % 2) ? point.y() : point.x();
    }

    template <typename Container>
    static KDTree *buildInternal(Container &elements, int depth) {
        if (elements.empty()) { // check again to be sure
            return nullptr;
        }

        const std::function<bool(const P &, const P &)> comparator =
            [depth](const P &a, const P &b) {
                if (depth % 2 == 0) {
                    return a.x() < b.x();
                } else {
                    return a.y() < b.y();
                }
            };
        std::sort(elements.begin(), elements.end(), comparator);
        size_t mid = elements.size() / 2;

        KDTree *tree = new KDTree(elements[mid]);

        std::vector<P> leftElts(elements.begin(), elements.begin() + mid);
        std::vector<P> rightElts(elements.begin() + mid + 1, elements.end());

        tree->left = buildInternal(leftElts, depth + 1);
        tree->right = buildInternal(rightElts, depth + 1);

        return tree;
    }

    static bool insertInternal(KDTree *&tree, P point, int depth) {
        if (!tree) { // reached a null ptr, create leaf
            tree = new KDTree(point);
            return true;
        }
        if (point == tree->point) { // point exists
            return false;
        }

        // choose which coord to compare depending on depth
        CoordType pointCoord = getCoord(point, depth);
        CoordType nodeCoord = getCoord(tree->point, depth);

        if (pointCoord < nodeCoord) { // descend left
            return insertInternal(tree->left, point, depth + 1);
        } else {                      // descend right
            return insertInternal(tree->right, point, depth + 1);
        }
    }

    static void rangeInternal(KDTree *tree, CoordType minX, CoordType minY,
                              CoordType maxX, CoordType maxY, int depth,
                              const std::function<void(const P &)> &visit) {
        if (!tree) {
            return;
        }

        // check if current node is inside the query range
        if (tree->point.x() >= minX && tree->point.x() <= maxX &&
            tree->point.y() >= minY && tree->point.y() <= maxY) {
            visit(tree->point);
        }

        // determine splitting dimension logic
        CoordType nodeCoord = getCoord(tree->point, depth);
        CoordType boxMin = (depth % 2) ? minY : minX;
        CoordType boxMax = (depth % 2) ? maxY : maxX;

        // left subtree
        if (boxMin < nodeCoord) {
            rangeInternal(tree->left, minX, minY, maxX, maxY, depth + 1, visit);
        }

        // right subtree
        if (boxMax >= nodeCoord) {
            rangeInternal(tree->right, minX, minY, maxX, maxY, depth + 1,
                          visit);
        }
    }

    static void traversePartitionInternal(
        KDTree *tree, CoordType minX, CoordType maxX, CoordType minY,
        CoordType maxY, int depth,
        const std::function<void(const P &, int, CoordType, CoordType,
                                 CoordType, CoordType)> &visit) {
        if (!tree) {
            return;
        }

        visit(tree->point, depth, minX, maxX, minY, maxY);

        CoordType splitVal = getCoord(tree->point, depth);

        if (depth % 2 == 0) { // vertical split (x-axis)
            // Left child gets valid X range: [minX, splitVal]
            traversePartitionInternal(tree->left, minX, splitVal, minY, maxY,
                                      depth + 1, visit);
            // Right child gets valid X range: [splitVal, maxX]
            traversePartitionInternal(tree->right, splitVal, maxX, minY, maxY,
                                      depth + 1, visit);
        } else {
            // Left child gets valid Y range: [minY, splitVal]
            traversePartitionInternal(tree->left, minX, maxX, minY, splitVal,
                                      depth + 1, visit);
            // Right child gets valid Y range: [splitVal, maxY]
            traversePartitionInternal(tree->right, minX, maxX, splitVal, maxY,
                                      depth + 1, visit);
        }
    }
};
