/*
Jordan Reavis
jreavis
Spring 23 - CSE101 - pa6
BigInteger.cpp - BigInteger ADT implementation file
*/

#include <iostream>
#include <string>
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include "List.h"
#include "BigInteger.h"

//BigInt base constants
//const ListElement base = 1000000000; // 10^(power)
const int64_t base = 1000000000; // 10^(power)
const int power = 9; // log_10(base)

//BigInteger helper functions------------------------------------------------------------

//Changes the sign of each int in List L. Used by sub()
void negateList(List &L) {
    L.moveFront();
    while(L.position() < L.length()) {
        ListElement x = L.moveNext();
        if(x == 0) {
            L.moveNext();
        } else {
            L.setBefore(-x);
        }
    }
}

//Overwrites S with A + sgn*B (considered as vectors). Used by sum() and sub()
void sumList(List &S, List A, List B, int sgn) {
    ListElement x, y;
    S.clear();

    A.moveFront();
    B.moveFront();

    while(A.position() < A.length() && B.position() < B.length()) {
        x = A.moveNext();
        y = B.moveNext();
        S.insertBefore(x + sgn*y);
    }
    while(A.position() < A.length()) {
        x = A.moveNext();
        S.insertBefore(x);
    }
    while(B.position() < B.length()) {
        y = B.moveNext();
        S.insertBefore(y*sgn);
    }
}

//Performs carries from right to left (least to most significant digits), then returns
//the sign of the resulting integer. Used by add(), sub(), and mult()
int normalizeList(List &L) {
    int sgn;
    ListElement q, r;
    ListElement carryIn, curCol, carryOut = 0;
    L.moveBack();

    //performs carries from right (front) to left (back)
    while(L.position() > 0) {
        carryIn = carryOut;
        curCol = L.movePrev();
        curCol += carryIn;
        q = curCol / base;
        r = curCol % base;
        if(r < 0) {
            curCol = r + base;
            carryOut = q - 1;
        } else {
            curCol = r;
            carryOut = q;
        }

        L.setAfter(curCol);
       
        if(L.position() == 0) {
            if(carryOut > 0) {
                L.insertBefore(0);
            } else if(carryOut < 0) {
                L.insertBefore(carryOut);
                break;
            }
        }
    }

    //erase all leading zeros from L
    L.moveFront();
    while(L.length() > 0) {
        if(L.peekNext() == 0) {
            L.eraseAfter();
        } else {
            break;
        }
    }

    //determine and return the sign
    if(L.length() != 0) {
        L.moveFront();
        if(L.peekNext() > 0) {
            sgn = 1;
        } else if(L.peekNext() < 0) {
            sgn = -1;
            negateList(L);
            normalizeList(L);
        } 
    }
    else {
            sgn = 0;
        }
    return sgn;
}

//Prepends p zero digits to L, multiplying L by base^p. Used by mult()
void shiftList(List &L, int p) {
    L.moveFront();
    for(int i = 0; i < p; i++) {
        L.insertAfter(0);
    }
}

//Multiplies L (considered as a vector) by m. Used by mult()
void scalarMultList(List &L, ListElement m) {
    if(m != 0) {
        L.moveFront();
        while(L.position() < L.length()) {
            ListElement x = L.moveNext();
            if(x == 0) {
                L.moveNext();
            } else {
                L.eraseBefore();
                L.insertBefore(x * m);
            }
        }
    } else {
        L.clear();
    }
}

//End of BigInteger helper functions--------------------------------------------

//Class constructors & destructors----------------------------------------------

//Creates a BigInteger in the empty state
BigInteger::BigInteger() {
    signum = 0;
    digits = List();
}

//Creates a new BigInteger from the long value x
BigInteger::BigInteger(long x) {
    if(x < 0) {
        signum = -1;
    } else if (x > 1) {
        signum = 1;
    } else {
        signum = 0;
    }

    digits = List();
    
    while(digits.position() < digits.length()) {
        digits.insertBefore(x);
    }
}

//Creates a new BigInteger from the string s.
//Pre: s is a non-empty string consisting of (at least one) base 10 digit
// {0, 1, 2, 3, 4, 5, 6, 7, 8, 9} and an optional sign {-, +} prefix.
BigInteger::BigInteger(std::string s) {
    int n = s.length(); //holds the length of string s
    int r, i;

    //check empty string
    if(n <= 0) {
        throw std::invalid_argument("BigInteger: Constructor: empty string");
    }

    //get preliminary signum
    if(s[0] == '-') {
        signum = -1;
        s.erase(0, 1);
        n--;
    } else if(s[0] == '+') {
        signum = 1;
        s.erase(0, 1);
        n--;
    } else {
        signum = 1;
    }

    //eliminate leading zeros
    while(s[0] == '0') {
        s.erase(0, 1);
        n--;
    }

    //after eliminating zeros, re-check size and non numeric characters
    if(n <= 0) {
        throw std::invalid_argument("BigInteger: Constructor: empty string");
    }

    for(i = 0; i < n; i++) {
        if(!(isdigit(s[i]))) {
            std::cout << s[i];
            throw std::invalid_argument("BigInteger: Constructor: non-numeric string");
        }
    }

    //create empty digits List

    if(s[0] != '0') {
        r = n % power;
        if(r > 0) {
            long x = stol(s.substr(0, r));
            digits.insertBefore(x);
        }
        for(i = r; i < n; i += power) {
            long x = stol(s.substr(i, power));
            digits.insertBefore(x);
        }
    }
}

//Constructor that creates a copy of N
BigInteger::BigInteger(const BigInteger &N) {
    this->signum = N.signum;
    this->digits = N.digits;
}

//Access functions
int BigInteger::sign() const {
    return (signum);
}

//Returns -1, 1, or 0 if this BigInt is less than N, greater than N, or equal to N respectively
int BigInteger::compare(const BigInteger &N) const {
    int ret;
    List L = this->digits;
    List R = N.digits;

    if(signum > N.signum) {
        ret = 1;
        return ret;
    } else if(signum < N.signum) {
        ret = -1;
        return ret;
    } else {
        ret = 0;
    }

    if(L.length() > R.length()) {
        ret = 1;
        if(signum == -1) {
            ret = -1;
        }
        return ret;
    } else if (L.length() < R.length()) {
        ret = -1;
        if(signum == 1) {
            ret = 1;
        }
        return ret;
    } else {
        ret = 0;
    }

    L.moveFront();
    R.moveFront(); 
    while(L.position() < L.length() && R.position() < R.length())
    {
        int x = L.moveNext();
        int y = R.moveNext();
        if(x == y) {
            ret = 0;
        } else if(x < y){
            ret = -1;
            return ret;
        } else {
            ret = 1;
            return ret;
        }
    }
    return ret;
}

//Manipulation procedures
void BigInteger::makeZero() {
    signum = 0;
    digits.moveFront();

    while(digits.position() < digits.length()) {
        digits.eraseAfter();
    }
}

void BigInteger::negate() {
    if(signum == 0) {
        return;
    } else if (signum == 1) {
        signum = -1;
    } else {
        signum = 1;
    }
}

//BigInteger arithmetic operations
BigInteger BigInteger::add(const BigInteger &N) const {
    BigInteger Sum;
    int sgn;

    List A = this->digits;
    List B = N.digits;
    List &S = Sum.digits;

    if(N.signum == this->signum) {
        sumList(S, A, B, 1);
        normalizeList(S);
        Sum.signum = this->signum;
    } else if(N.signum != this->signum){
        sumList(S, A, B, -1);
        sgn = normalizeList(S);
        Sum.signum = (sgn * this->signum);
    }

    return Sum;
}

BigInteger BigInteger::sub(const BigInteger &N) const {
    BigInteger A = BigInteger(N);
    A.negate();
    return add(A);
}

BigInteger BigInteger::mult(const BigInteger &N) const {
    int col = 0;
    BigInteger Prod;
    List A = this->digits;
    List B = N.digits;
    List C;
    List &P = Prod.digits;
    Prod.signum = this->signum * N.signum;

    B.moveFront();
    while(B.position() < B.length()) {
        C = A;
        ListElement x = B.moveNext();
        C.moveFront();
        scalarMultList(C, x);

        shiftList(C, col);
        sumList(P, C, P, 1);
        normalizeList(P);
        col++;
    }
    return Prod;
}

//Other functions
std::string BigInteger::to_string() {
    ListElement x;
    std::string s;
    std::string data;

    if(signum == -1) {
        s.append("-");
    } 

    if(signum == 0) {
        s.append("0");
    } else {
        digits.moveFront();
        x = digits.moveNext();
        data = std::to_string(x);
        s.append(data);

        while(digits.position() < digits.length()) {
            x = digits.moveNext();
            std::ostringstream ss;
            ss << std::setw(power) << std::setfill('0') << x;
            data = ss.str();
            s.append(data);
        }
    }

    return s;
}

//Overridden operators
std::ostream &operator<<(std::ostream &stream, BigInteger N) {
    return stream << N.BigInteger::to_string();
}

bool operator ==(const BigInteger &A, const BigInteger &B) {
    if(A.compare(B) == 0) {
        return true;
    } else {
        return false;
    }
}

bool operator <(const BigInteger &A, const BigInteger &B) {
    if(A.compare(B) == -1) {
        return true;
    } else {
        return false;
    }
}

bool operator <=(const BigInteger &A, const BigInteger &B) {
    if(A.compare(B) == -1 || A.compare(B) == 0) {
        return true;
    } else {
        return false;
    }
}

bool operator >(const BigInteger &A, const BigInteger &B) {
    if(A.compare(B) == 1) {
        return true;
    } else {
        return false;
    }
}

bool operator >=(const BigInteger &A, const BigInteger &B) {
    if(A.compare(B) == 1 || A.compare(B) == 0) {
        return true;
    } else {
        return false;
    }
}

BigInteger operator+(const BigInteger &A, const BigInteger &B) {
    return A.add(B);
}

BigInteger operator+=(BigInteger &A, const BigInteger &B) {
    A = A + B;
    return A;
}

BigInteger operator-(const BigInteger &A, const BigInteger &B) {
    return A.sub(B);
}

BigInteger operator-=(BigInteger &A, const BigInteger &B) {
    A = A.sub(B);
    return A;
}

BigInteger operator*(const BigInteger &A, const BigInteger &B) {
    return A.mult(B);
}

BigInteger operator*=(BigInteger &A, const BigInteger &B) {
    A = A * B;
    return A;
}