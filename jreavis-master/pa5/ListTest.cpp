/*
Jordan Reavis
jreavis
Spring 23 - CSE101 - pa5
ListTest.cpp - List ADT testing file
*/

#include <iostream>
#include <string>
#include <stdexcept>
#include "List.h"

using namespace std;


int main() {
    List A, B, C;

    for(int i = 1; i <= 10; i++) {
        A.insertBefore(i);
    }

    for(int i = 10; i >= 1; i--) {
        B.insertAfter(i);
    }

    cout << "List A: " << A << endl;
    cout << "List B: " << B << endl;

    cout << "Lengths: " << A.length() << endl;

    cout << "Front of A: " << A.front() << endl;
    cout << "Back of B: " << B.back() << endl;

    A.moveFront();
    cout << "A position: " << A.position() << endl;
    cout << "A's next value: " << A.peekNext() << endl;
    cout << "Advancing A's cursor: " << A.moveNext() << endl;

    B.moveBack();
    cout << "B's position: " << B.position() << endl;
    cout << "B's previous value: " << B.peekPrev() << endl;
    cout << "Decrementing B's cursor: " << B.movePrev() << endl;

    A.moveFront();
    A.setAfter(5);
    cout << "New A front: " << A.front() << endl;

    B.moveBack();
    B.setBefore(20);
    cout << "New B back: " << B.back() << endl;

    cout << "Deleting A front " << endl;
    A.eraseAfter();
    cout << A << endl;

    cout << "Deleting B back " << endl;
    B.eraseBefore();
    cout << B << endl;

    cout << A.findNext(7) << endl;
    cout << B.findPrev(2) << endl;

    C = A.concat(B);
    cout << "List C: " << C << endl;

    cout << "Removing repeat elements in C..." << endl;
    C.cleanup();
    cout << C << endl;

    A.clear();
    B.clear();
    if(A.equals(B)) {
        cout << "True" << endl;
    }

    return 0;
}