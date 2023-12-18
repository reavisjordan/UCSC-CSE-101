/*
Jordan Reavis
jreavis
Spring 23 - CSE101 - pa8
Dictionary.cpp - Dictionary ADT implementation file
*/

#include <iostream>
#include <cstring>
#include <string>
#include <limits>
#include <sstream>
#include <stdexcept>
#include "Dictionary.h"

#define RED 0
#define BLACK 1

//Private constructor------------------------------------------------------

//Dictionary node constructor
Dictionary::Node::Node(keyType k, valType v) {
    key = k;
    val = v;
    parent = nullptr;
    left = nullptr;
    right = nullptr;
    color = BLACK;
}

//Dictionary helper functions------------------------------------------------------------------------
void Dictionary::inOrderString(std::string &s, Dictionary::Node* R) const {
    if(R != nil) {
        inOrderString(s, R->left);
        s.append(R->key);
        s.append(" : ");
        s.append(std::to_string(R->val));
        s.append("\n");
        inOrderString(s, R->right);
    }
}

void Dictionary::preOrderString(std::string& s, Dictionary::Node* R) const {
    if(R != nil) {
        s.append(R->key);
        if(R->color == RED) {
             s.append(" ");
        s.append("(RED)");
        }
        s.append("\n");
        preOrderString(s, R->left);
        preOrderString(s, R->right);
    } 
}

void Dictionary::BST_insert(Node *M) {
    Node* y = nil;
    Node* x = root;
    while(x != nil) {
        y = x;
        if(M->key < x->key) {
            x = x->left;
        } else {
            x = x->right;
        }
    }

    M->parent = y;

    if(y == nil) {
        root = M;
    } else if(M->key < y->key) {
        y->left = M;
    } else {
        y->right = M;
    }

    M->left = nil;
    M->right = nil;
    M->color = RED;
    num_pairs++;
    RB_InsertFixUp(M);
}

void Dictionary::preOrderCopy(Node* R, Node* N) {
    if(R != N) {
        setValue(R->key, R->val);
        preOrderCopy(R->left, N);
        preOrderCopy(R->right, N);
    }
}

void Dictionary::postOrderDelete(Node* R) {
    if(R != nil) {
        postOrderDelete(R->left);
        postOrderDelete(R->right);
        delete R;
    }
}

Dictionary::Node* Dictionary::search(Node* R, keyType k) const {
    if(R == nil || k == R->key) {
        return R;
    } else if(k < R->key) {
        return search(R->left, k);
    } else {
        return search(R->right, k);
    }
}

Dictionary::Node* Dictionary::findMin(Node* R) {
    if(R == nil) {
        return nil;
    }

    Node* min = R;

    while(min->left != nil) {
        min = min->left;
    }

    return min;
}

Dictionary::Node* Dictionary::findMax(Node* R) {
    if(R == nil) {
        return nil;
    }

    Node* max = R;

    while(max->right != nil) {
        max = max->right;
    }

    return max;
}


Dictionary::Node* Dictionary::findNext(Node* N) {
    if(N == nil) {
        return nil;
    }  
    if(N->right != nil) {
        return findMin(N->right);
    } 
     Node* y = N->parent;
    while(y != nil && N == y->right) {
         N = y;
         y = y->parent;
    }
        return y;
}

Dictionary::Node* Dictionary::findPrev(Node* N) {
    if(N == nil) {
        return nil;
    } 
    if(N->left != nil) {
        return findMax(N->left);
    } 

     Node* y = N->parent;
        while(y != nil && N == y->left) {
            N = y;
            y = y->parent;
        }
        return y;
}

//RBT Helper functions
void Dictionary::LeftRotate(Node* N) {
    Node* y = N->right;

    N->right = y->left;
    if(y->left != nil) {
        y->left->parent = N;
    }

    y->parent = N->parent;
    if(N->parent == nil) {
        root = y;
    } else if(N == N->parent->left) {
        N->parent->left = y;
    } else {
        N->parent->right = y;
    }

    y->left = N;
    N->parent = y;
}

void Dictionary::RightRotate(Node* N) {
    Node* y = N->left;

    N->left = y->right;
    if(y->right != nil) {
        y->right->parent = N;
    }

    y->parent = N->parent;
    if(N->parent == nil) {
        root = y;
    } else if(N == N->parent->right) {
        N->parent->right = y;
    } else {
        N->parent->left = y;
    }

    y->right = N;
    N->parent = y;
}

void Dictionary::RB_InsertFixUp(Node* N) {
    while(N->parent->color == RED) {
        if(N->parent == N->parent->parent->left) {
            Node* y = N->parent->parent->right;
            if(y->color == RED) {
                N->parent->color = BLACK;
                y->color = BLACK;
                N->parent->parent->color = RED;
                N = N->parent->parent;
            } else {
                if(N == N->parent->right) {
                    N = N->parent;
                    LeftRotate(N);
                }
                N->parent->color = BLACK;
                N->parent->parent->color = RED;
                RightRotate(N->parent->parent);
            }
        } else {
            Node* y = N->parent->parent->left;
            if(y->color == RED) {
                N->parent->color = BLACK;
                y->color = BLACK;
                N->parent->parent->color = RED;
                N = N->parent->parent;
            } else {
                if(N == N->parent->left) {
                    N = N->parent;
                    RightRotate(N);
                }
                N->parent->color = BLACK;
                N->parent->parent->color = RED;
                LeftRotate(N->parent->parent);
            }
        }
    }
    root->color = BLACK;
}

void Dictionary::RB_Transplant(Node* u, Node* v) {
    if(u->parent == nil) {
        root = v;
    } else if(u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    v->parent = u->parent;
}


void Dictionary::RB_DeleteFixUp(Node* N) {
    while(N != root && N->color == BLACK) {
        if(N == N->parent->left) {
            Node* w = N->parent->right;
            if(w->color == RED) {
                w->color = BLACK;
                N->parent->color = RED;
                LeftRotate(N->parent);
                w = N->parent->right;
            }
            if(w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                N = N->parent;
            } else {
                if(w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    RightRotate(w);
                    w = N->parent->right;
                }
                w->color = N->parent->color;
                N->parent->color = BLACK;
                w->right->color = BLACK;
                LeftRotate(N->parent);
                N = root;
            }
        } else {
           Node* w = N->parent->left;
           if(w->color == RED) {
                w->color = BLACK;
                N->parent->color = RED;
                RightRotate(N->parent);
                w = N->parent->left;
           }
           if(w->right->color == BLACK && w->left->color == BLACK) {
                w->color = RED;
                N = N->parent;
           } else {
            if(w->left->color == BLACK) {
                w->right->color = BLACK;
                w->color = RED;
                LeftRotate(w);
                w = N->parent->left;
            }
            w->color = N->parent->color;
            N->parent->color = BLACK;
            w->left->color = BLACK;
            RightRotate(N->parent);
            N = root;
           }
        }
    }
    N->color = BLACK;
}

void Dictionary::RB_Delete(Node* N) { //N == z
    Node* y = N;
    Node* x;
    int y_original_color = y->color;
    if(N->left == nil) {
        x = N->right;
        RB_Transplant(N, N->right);
    } else if(N->right == nil) {
        x = N->left;
        RB_Transplant(N, N->left);
    }else {
        y = findMin(N->right);
        y_original_color = y->color;
        x = y->right;
        if(y->parent == N) {
            x->parent = y;
        } else {
            RB_Transplant(y, y->right);
            y->right = N->right;
            y->right->parent = y;
        }
        RB_Transplant(N, y);
        y->left = N->left;
        y->left->parent = y;
        y->color = N->color;
    }

    if(y_original_color == BLACK) {
        RB_DeleteFixUp(x);
    }
}

//Class constructors & destructors-----------------------------------------------------------

//Creates a new Dictionary in the empty state
Dictionary::Dictionary() {
    nil = new Node("random", 999);
    root = nil;
    current = nil;
    num_pairs = 0;
}

//Copy constructor
Dictionary::Dictionary(const Dictionary& D) {
    nil = new Node("random", 999);
    root = nil;
    current = nil;
    num_pairs = 0;
    this->preOrderCopy(D.root, D.nil);
}

//Destructor
Dictionary::~Dictionary() {
    postOrderDelete(root);
    delete nil;
}

//Access functions-----------------------------------------------------------------------------------------
int Dictionary::size() const {
    return num_pairs;
}

bool Dictionary::contains(keyType k) const {
    Node *N = search(root, k);
    if(N != nil) {
        return true;
    } else {
        return false;
    }
}

valType& Dictionary::getValue(keyType k) const {
    if(!contains(k)) {
        throw std::logic_error("Dictionary: getValue(): key \""+k+"\" does not exist");
    }
    
    Node* N = search(root, k);
    
    if(N == nil) {
        throw std::logic_error("Dictionary: getValue(): key \""+k+"\" does not exist");
    }
    
    return N->val;
}

bool Dictionary::hasCurrent() const {
    if(current != nil) {
        return true;
    } else {
        return false;
    }
}

keyType Dictionary::currentKey() const {
    if(!hasCurrent()) {
        throw std::logic_error("Dictionary: currentKey(): current undefined");
    }
    return current->key;
}

valType& Dictionary::currentVal() const {
    if(!hasCurrent()) {
        throw std::logic_error("Dictionary: currentVal(): current undefined");
    }
    return current->val;
}

//Manipulation procedures-----------------------------------------------------------------------------------------------------
void Dictionary::clear() {
    postOrderDelete(root);
    root = nil;
    current = nil;
    num_pairs = 0;
}



void Dictionary::setValue(keyType k, valType v) { 
    Node* P = nil;
    Node* R = root;

    while(R != nil) {
        P = R;
        if(k < R->key) {
            R = R->left;
        } else if(k > R->key) {
            R = R->right;
        } else {
            R->val = v;
            return;
        }
    }

    Node* N = new Node(k, v);
    if(P == nil) {
        N->parent = nil;
        N->left = nil;
        N->right = nil;
        root = N;
    } else if(k < P->key) {
        P->left = N;
        N->parent = P;
        N->left = nil;
        N->right = nil;
    } else {
        P->right = N;
        N->parent = P;
        N->left = nil;
        N->right = nil;
    }

    N->color = RED;
    RB_InsertFixUp(N);

    num_pairs++;
}  



void Dictionary::remove(keyType k) { 
    if(!contains(k)) {
        throw std::logic_error("Dictionary: remove(): key \""+k+"\" does not exist");
    }

    Node* N = search(root, k);

    if(current == N) {
        current = nil;
    }

    RB_Delete(N);
    num_pairs--;
    delete N; 
}

void Dictionary::begin() {
    if(num_pairs != 0) {
        current = findMin(root);
    }
}

void Dictionary::end() {
    if(num_pairs != 0) {
        current = findMax(root);
    }
}

void Dictionary::next() {
    if(!hasCurrent()) {
        throw std::logic_error("Dictionary: next(): current not defined");
    }
    current = findNext(current);
}

void Dictionary::prev() {
    if(!hasCurrent()) {
        throw std::logic_error("Dictionary: prev(): current not defined");
    }
    current = findPrev(current);
}

//Other functions---------------------------------------------------------------------------------------
std::string Dictionary::to_string() const {
    std::string s = "";
    inOrderString(s, root);
    return s;
}

std::string Dictionary::pre_string() const {
    std::string s = "";
    preOrderString(s, root);
    return s;
}

bool Dictionary::equals(const Dictionary& D) const {
    std::string a = to_string();
    std::string b = D.to_string();

    if(num_pairs == D.num_pairs && a == b) {
        return true;
    } else {
        return false;
    }
}

//Overloaded oeprators-----------------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& stream, Dictionary& D) {
    return stream << D.Dictionary::to_string();
}

bool operator==(const Dictionary& A, const Dictionary& B) {
    return A.Dictionary::equals(B);
}

Dictionary& Dictionary::operator=(const Dictionary& D) {
    if(this != &D) {
        Dictionary temp = D;

        std::swap(nil, temp.nil);
        std::swap(root, temp.root);
        std::swap(current, temp.current);
        std::swap(num_pairs, temp.num_pairs);
    }
    return *this;
}