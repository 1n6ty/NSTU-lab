#include <iostream>
#include <string>
#include <algorithm>

struct AVLNode {
    std::string key;
    double value;
    int height;
    AVLNode* left;
    AVLNode* right;

    AVLNode(std::string k, double v) : key(k), value(v), height(1), left(nullptr), right(nullptr) {}
};

int getHeight(AVLNode* node) {
    if (node == nullptr) {
        return 0;
    }
    return node->height;
}

int getBalanceFactor(AVLNode* node) {
    if (node == nullptr) {
        return 0;
    }
    return getHeight(node->left) - getHeight(node->right);
}

AVLNode* rotateRight(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = 1 + std::max(getHeight(y->left), getHeight(y->right));
    x->height = 1 + std::max(getHeight(x->left), getHeight(x->right));

    return x;
}

AVLNode* rotateLeft(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = 1 + std::max(getHeight(x->left), getHeight(x->right));
    y->height = 1 + std::max(getHeight(y->left), getHeight(y->right));

    return y;
}

AVLNode* insertNode(AVLNode* root, std::string key, double value) {
    if (root == nullptr) {
        std::cout << "The element was successfully inserted into the table.\n";
        return new AVLNode(key, value);
    }

    if (key < root->key) {
        root->left = insertNode(root->left, key, value);
    } else if (key > root->key) {
        root->right = insertNode(root->right, key, value);
    } else {
        std::cout << "Key " << key << " already exists in the table. Failed to insert element." << std::endl;
        return root;
    }

    root->height = 1 + std::max(getHeight(root->left), getHeight(root->right));

    int balance = getBalanceFactor(root);

    if (balance > 1 && key < root->left->key) {
        return rotateRight(root);
    }

    if (balance < -1 && key > root->right->key) {
        return rotateLeft(root);
    }

    if (balance > 1 && key > root->left->key) {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }

    if (balance < -1 && key < root->right->key) {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }

    return root;
}

void insertElement(AVLNode*& root) {
    std::string key;
    double value;

    std::cout << "Enter element key (no more than 15 characters): ";
    std::cin >> key;

    if (key.size() > 15){
        std::cout << "Enter invalid key.\n";
        return;
    }

    std::cout << "Enter element value: ";
    std::cin >> value;

    root = insertNode(root, key, value);
}

int main() {
    AVLNode* root = nullptr;

    int continueInput = 1;
    while (continueInput) {
        insertElement(root);

        std::cout << "Want to add another element? (1/0): ";
        std::cin >> continueInput;
    }

    return 0;
}

