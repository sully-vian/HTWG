#include <algorithm>
#include <functional>
#include <iostream>
#include <sstream>
#include <vector>

template <typename KeyType, typename ValueType> class AVLTree {
  public:
    AVLTree(KeyType key, ValueType value)
        : key(key), val(value), height(0), left(nullptr), right(nullptr),
          pred(nullptr), succ(nullptr) {};

    static bool insert(AVLTree *&tree, KeyType key, ValueType v) {
        AVLTree *pred = nullptr;
        AVLTree *succ = nullptr;
        return insertInternal(tree, key, v, pred, succ);
    }

    static bool remove(AVLTree *&tree, KeyType key) {
        return removeInternal(tree, key);
    }

    // use the visit function to add the elements to a container
    static void range(
        AVLTree *tree, KeyType lo, KeyType hi,
        const std::function<void(const KeyType &, const ValueType &)> &visit) {
        if (!tree) {
            return;
        }

        // find start node
        AVLTree *start = nullptr;
        while (tree) {
            if (lo <= tree->key) {
                start = tree;
                tree = tree->left;
            } else {
                tree = tree->right;
            }
        }
        while (start && start->key < lo) {
            start = start->succ;
        }
        for (AVLTree *n = start; n && n->key <= hi; n = n->succ) {
            visit(n->key, n->val);
        }
    }

    static bool validate(AVLTree *&tree) {
        if (!tree) {
            return true;
        }
        bool ok = true;
        std::vector<AVLTree *> inOrder;

        std::function<int(AVLTree *)> dfs = [&](AVLTree *n) -> int {
            if (!n)
                return -1;
            int lh = dfs(n->left);
            inOrder.push_back(n);
            int rh = dfs(n->right);

            // check height
            int expected = std::max(lh, rh) + 1;
            if (n->height != expected) {
                ok = false;
            }

            // check balance
            int bf = rh - lh;
            if (bf < -1 || bf > 1) {
                ok = false;
            }

            // check local ordering
            if (n->left) {
                if (n->left->key >= n->key) {
                    ok = false;
                }
            }
            if (n->right) {
                if (n->right->key <= n->key) {
                    ok = false;
                }
            }
            return expected;
        };
        dfs(tree);

        // check in-order strict increase and successor chain alignment
        for (size_t i = 0; i < inOrder.size(); ++i) {
            AVLTree *curr = inOrder[i];
            AVLTree *prev = (i > 0) ? inOrder[i - 1] : nullptr;
            AVLTree *next = (i < inOrder.size() - 1) ? inOrder[i + 1] : nullptr;

            // check doubly-linked list integrity
            if (curr->pred != prev) {
                ok = false;
            }
            if (curr->succ != next) {
                ok = false;
            }
            // check sorting
            if (next && curr->key >= next->key) {
                ok = false;
            }
        }
        return ok;
    }

    /*
     * Returns the string representing the tree according to the DOT syntax
     */
    std::string toDot() {
        std::ostringstream oss;
        oss << "digraph G {\n";

        std::function<void(AVLTree *)> traverse = [&](AVLTree *tree) {
            if (!tree) {
                return;
            }

            // Left child
            if (tree->left) {
                oss << "\t\"" << tree->key << "\" -> \"" << tree->left->key
                    << "\"[color=red, label=\"L\", fontcolor=red];\n";
                traverse(tree->left);
            }

            // Right child
            if (tree->right) {
                oss << "\t\"" << tree->key << "\" -> \"" << tree->right->key
                    << "\"[color=green, label=\"R\", fontcolor=green];\n";
                traverse(tree->right);
            }

            // successor
            if (tree->succ) {
                oss << "\t\"" << tree->key << "\" -> \"" << tree->succ->key
                    << "\"[style=dashed, color=blue];\n";
            }
            // predecessor
            if (tree->pred) {
                oss << "\t\"" << tree->key << "\" -> \"" << tree->pred->key
                    << "\"[style=dotted, color=gray];\n";
            }
        };

        traverse(this);
        oss << "}\n";
        return oss.str();
    };

    static void print(const std::string msg) { std::cout << msg << std::endl; }

  private:
    KeyType key;
    ValueType val;
    int height;
    AVLTree *left;
    AVLTree *right;
    AVLTree *pred; // in-order predecessor
    AVLTree *succ; // in-order successor

    /* ------------------- */
    /* Auxiliary functions */
    /* ------------------- */

    static int getHeight(const AVLTree *tree) {
        return tree ? tree->height : -1;
    }

    static int getBalance(const AVLTree *tree) {
        return tree ? getHeight(tree->right) - getHeight(tree->left) : 0;
    }

    static bool insertInternal(AVLTree *&tree, KeyType key, ValueType v,
                               AVLTree *&pred, AVLTree *&succ) {
        bool inserted;
        if (!tree) { // key does not exist in the tree
            tree = new AVLTree(key, v);
            balance(tree);
            tree->succ = succ;
            tree->pred = pred;
            if (pred) {
                pred->succ = tree;
            }
            if (succ) {
                succ->pred = tree;
            }
            inserted = true;
        } else if (key < tree->key) { // smaller, insert left
            succ = tree;
            inserted = insertInternal(tree->left, key, v, pred, succ);
        } else if (key > tree->key) { // larger, insert right
            pred = tree;
            inserted = insertInternal(tree->right, key, v, pred, succ);
        } else {                      // key exists, nothing to do
            inserted = false;
        }

        if (inserted) {
            balance(tree);
        }
        return inserted;
    }

    static bool removeInternal(AVLTree *&tree, KeyType key) {
        bool removed;
        if (!tree) {
            removed = false;
        } else if (key < tree->key) {
            removed = removeInternal(tree->left, key);

        } else if (key > tree->key) {
            removed = removeInternal(tree->right, key);
        } else {

            // found node
            AVLTree *pred = tree->pred;
            AVLTree *succ = tree->succ;
            if (!tree->left || !tree->right) { // leaf or one child

                // child is null if tree is leaf
                AVLTree *child = tree->left ? tree->left : tree->right;

                // update both directions of linked list
                if (pred) {
                    pred->succ = tree->succ;
                }
                if (succ) {
                    succ->pred = tree->pred;
                }
                delete tree;
                tree = child; // replace tree with the child

            } else {          // two children: copy succ data then remove succ

                // successor cannot be null because tree has right subtree
                KeyType succKey = tree->succ->key;
                ValueType succVal = tree->succ->val;

                // recursively removes successor
                removeInternal(tree->right, succKey);

                // replace tree with the values from succ
                tree->key = succKey;
                tree->val = succVal;
            }
            removed = true;
        }
        // balance on the way up after recursive calls
        if (removed) {
            balance(tree);
        }
        return removed;
    }

    static void rotateRight(AVLTree *&tree) {
        AVLTree *q = tree->left;
        tree->left = q->right;
        q->right = tree;
        tree->height =
            std::max(getHeight(tree->left), getHeight(tree->right)) + 1;
        q->height = std::max(getHeight(q->left), getHeight(q->right)) + 1;
        tree = q;
    }

    static void rotateLeft(AVLTree *&tree) {
        AVLTree *q = tree->right;
        tree->right = q->left;
        q->left = tree;
        tree->height =
            std::max(getHeight(tree->left), getHeight(tree->right)) + 1;
        q->height = std::max(getHeight(q->left), getHeight(q->right)) + 1;
        tree = q;
    }

    static void rotateLeftRight(AVLTree *&tree) {
        rotateLeft(tree->left);
        rotateRight(tree);
    }

    static void rotateRightLeft(AVLTree *&tree) {
        rotateRight(tree->right);
        rotateLeft(tree);
    }

    // balance the tree(only
    static void balance(AVLTree *&tree) {
        if (!tree) {
            return;
        }

        // update height
        tree->height =
            std::max(getHeight(tree->left), getHeight(tree->right)) + 1;

        int b = getBalance(tree);
        if (b == -2) { // left heavy
            if (getBalance(tree->left) <= 0) {
                rotateRight(tree);
            } else {
                rotateLeftRight(tree);
            }
        } else if (b == 2) { // right heavy
            if (getBalance(tree->right) >= 0) {
                rotateLeft(tree);
            } else {
                rotateRightLeft(tree);
            }
        }
    }
};
