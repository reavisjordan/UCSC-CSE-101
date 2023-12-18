/*
Jordan Reavis
jreavis
Spring 23 - CSE101 - pa5
Shuffle.cpp - List ADT client file
*/

#include <iostream>
#include <string>
#include <stdexcept>
#include <iomanip>
#include "List.h"

using namespace std;

void shuffle(List &D); 
//Make a deep copy of Deck: Deck = deckCopy
//bool eq = (Deck.equals(deckCopy));
//While !eq: {
//shuffle(Deck)
//shuffleCount++
//}

int main(int argc, char* argv[]) {
    List Deck, deckCopy;
    int size, i;
    int shuffleCount = 0;
    bool eq;

    if(argc != 2) {
        cerr << "Usage: " << argv[0] << "<Shuffle> <Deck size>" << endl;
        return(EXIT_FAILURE);
    }

    size = stoi(argv[1]);
    if(size < 0) {
        cout<<"Shuffle error: passing invalid deck size\n";
        return 0;
    }

    cout << "deck size" << setw(16) << "shuffle count" << endl;
    cout << "------------------------------" << endl;

    for(i = 1; i <= size; i++) {
        shuffleCount = 0;
        Deck.insertBefore(i);
        deckCopy = Deck;
        shuffle(Deck);
        shuffleCount++;
        eq = (deckCopy.equals(Deck));
        while(!eq) {
            shuffle(Deck);
            eq = (deckCopy.equals(Deck));
            shuffleCount++;
        }
        cout << " " << left << setw(16) << i << shuffleCount << endl;
    }  
    return 0;
}

void shuffle(List &D) {
    List A, B;

    //Split into two lists A and B: 
    D.moveFront();
    while(D.position() < D.length()/2) {
        A.insertBefore(D.moveNext());
    }

    while(D.position() < D.length()) {
        B.insertBefore(D.moveNext());
    }

    //Merge back into D
    A.moveFront();
    B.moveFront();

    D.clear();

    while(B.position() < B.length()) {
        if(B.position() < B.length()) {
            D.insertBefore(B.peekNext());
            B.moveNext();
        }
        if(A.position() <  A.length()) {
            D.insertBefore(A.peekNext());
            A.moveNext();
        }
    }
}