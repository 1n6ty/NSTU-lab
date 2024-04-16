#include <iostream>
#include <math.h>
#include <stack>
#include <utility>
#include <string>
#include <limits.h>

typedef struct Node {
    int data;
    Node *left;
    Node *right;
    Node *parent;
} Node;

Node *createTree(){
    Node *tree = new Node;
    tree->left = NULL;
    tree->right = NULL;
    tree->parent = NULL;

    return tree;
}
Node *createTree(int data){
    Node *tree = createTree();
    tree->data = data;

    return tree;
}
Node *createTree(int data, Node *left, Node *right){
    Node *tree = createTree();
    tree->data = data;
    tree->left = left;
    tree->right = right;

    return tree;
}

void insert(Node *node, char where, int data){
    Node *buff = createTree(data);
    buff->parent = node;
    if(where == 'l'){
        node->left = buff;
    } else if(where == 'r'){
        node->right = buff;
    }
}

Node *findLeftMost(Node *tree){
    while(tree->left != NULL){
        tree = tree->left;
    }
    return tree;
}

void printTree(Node *tree, Node *current = NULL){
    std::stack<std::pair<Node *, int>> s;
    s.push({tree, 0});

    std::pair<Node *, int> buff;
    while(!s.empty()){
        buff = s.top();
        s.pop();

        std::cout << std::string(std::max(buff.second - 1, 0), '|') << ((buff.second != 0) ? "∟": "") << ((buff.first == current) ? ">": "") << buff.first->data << '\n';
        if(buff.first->left != NULL) s.push({buff.first->left, buff.second + 1});
        if(buff.first->right != NULL) s.push({buff.first->right, buff.second + 1});
    }
}

int main(){

    Node *tree = createTree(0);
    Node *current = tree;

    char inp_buff;
    int num;
    while(1){
        std::cout << "Enter u - go up, l - go left, r - go right, 1 - add to left, 2 - add to right, 3 - print leftmost node, 4 - exit\n";
        std::cin >> inp_buff;
        if(inp_buff == 'u'){
            if(current->parent != NULL){
                current = current->parent;
            } else {
                std::cout << "You are already up)\n";
            }
        } else if(inp_buff == 'l'){
            if(current->left != NULL){
                current = current->left;
            } else {
                std::cout << "There is no left node(\n";
            }
        } else if(inp_buff == 'r'){
            if(current->right != NULL){
                current = current->right;
            } else {
                std::cout << "There is no right node(\n";
            }
        } else if(inp_buff == '1'){
            if(current->left == NULL){
                std::cout << "Enter the data to store: ";
                std::cin >> num;
                insert(current, 'l', num);
            } else {
                std::cout << "Left node already exist)\n";
            }
        } else if(inp_buff == '2'){
            if(current->right == NULL){
                std::cout << "Enter the data to store: ";
                std::cin >> num;
                insert(current, 'r', num);
            } else {
                std::cout << "Right node already exist)\n";
            }
        } else if(inp_buff == '3'){
            std::cout << "LeftMost Node: " << findLeftMost(tree)->data << '\n';
        }else if(inp_buff == '4'){
            break;
        }else {
            std::cout << "Wrong mode!\n";
        }
        printTree(tree, current);
    }
    return 0;
}