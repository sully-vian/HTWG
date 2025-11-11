#include <algorithm>
#include <functional>
#include <iostream>
#include <sstream>

template <typename K, typename V> class AVLTree {
  public:
    AVLTree() : root(nullptr) {}

    bool insert(K key, V value) {
        bool result;
        if (root == nullptr) {
            root = new Node(key, value);
            result = true;
        } else if (key < root->key) {
            result = root->left.insert(key, value);
        } else if (key > root->key) {
            result = root->right.insert(key, value);
        } else {            // key == root.key
            result = false; // key already exists
        }
        if (result) {
            this->balance();
        }
        return result;
    }

    /*
     * Returns the string representing the tree according to the DOT syntax
     */
    std::string toDot() {
        if (!root) {
            // empty tree
            return "digraph G {\n}";
        }

        std::ostringstream oss;
        oss << "digraph G {\n";

        std::function<void(Node *)> traverse = [&](Node *node) {
            if (!node) {
                return;
            }

            // Left child
            if (node->left.root) {
                oss << "\t\"" << node->key << "\" -> \"" << node->left.root->key
                    << "\";\n";
                traverse(node->left.root);
            }

            // Right child
            if (node->right.root) {
                oss << "\t\"" << node->key << "\" -> \""
                    << node->right.root->key << "\";\n";
                traverse(node->right.root);
            }
        };

        traverse(root);
        oss << "}\n";
        return oss.str();
    };

    void print(const char msg[]) { std::cout << msg << std::endl; }

  private:
    struct Node {
        int height;
        K key;
        V value;
        AVLTree left;
        AVLTree right;
        Node *succ; // in-order successor

        /*
         * Node inserted as a leaf so initial height is 0
         */
        Node(K key, V value)
            : height(0), key(key), value(value), left(), right() {};
    };

    Node *root;

    void balance() {
        if (root == nullptr) {
            // empty tree, nothing to do
            return;
        }

        // update height
        root->height =
            std::max(root->left.getHeight(), root->right.getHeight()) + 1;

        if (getBalance() == -2) {        // tree is left heavy
            if (root->left.getBalance() <= 0) {
                rotateRight();           // A1
            } else {
                rotateLeftRight();       // A2
            }
        } else if (getBalance() == +2) { // tree is right heavy
            if (root->right.getBalance() >= 0) {
                rotateLeft();            // B1
            } else {
                rotateRightLeft();       // B2
            }
        }
    }

    /* ------------------- */
    /* Auxiliary functions */
    /* ------------------- */

    int getHeight() { return root ? root->height : -1; }

    int getBalance() {
        return root ? root->right.getHeight() - root->left.getHeight() : 0;
    }

    void rotateRight() {
        Node *oldRoot = root;
        Node *newRoot = oldRoot->left.root;
        // re-link: oldRoot left as newRoot right
        oldRoot->left.root = newRoot->right.root;
        newRoot->right.root = oldRoot;
        oldRoot->height =
            std::max(oldRoot->left.getHeight(), oldRoot->right.getHeight()) + 1;
        newRoot->height =
            std::max(newRoot->left.getHeight(), newRoot->right.getHeight()) + 1;

        root = newRoot; // set new root
    }

    void rotateLeft() {
        Node *oldRoot = root;
        Node *newRoot = oldRoot->right.root;

        oldRoot->right.root = newRoot->left.root;
        newRoot->left.root = oldRoot;
        oldRoot->height =
            std::max(oldRoot->left.getHeight(), oldRoot->right.getHeight()) + 1;
        newRoot->height =
            std::max(newRoot->left.getHeight(), newRoot->right.getHeight()) + 1;

        root = newRoot; // set new root
    }

    void rotateLeftRight() {
        root->left.rotateLeft();
        rotateRight();
    }

    void rotateRightLeft() {
        root->right.rotateRight();
        rotateLeft();
    }
};
