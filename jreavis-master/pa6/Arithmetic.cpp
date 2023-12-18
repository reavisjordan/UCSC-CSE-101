/*
Jordan Reavis
jreavis
Spring 23 - CSE101 - pa6
Arithmetic.cpp - BigInteger ADT client file
*/

#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <cmath>
#include <sstream>
#include <iomanip>
#include "List.h"
#include "BigInteger.h"

#define MAX_LEN 3

using namespace std;

int main(int argc, char* argv[]) {
    ifstream in;
    ofstream out;
    string s1, e, s2;
    BigInteger A, B;

    if(argc != 3) {
        cerr << "Usage: " << argv[0] << " <input file> <output file>" << endl;
        return(EXIT_FAILURE);
    }

    in.open(argv[1]);
    if(!in.is_open()) {
        cerr << "Unable to open file " << argv[1] << " for reading" << endl;
        return(EXIT_FAILURE);
    }

    out.open(argv[2]);
    if(!out.is_open()) {
        cerr << "Unable to open file " << argv[2] << " for writing" << endl;
        return(EXIT_FAILURE);
    }
    
    getline(in, s1);
    getline(in, e);
    getline(in, s2);

    A = BigInteger(s1);
    B = BigInteger(s2);
    BigInteger C = BigInteger("9");
    BigInteger D = BigInteger("16");
    BigInteger E = BigInteger("3");
    BigInteger F = BigInteger("2");
    
    out << A << endl;
    out << '\n';

    out << B << endl;
    out << '\n';

    out << A+B << endl;
    out << '\n';

    out << A-B << endl;
    out << '\n';

    out << A-A << endl;
    out << '\n';

    out << (E*A) - (F*B) << endl;
    out << '\n';

    out << A*B << endl;
    out << '\n';

    out << A*A << endl;
    out << '\n';

    out << B*B << endl;
    out << '\n';

    out << C*(A * A * A * A) + D*(B * B * B * B * B)<< endl;
    out << '\n';

    in.close();
    out.close();

    return 0;
}