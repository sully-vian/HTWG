#include <algorithm>
#include <functional>
#include <iostream>
#include <sstream>
#include <vector>

template <typename KeyType, typename ValueType> class Node {
  public:
    Node(KeyType key, ValueType value)
        : key(key), val(value), height(0), left(nullptr), right(nullptr),
          next(nullptr) {};

    static bool insert(Node *&root, KeyType key, ValueType v) {
        Node *pred = nullptr;
        Node *succ = nullptr;
        return insertInternal(root, key, v, pred, succ);
    }

    static bool remove(Node *&root, KeyType key) {
        return removeInternal(root, root, key);
    }

    static void range(
        Node *root, KeyType lo, KeyType hi,
        const std::function<void(const KeyType &, const ValueType &)> &visit) {
        if (!root) {
            return;
        }
        Node *start = lowerBound(root, lo);
        while (start && start->key < lo) {
            start = start->next;
        }
        for (Node *n = start; n && n->key <= hi; n = n->next) {
            visit(n->key, n->val);
        }
    }

    static void search(void) {
        // TODO
    }

    // TODO: finish
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
            if (i + 1 < inOrder.size()) {
                if (inOrder[i]->next != inOrder[i + 1]) {
                    ok = false;
                }
                if (inOrder[i]->key >= inOrder[i + 1]->key) {
                    ok = false;
                }
            } else {
                if (inOrder[i]->next != nullptr) {
                    ok = false;
                }
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
            if (node->next) {
                oss << "\t\"" << node->key << "\" -> \"" << node->next->key
                    << "\"[style=dashed, color=blue];\n";
            }
        };

        traverse(this);
        oss << "}\n";
        return oss.str();
    };

    void print(const char msg[]) { std::cout << msg << std::endl; }

  private:
    KeyType key;
    ValueType val;
    int height;
    Node *left;
    Node *right;
    Node *next; // in-order successor

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
            p->next = succ;
            if (pred) {
                pred->next = p;
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

    static Node *findPred(Node *root, KeyType key) {
        Node *pred = nullptr;
        while (root) {
            if (key < root->key) {
                root = root->left;
            } else if (key > root->key) {
                pred = root;
                root = root->right;
            } else { // found
                break;
            }
        }
        return pred;
    }

    static bool removeInternal(Node *&p, Node *root, KeyType key) {
        bool removed = false;
        if (!p) {
            removed = false;

        } else if (key < p->key) {
            removed = removeInternal(p->left, root, key);

        } else if (key > p->key) {
            removed = removeInternal(p->right, root, key);

        } else {                         // found node
            Node *pred = findPred(root, key);
            if (!p->left && !p->right) { // leaf
                if (pred) {
                    pred->next = p->next;
                }
                delete p;
                p = nullptr;

            } else if (!p->left) { // only right child
                if (pred) {
                    pred->next = p->next;
                }
                Node *tmp = p;
                p = p->right;
                delete tmp;

            } else if (!p->right) { // only right children
                if (pred) {
                    pred->next = p->next;
                }
                Node *tmp = p;
                p = p->left;
                delete tmp;

            } else { // two children
                // find successor (left most in right tree)
                Node *succ = p->right;
                while (succ->left) {
                    succ = succ->left;
                }
                p->key = succ->key;
                p->val = succ->val;
                p->next = succ->next;
                // remove successor node from right tree
                removeInternal(p->right, root, succ->key);
            }
            removed = true;
        }

        if (removed && p) {
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
