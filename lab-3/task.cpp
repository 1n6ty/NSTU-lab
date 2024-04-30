#include <iostream>
#include <stack>
#include <utility>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>

typedef struct Node {
    std::string data;
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
Node *createTree(std::string data){
    Node *tree = createTree();
    tree->data = data;

    return tree;
}
Node *createTree(std::string data, Node *left, Node *right){
    Node *tree = createTree();
    tree->data = data;
    tree->left = left;
    tree->right = right;

    return tree;
}

void insert(Node *node, char where, std::string data){
    Node *buff = createTree(data);
    buff->parent = node;
    if(where == 'l'){
        node->left = buff;
    } else if(where == 'r'){
        node->right = buff;
    }
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

void convert2formulaTree(Node *tree, std::string &avail_s){
    std::string bef = "", aft = "", sym = "";
    int priority = -1;

    size_t f_size = tree->data.length(), open_bc = 0, close_bc = 0;
    std::string::iterator buff;
    for(size_t i = 0; i < f_size; i++){
        if(tree->data[i] == '('){
            open_bc ++;
            continue;
        } 
        if(tree->data[i] == ')'){
            close_bc ++;
            continue;
        } 
        buff = std::find(avail_s.begin(), avail_s.end(), tree->data[i]);
        if(buff != avail_s.end()){
            if(open_bc == close_bc && (buff - avail_s.begin()) >= priority){
                bef = std::string(tree->data.begin(), tree->data.begin() + i);
                aft = std::string(tree->data.begin() + i + 1, tree->data.end());
                sym = tree->data[i];
                priority = (buff - avail_s.begin());
            }
            continue;
        }
    }
    if(sym == "" && *(tree->data.begin()) == '(' && *(tree->data.rbegin()) == ')'){
        tree->data = std::string(tree->data.begin() + 1, tree->data.end() - 1);
        convert2formulaTree(tree, avail_s);
    } else if(sym != ""){
        insert(tree, 'l', bef);
        insert(tree, 'r', aft);
        tree->data = sym;

        convert2formulaTree(tree->left, avail_s);
        convert2formulaTree(tree->right, avail_s);
    }
}

char check_formulaTree(Node *tree, std::string &avail_f){
    std::stack<Node *> s;
    s.push(tree);

    std::string::iterator buff;
    Node *buff_node;
    while(!s.empty()){
        buff_node = s.top();
        s.pop();

        if(buff_node->left == NULL && buff_node->right == NULL){
            buff = std::find(avail_f.begin(), avail_f.end(), buff_node->data[0]);

            if(buff == avail_f.end()) return 0;
        }

        if(buff_node->right != NULL) s.push(buff_node->right);
        if(buff_node->left != NULL) s.push(buff_node->left);
    }
    return 1;
}

int sum_tree(Node *tree){
    std::stack<Node *> s;
    s.push(tree);

    int answ = 0;

    Node *buff_node;
    while(!s.empty()){
        buff_node = s.top();
        s.pop();

        if(buff_node->left == NULL && buff_node->right == NULL){
            answ += buff_node->data[0] - '0';
        }

        if(buff_node->right != NULL) s.push(buff_node->right);
        if(buff_node->left != NULL) s.push(buff_node->left);
    }
    return answ;
}

int main(){
    std::string s, new_s = "", avail_f, avail_s;
    std::fstream input, symbols, formulas;

    input.open("input.txt", std::ios::in);
    symbols.open("symbols.txt", std::ios::in);
    formulas.open("formulas.txt", std::ios::in);

    if(input.is_open() && symbols.is_open() && formulas.is_open()){
        std::getline(input, s);
        std::getline(symbols, avail_s);
        std::getline(formulas, avail_f);
    } else {
        std::cout << "Files are unavailable\n";
        return 1;
    }

    // Remove witespaces
    size_t s_len = s.length();
    for(int i = 0; i < s_len; i++){
        if(s[i] != ' ') new_s += s[i];
    }
    
    Node *tree = createTree(new_s);
    convert2formulaTree(tree, avail_s);
    printTree(tree);

    std::cout << "Is it a formula tree: " << (bool) check_formulaTree(tree, avail_f) << '\n';
    std::cout << "Sum: " << sum_tree(tree) << '\n';

    input.close();
    symbols.close();
    formulas.close();

    return 0;
}