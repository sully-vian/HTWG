#include <algorithm>
#include <functional>
#include <iostream>
#include <sstream>
#include <vector>

template <typename KeyType, typename ValueType> class Node {
  public:
    Node(KeyType key, ValueType value)
        : key(key), val(value), height(0), left(nullptr), right(nullptr),
          pred(nullptr), succ(nullptr) {};

    static bool insert(Node *&root, KeyType key, ValueType v) {
        Node *pred = nullptr;
        Node *succ = nullptr;
        return insertInternal(root, key, v, pred, succ);
    }

    static bool remove(Node *&root, KeyType key) {
        return removeInternal(root, key);
    }

    // use the visit function to add the elements to a container
    static void range(
        Node *root, KeyType lo, KeyType hi,
        const std::function<void(const KeyType &, const ValueType &)> &visit) {
        if (!root) {
            return;
        }
        Node *start = lowerBound(root, lo);
        while (start && start->key < lo) {
            start = start->succ;
        }
        for (Node *n = start; n && n->key <= hi; n = n->succ) {
            visit(n->key, n->val);
        }
    }

    static bool validate(Node *&root) {
        if (!root) {
            return true;
        }
        bool ok = true;
        std::vector<Node *> inOrder;

        std::function<int(Node *)> dfs = [&](Node *n) -> int {
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
        dfs(root);

        // check in-order strict increase and successor chain alignment
        for (size_t i = 0; i < inOrder.size(); ++i) {
            Node *curr = inOrder[i];
            Node *prev = (i > 0) ? inOrder[i - 1] : nullptr;
            Node *next = (i < inOrder.size() - 1) ? inOrder[i + 1] : nullptr;

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

        std::function<void(Node *)> traverse = [&](Node *node) {
            if (!node) {
                return;
            }

            // Left child
            if (node->left) {
                oss << "\t\"" << node->key << "\" -> \"" << node->left->key
                    << "\"[color=red, label=\"L\", fontcolor=red];\n";
                traverse(node->left);
            }

            // Right child
            if (node->right) {
                oss << "\t\"" << node->key << "\" -> \"" << node->right->key
                    << "\"[color=green, label=\"R\", fontcolor=green];\n";
                traverse(node->right);
            }

            // successor
            if (node->succ) {
                oss << "\t\"" << node->key << "\" -> \"" << node->succ->key
                    << "\"[style=dashed, color=blue];\n";
            }
            // predecessor
            if (node->pred) {
                oss << "\t\"" << node->key << "\" -> \"" << node->pred->key
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
    Node *left;
    Node *right;
    Node *pred; // in-order predecessor
    Node *succ; // in-order successor

    /* ------------------- */
    /* Auxiliary functions */
    /* ------------------- */

    static int getHeight(const Node *p) { return p ? p->height : -1; }

    static int getBalance(const Node *p) {
        return p ? getHeight(p->right) - getHeight(p->left) : 0;
    }

    static bool insertInternal(Node *&p, KeyType key, ValueType v, Node *&pred,
                               Node *&succ) {
        bool inserted;
        if (!p) { // key does not exist in the tree
            p = new Node(key, v);
            balance(p);
            p->succ = succ;
            p->pred = pred;
            if (pred) {
                pred->succ = p;
            }
            if (succ) {
                succ->pred = p;
            }
            inserted = true;
        } else if (key < p->key) { // smaller, insert left
            succ = p;
            inserted = insertInternal(p->left, key, v, pred, succ);
        } else if (key > p->key) { // larger, insert right
            pred = p;
            inserted = insertInternal(p->right, key, v, pred, succ);
        } else {                   // key exists, nothing to do
            inserted = false;
        }

        if (inserted) {
            balance(p);
        }
        return inserted;
    }

    static bool removeInternal(Node *&p, KeyType key) {
        bool removed;
        if (!p) {
            removed = false;
        } else if (key < p->key) {
            removed = removeInternal(p->left, key);

        } else if (key > p->key) {
            removed = removeInternal(p->right, key);
        } else {

            // found node
            Node *pred = p->pred;
            Node *succ = p->succ;
            if (!p->left || !p->right) {                    // leaf or one child

                Node *child = p->left ? p->left : p->right; // null if p is leaf

                // update both directions of linked list
                if (pred) {
                    pred->succ = p->succ;
                }
                if (succ) {
                    succ->pred = p->pred;
                }
                delete p;
                p = child; // replace p with the child

            } else {       // two children: copy succ data then remove succ

                // successor cannot be null because p has right subtree
                KeyType succKey = p->succ->key;
                ValueType succVal = p->succ->val;

                // recursively removes successor
                removeInternal(p->right, succKey);

                // replace p with the values from succ
                p->key = succKey;
                p->val = succVal;
            }
            removed = true;
        }
        // balance on the way up after recursive calls
        if (removed) {
            balance(p);
        }
        return removed;
    }

    // Lower bound (first node with key >= k)
    static Node *lowerBound(Node *root, KeyType k) {
        Node *candidate = nullptr;
        while (root) {
            if (k <= root->key) {
                candidate = root;
                root = root->left;
            } else {
                root = root->right;
            }
        }
        return candidate;
    }

    static void rotateRight(Node *&p) {
        Node *q = p->left;
        p->left = q->right;
        q->right = p;
        p->height = std::max(getHeight(p->left), getHeight(p->right)) + 1;
        q->height = std::max(getHeight(q->left), getHeight(q->right)) + 1;
        p = q;
    }

    static void rotateLeft(Node *&p) {
        Node *q = p->right;
        p->right = q->left;
        q->left = p;
        p->height = std::max(getHeight(p->left), getHeight(p->right)) + 1;
        q->height = std::max(getHeight(q->left), getHeight(q->right)) + 1;
        p = q;
    }

    static void rotateLeftRight(Node *&p) {
        rotateLeft(p->left);
        rotateRight(p);
    }

    static void rotateRightLeft(Node *&p) {
        rotateRight(p->right);
        rotateLeft(p);
    }

    // balance the tree(only
    static void balance(Node *&p) {
        if (!p) {
            return;
        }

        // update height
        p->height = std::max(getHeight(p->left), getHeight(p->right)) + 1;

        int b = getBalance(p);
        if (b == -2) { // left heavy
            if (getBalance(p->left) <= 0) {
                rotateRight(p);
            } else {
                rotateLeftRight(p);
            }
        } else if (b == 2) { // right heavy
            if (getBalance(p->right) >= 0) {
                rotateLeft(p);
            } else {
                rotateRightLeft(p);
            }
        }
    }
};
