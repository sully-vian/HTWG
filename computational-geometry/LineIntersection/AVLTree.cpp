#include <algorithm>
#include <functional>
#include <iostream>
#include <sstream>

template <typename K, typename V> class AVLNode {
  public:
    AVLNode(K key, V value)
        : height(0), key(key), value(value), left(), right() {};

    // returns thenew root of this subtree
    AVLNode *insert(K key, V value) {
        if (key < this->key) {
            if (left) {
                left = left->insert(key, value);
            } else {
                left = new AVLNode(key, value);
            }
        } else if (key > this->key) {
            if (right) {
                right = right->insert(key, value);
            } else {
                right = new AVLNode(key, value);
            }
        } else { // duplicate key
            return this;
        }
        updateHeight();
        return balance();
    }

    /*
     * Returns the string representing the tree according to the DOT syntax
     */
    std::string toDot() {
        std::ostringstream oss;
        oss << "digraph G {\n";

        std::function<void(AVLNode *)> traverse = [&](AVLNode *node) {
            if (!node) {
                return;
            }

            // Left child
            if (node->left) {
                oss << "\t\"" << node->key << "\" -> \"" << node->left->key
                    << "\";\n";
                traverse(node->left);
            }

            // Right child
            if (node->right) {
                oss << "\t\"" << node->key << "\" -> \"" << node->right->key
                    << "\";\n";
                traverse(node->right);
            }
        };

        traverse(this);
        oss << "}\n";
        return oss.str();
    };

    void print(const char msg[]) { std::cout << msg << std::endl; }

  private:
    K key;
    V value;
    int height;
    AVLNode *left;
    AVLNode *right;

    /* ------------------- */
    /* Auxiliary functions */
    /* ------------------- */

    int getHeight(AVLNode *node) { return node ? node->height : -1; }
    int getBalance() { return getHeight(right) - getHeight(left); }

    void updateHeight() {
        height = 1 + std::max(getHeight(left), getHeight(right));
    }

    AVLNode *rotateLeft() {
        AVLNode *newRoot = right;
        right = newRoot->left;
        newRoot->left = this;
        updateHeight();
        newRoot->updateHeight();
        return newRoot;
    }
    AVLNode *rotateRight() {
        AVLNode *newRoot = left;
        left = newRoot->right;
        newRoot->right = this;
        updateHeight();
        newRoot->updateHeight();
        return newRoot;
    }

    AVLNode *balance() {
        int b = getBalance();
        if (b == -2) { // left heavy
            if (left->getBalance() <= 0) {
                return rotateRight();
            } else {
                left = left->rotateLeft();
                return rotateRight();
            }
        } else if (b == 2) { // right heavy
            if (right->getBalance() >= 0) {
                return rotateLeft();
            } else {
                right = right->rotateRight();
                return rotateLeft();
            }
        }
        return this; // nothing to do
    }
};
