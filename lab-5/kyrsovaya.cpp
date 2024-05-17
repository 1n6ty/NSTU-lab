#include <iostream>
#include <algorithm>
#include <stack>

struct Node {
    int key;
    int height;
    Node* left;
    Node* right;
    
    Node(int k) : key(k), height(1), left(nullptr), right(nullptr) {}
};

int height(Node* node) {
    if (node == nullptr) {
        return 0;
    }
    return node->height;
}

int balanceFactor(Node* node) {
    if (node == nullptr) {
        return 0;
    }
    return height(node->left) - height(node->right);
}

void updateHeight(Node* node) {
    node->height = 1 + std::max(height(node->left), height(node->right));
}

Node* rotateRight(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;
    
    x->right = y;
    y->left = T2;
    
    updateHeight(y);
    updateHeight(x);
    
    return x;
}

Node* rotateLeft(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;
    
    y->left = x;
    x->right = T2;
    
    updateHeight(x);
    updateHeight(y);
    
    return y;
}

Node* insert(Node* node, int key) {
    if (node == nullptr) {
        return new Node(key);
    }
    
    if (key < node->key) {
        node->left = insert(node->left, key);
    } else {
        node->right = insert(node->right, key);
    }
    
    updateHeight(node);
    
    int bf = balanceFactor(node);
    
    if (bf > 1 && key < node->left->key) {
        return rotateRight(node);
    }
    if (bf < -1 && key > node->right->key) {
        return rotateLeft(node);
    }
    if (bf > 1 && key > node->left->key) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    if (bf < -1 && key < node->right->key) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    
    return node;
}

void printTree(Node *tree, Node *current = NULL){
    std::stack<std::pair<Node *, int>> s;
    s.push({tree, 0});

    std::pair<Node *, int> buff;
    while(!s.empty()){
        buff = s.top();
        s.pop();

        std::cout << std::string(std::max(buff.second - 1, 0), '|') << ((buff.second != 0) ? "<": "") << ((buff.first == current) ? ">": "") << buff.first->key << '\n';
        if(buff.first->left != NULL) s.push({buff.first->left, buff.second + 1});
        if(buff.first->right != NULL) s.push({buff.first->right, buff.second + 1});
    }
}

void buildAVLTree(Node*& root) {
    int n, key;
    
    std::cout << "Enter the number of elements to insert: ";
    std::cin >> n;
    
    std::cout << "Enter the elements to insert: ";
    for (int i = 0; i < n; ++i) {
        std::cin >> key;
        root = insert(root, key);
    }
}

int main() {
    Node* root = nullptr;
    char choice;
    
    do {
        int key;
        std::cout << "Enter the element to insert: ";
        std::cin >> key;
        root = insert(root, key);

        std::cout << "Inorder traversal of the AVL tree:\n";
        printTree(root);
        
        std::cout << "\nDo you want to insert another element? (y/n): ";
        std::cin >> choice;
    } while (choice == 'y' || choice == 'Y');
    

    
    return 0;
}
