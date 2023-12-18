/*
Jordan Reavis
jreavis
Spring 23 - CSE101 - pa7
Dictionary.cpp - Dictionary ADT implementation file
*/

#include <iostream>
#include <cstring>
#include <string>
#include <limits>
#include <sstream>
#include <stdexcept>
#include "Dictionary.h"

//Private constructor------------------------------------------------------

//Dictionary node constructor
Dictionary::Node::Node(keyType k, valType v) {
    key = k;
    val = v;
    parent = nullptr;
    left = nullptr;
    right = nullptr;
}

//Dictionary helper functions------------------------------------------------------------------------
//"key : value\n"
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

//keys only: "key\n"
void Dictionary::preOrderString(std::string& s, Dictionary::Node* R) const {
    if(R != nil) {
        s.append(R->key);
        s.append("\n");
        preOrderString(s, R->left);
        preOrderString(s, R->right);
    }
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
//End of Dictionary helper functions

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

    num_pairs++;
}  

void Dictionary::transplant(Node *u, Node* v) {
    if(u->parent == nil) {
        root = v;
    } else if(u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    if(v != nil) {
        v->parent = u->parent;
    }
}

void Dictionary::remove(keyType k) {
    if(!contains(k)) {
        throw std::logic_error("Dictionary: remove(): key \""+k+"\" does not exist");
    }

    Node* N = search(root, k);

    if(N->left == nil) {
        transplant(N, N->right);
        num_pairs--;
    } else if(N->right == nil) {
        transplant(N, N->left);
        num_pairs--;
    } else {
        Node* y = findMin(N->right);
        if(y->parent != N) {
            transplant(y, y->right);
            y->right = N->right;
            y->right->parent = y;
        } 
            transplant(N, y);
            y->left = N->left;
            y->left->parent = y;
            num_pairs--;
    }
    if(current == N) {
        current = nil;
    }

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
    //call to_string, check if equal
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