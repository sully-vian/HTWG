#include <algorithm>
#include <functional>
#include <iostream>
#include <sstream>
#include <vector>

template <typename CoordType> struct Point {
    CoordType x;
    CoordType y;

    Point(CoordType x, CoordType y) : x(x), y(y) {}

    bool operator==(const Point &other) const {
        return x == other.x && y == other.y;
    }

    friend std::ostream &operator<<(std::ostream &os, const Point &point) {
        return os << "(" << point.x << "," << point.y << ")";
    }

    /*
     * Helper that reaturns x on even depths and y otherwise
     */
    CoordType getCoord(int depth) const {
        return (depth % 2) ? this->y : this->x;
    }
};

template <typename CoordType> class KDTree {
  public:
    KDTree(Point<CoordType> point)
        : point(point), left(nullptr), right(nullptr) {}

    static KDTree *build(std::vector<Point<CoordType>> &elements) {
        if (elements.empty()) {
            return nullptr;
        }
        return buildInternal(elements, 0);
    }

    static bool insert(KDTree *&tree, Point<CoordType> point) {
        return insertInternal(tree, point, 0);
    }

    static void
    range(KDTree *tree, CoordType minX, CoordType minY, CoordType maxX,
          CoordType maxY,
          const std::function<void(const Point<CoordType> &)> &visit) {
        rangeInternal(tree, minX, minY, maxX, maxY, 0, visit);
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
            if (minX && tree->point.x < *minX) {
                std::cerr << "fail minX: " << tree->point << std::endl;
                return false;
            }
            if (maxX && tree->point.x >= *maxX) {
                std::cerr << "fail maxX: " << tree->point << std::endl;
                return false;
            }

            // check current node against y bounds
            if (minY && tree->point.y < *minY) {
                std::cerr << "fail minY: " << tree->point << std::endl;
                return false;
            }
            if (maxY && tree->point.y >= *maxY) {
                std::cerr << "fail maxY: " << tree->point << std::endl;
                return false;
            }

            // prepare bounds for children
            CoordType currentCoord = tree->point.getCoord(depth);
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
    std::string toDot() {
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
        traverse(this, 0);
        oss << "}\n";
        return oss.str();
    };

    static void print(const std::string msg) { std::cout << msg << std::endl; }

  private:
    Point<CoordType> point;
    KDTree *left;
    KDTree *right;

    /* ------------------- */
    /* Auxiliary functions */
    /* ------------------- */

    static KDTree *buildInternal(std::vector<Point<CoordType>> &elements,
                                 int depth) {
        if (elements.empty()) { // check again to be sure
            return nullptr;
        }

        const std::function<bool(const Point<CoordType> &,
                                 const Point<CoordType> &)>
            comparator =
                [depth](const Point<CoordType> &a, const Point<CoordType> &b) {
                    if (depth % 2 == 0) {
                        return a.x < b.x;
                    } else {
                        return a.y < b.y;
                    }
                };
        std::sort(elements.begin(), elements.end(), comparator);
        size_t mid = elements.size() / 2;

        KDTree *tree = new KDTree(elements[mid]);

        std::vector<Point<CoordType>> leftElts(elements.begin(),
                                               elements.begin() + mid);
        std::vector<Point<CoordType>> rightElts(elements.begin() + mid + 1,
                                                elements.end());

        tree->left = buildInternal(leftElts, depth + 1);
        tree->right = buildInternal(rightElts, depth + 1);

        return tree;
    }

    static bool insertInternal(KDTree *&tree, Point<CoordType> point,
                               int depth) {
        if (!tree) { // reached a null ptr, create leaf
            tree = new KDTree(point);
            return true;
        }
        if (point == tree->point) { // point exists
            return false;
        }

        // choose which coord to compare depending on depth
        CoordType pointCoord = point.getCoord(depth);
        CoordType nodeCoord = tree->point.getCoord(depth);

        if (pointCoord < nodeCoord) { // descend left
            return insertInternal(tree->left, point, depth + 1);
        } else {                      // descend right
            return insertInternal(tree->right, point, depth + 1);
        }
    }

    static void
    rangeInternal(KDTree *tree, CoordType minX, CoordType minY, CoordType maxX,
                  CoordType maxY, int depth,
                  const std::function<void(const Point<CoordType> &)> &visit) {
        if (!tree) {
            return;
        }

        // check if current node is inside the query range
        if (tree->point.x >= minX && tree->point.x <= maxX &&
            tree->point.y >= minY && tree->point.y <= maxY) {
            visit(tree->point);
        }

        // determine splitting dimension logic
        CoordType nodeCoord = tree->point.getCoord(depth);
        CoordType boxMin = Point<CoordType>(minX, minY).getCoord(depth);
        CoordType boxMax = Point<CoordType>(maxX, maxY).getCoord(depth);

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
};
