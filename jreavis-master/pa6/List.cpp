/*
Jordan Reavis
jreavis
Spring 23 - CSE101 - pa6
List.cpp - List ADT implementation file
*/

#include <iostream>
#include <string>
#include <stdexcept>
#include "List.h"

//Private constructor
List::Node::Node(ListElement x) {
    data = x;
    next = nullptr;
    prev = nullptr;
}

//Class constructors and destructors

//Creates a new List in the empty state
List::List() {
    frontDummy = new Node(0);
    backDummy = new Node(0);
    frontDummy->next = backDummy;
    backDummy->prev = frontDummy;
    beforeCursor = frontDummy;
    afterCursor = backDummy;
    pos_cursor = 0;
    num_elements = 0;
}

//Copy constructor
List::List(const List& L) {
    //Make this an empty List
    frontDummy = new Node(0);
    backDummy = new Node(0);
    frontDummy->next = backDummy;
    backDummy->prev = frontDummy;
    beforeCursor = frontDummy;
    afterCursor = backDummy;
    pos_cursor = 0;
    num_elements = 0;

    //Load elements of L into this List
    Node* N = L.frontDummy->next;

    while(N != L.backDummy) {
        this->insertAfter(N->data);
        this->moveNext();
        N = N->next;
    }
    moveFront();
}

//Destructor
List::~List() {
    moveFront();
    while(num_elements > 0) {
        eraseAfter(); 
    }
    delete(backDummy);
    delete(frontDummy);
}

//Access functions
int List::length() const {
    return (num_elements);
}

ListElement List::front() const {
    if(length() < 0) {
        throw std::length_error("List error: calling front() on invalid List reference\n");
    }
    return (frontDummy->next->data);
}

ListElement List::back() const {
    if(length() < 0) {
        throw std::length_error("List error: calling back() on invalid List reference\n");
    }
    return (backDummy->prev->data);
}

int List::position() const {
    return(pos_cursor);
}

ListElement List::peekNext() const {
    if(position() > length()) {
        throw std::length_error("List error: calling peekNext() on invalid List reference\n");
    }
    return(afterCursor->data);
}

ListElement List::peekPrev() const {
    if(position() < 0) {
        throw std::length_error("List error: calling peekPrev() on invalid List reference\n");
    }
    return (beforeCursor->data);
}

//Manipulation procedures
void List::clear() {
    if(length() < 0) {
        throw std::length_error("List error: calling clear() on empty List\n");
    }

    moveFront();
    while(num_elements > 0) {
        eraseAfter(); 
    }
}

void List::moveFront() {
    if(length() < 0) {
        throw std::length_error("List error: calling moveFront() on empty List reference\n");
    }

    pos_cursor = 0;
    beforeCursor = frontDummy;
    afterCursor = frontDummy->next;
}

void List::moveBack() {
    if(length() < 0) {
        throw std::length_error("List error: calling moveBack() on empty List reference\n");
    }

        pos_cursor = length();
        afterCursor = backDummy;
        beforeCursor = backDummy->prev;
}

ListElement List::moveNext() {
    if(position() > length()) {
        throw std::length_error("List error: calling moveNext() on invalid List reference\n");
    }
    beforeCursor = beforeCursor->next;
    afterCursor = afterCursor->next;
    // afterCursor->prev = beforeCursor;
    // beforeCursor->next = afterCursor;

    pos_cursor++;

    return(beforeCursor->data);
}

ListElement List::movePrev() {
    if(position() < 0) {
        throw std::length_error("List error: calling movePrev() on invalid List reference\n");
    }
    if(afterCursor == frontDummy) {
        throw std::length_error("List error: calling movePrev() at front of list reference\n");
    }

    afterCursor = afterCursor->prev;
    beforeCursor = beforeCursor->prev;
    // afterCursor->prev = beforeCursor;
    // beforeCursor->next = afterCursor;

    pos_cursor--;

    return (afterCursor->data);
}

void List::insertAfter(ListElement x) {
    Node *N = new Node(x);

    N->prev = beforeCursor;
    beforeCursor->next = N;
    N->next = afterCursor;
    afterCursor->prev = N;
    afterCursor = N;
    num_elements++;
}

void List::insertBefore(ListElement x) {
    Node *N = new Node(x);

    N->next = afterCursor;
    afterCursor->prev = N;
    N->prev = beforeCursor;
    beforeCursor->next = N;
    beforeCursor = N;
    num_elements++;
    pos_cursor++;
}

void List::setAfter(ListElement x) {
    if(position() >= num_elements) {
        throw std::length_error("List error: calling setAfter() on invalid List reference\n");
    }

    afterCursor->data = x;
}

void List::setBefore(ListElement x) {
    if(pos_cursor <= 0) {
        throw std::length_error("List error: calling setBefore() on invalid List reference\n");
    }

    beforeCursor->data = x;
}

void List::eraseAfter() {
    if(position() > length()) {
        throw std::length_error("List error: calling eraseAfter() on invalid List reference\n");
    }

    afterCursor = afterCursor->next;
    delete(afterCursor->prev);
    afterCursor->prev = beforeCursor;
    beforeCursor->next = afterCursor;

    num_elements--;

}

void List::eraseBefore() {
    if(position() < 0) {
        throw std::length_error("List error: calling eraseBefore() on invalid List reference\n");
    }

    beforeCursor = beforeCursor->prev;
    delete(beforeCursor->next);
    beforeCursor->next = afterCursor;
    afterCursor->prev = beforeCursor;
    
    num_elements--;
    pos_cursor--;
}

//Other operations
int List::findNext(ListElement x) {
    while(position() < length()) {
        if(afterCursor->data == x) {
            moveNext();
            return pos_cursor;
        } else {
            moveNext();
        }
    }
    pos_cursor = length();
    return -1;
}

int List::findPrev(ListElement x) {
    while(position() > 0) {
        if(beforeCursor->data == x) {
            movePrev();
            return pos_cursor;
        } else {
            movePrev();
        }
    }
    pos_cursor = 0;
    return -1;
}

void List::cleanup() {
    int i;
    int j;
    Node* temp = frontDummy->next;
    for(i = 1; i <= length(); i++) {
        Node *search = temp->next;
        for(j = i + 1; j <= length()+1; j++) {
            if(temp->data != search->data) {
                search = search->next;
            } else if (temp->data == search->data) {
                Node* deleteNode = search;
                search->prev->next = search->next;
                search->next->prev = search->prev;
                if(search == beforeCursor) {
                    beforeCursor = search->prev;
                    pos_cursor--;
                } else if (search == afterCursor) {
                    afterCursor = search->next;
                } else if(j < pos_cursor){
                    pos_cursor--;
                }
                search = search->next;
                delete(deleteNode);
                num_elements--;
            }
            }
            temp = temp->next;
        }
    }



List List::concat(const List& L) const {
    List concat;

    Node* N = this->frontDummy->next;
    Node* M = L.frontDummy->next;

    while(N != this->backDummy) {
        concat.insertBefore(N->data);
        N = N->next;
    }

    while(M != L.backDummy) {
        concat.insertBefore(M -> data);
        M = M->next;
    }

    concat.moveFront();
    return concat;
}

std::string List::to_string() const {
    Node* N = nullptr;
    std::string s = "(";
    if(num_elements > 0){
    for(N = (frontDummy->next); N != (backDummy->prev); N = N->next) {
        s += std::to_string(N->data)+", ";
    }
    s += std::to_string(N->data)+")";
}
    return s; 
}

bool List::equals(const List& R) const {
    bool eq = false;
    Node* N = nullptr;
    Node* M = nullptr;

    eq = (this->num_elements == R.num_elements);
    N = this->frontDummy->next;
    M = R.frontDummy->next;
    while(eq && N != backDummy) {
        eq = (N->data == M->data);
        N = N->next;
        M = M->next;
    }
    return eq;
}

//Overridden operators

//operator <<()
std::ostream& operator<<(std::ostream& stream, const List& L) {
    return stream << L.List::to_string();
}

//operator ==()
bool operator==(const List& A, const List& B) {
    return A.List::equals(B);
}

//operator =()
List& List::operator=(const List& L) {
    if(this != &L) {
        List temp = L;

        std::swap(frontDummy, temp.frontDummy);
        std::swap(backDummy, temp.backDummy);
        std::swap(beforeCursor, temp.beforeCursor);
        std::swap(afterCursor, temp.afterCursor);
        std::swap(pos_cursor, temp.pos_cursor);
        std::swap(num_elements, temp.num_elements);
    }
    return *this;
}