#include <iostream>
#include <queue>
using namespace std;

struct Node {
    int key;
    Node* left;
    Node* right;
    int height;
};

Node* newNode(int key) {
    Node* node = new Node();
    node->key = key;
    node->left = nullptr;
    node->right = nullptr;
    node->height = 1;
    return node;
}

int height(Node* node) {
    if (node == nullptr) {
        return 0;
    }
    return node->height;
}

int getBalance(Node* node) {
    if (node == nullptr) {
        return 0;
    }
    return height(node->left) - height(node->right);
}

Node* insert(Node* node, int key) {
    if (node == nullptr) {
        return newNode(key);
    }

    if (key < node->key) {
        node->left = insert(node->left, key);
    } else if (key > node->key) {
        node->right = insert(node->right, key);
    } else {
        return node;
    }

    node->height = 1 + max(height(node->left), height(node->right));

    int balance = getBalance(node);

    // Left Left Case
    if (balance > 1 && key < node->left->key) {
        // Right rotation
        // TODO
    }
    // Right Right Case
    if (balance < -1 && key > node->right->key) {
        // Left rotation
        // TODO
    }
    // Left Right Case
    if (balance > 1 && key > node->left->key) {
        // Left rotation
        // Right rotation
        // TODO
    }
    // Right Left Case
    if (balance < -1 && key < node->right->key) {
        // Right rotation
        // Left rotation
        // TODO
    }

    return node;
}

void printLevelOrder(Node* root) {
    if (root == nullptr) {
        return;
    }

    queue<Node*> q;
    q.push(root);

    while (!q.empty()) {
        int size = q.size();
        for (int i = 0; i < size; i++) {
            Node* current = q.front();
            q.pop();

            cout << current->key;

            if (current->left != nullptr) {
                q.push(current->left);
                cout << " \\ ";
            }
            if (current->right != nullptr) {
                q.push(current->right);
                cout << " / ";
            }

            if (i < size - 1) {
                cout << " | ";
            }
        }
        cout << endl;
    }
}

int main() {
    Node* root = nullptr;
    root = insert(root, 10);
    insert(root, 5);
    insert(root, 15);
    insert(root, 3);
    insert(root, 7);

    cout << "AVL Tree (Level Order with Branches):" << endl;
    printLevelOrder(root);

    return 0;
}
