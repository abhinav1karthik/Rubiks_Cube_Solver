#include "RubiksCube.h"
#include <cstdlib>
using namespace std;
char RubiksCube::getColorLetter(COLOR color) {
    switch (color) {
        case COLOR::BLUE: return 'B';
        case COLOR::GREEN: return 'G';
        case COLOR::RED: return 'R';
        case COLOR::YELLOW: return 'Y';
        case COLOR::WHITE: return 'W';
        case COLOR::ORANGE: return 'O';
        default: return '?'; // Add default return
    }
}

std::string RubiksCube::getMove(MOVE ind) const {
    switch (ind) {
        case MOVE::L: return "L";
        case MOVE::LPRIME: return "L'";
        case MOVE::L2: return "L2";
        case MOVE::R: return "R";
        case MOVE::RPRIME: return "R'";
        case MOVE::R2: return "R2";
        case MOVE::U: return "U";
        case MOVE::UPRIME: return "U'";
        case MOVE::U2: return "U2";
        case MOVE::D: return "D";
        case MOVE::DPRIME: return "D'";
        case MOVE::D2: return "D2";
        case MOVE::F: return "F";
        case MOVE::FPRIME: return "F'";
        case MOVE::F2: return "F2";
        case MOVE::B: return "B";
        case MOVE::BPRIME: return "B'";
        case MOVE::B2: return "B2";
        default: return "?"; // Add default return
    }
}

RubiksCube &RubiksCube::move(MOVE ind) {
    switch (ind) {
        case MOVE::L: return l();
        case MOVE::LPRIME: return lPrime();
        case MOVE::L2: return l2();
        case MOVE::R: return r();
        case MOVE::RPRIME: return rPrime();
        case MOVE::R2: return r2();
        case MOVE::U: return u();
        case MOVE::UPRIME: return uPrime();
        case MOVE::U2: return u2();
        case MOVE::D: return d();
        case MOVE::DPRIME: return dPrime();
        case MOVE::D2: return d2();
        case MOVE::F: return f();
        case MOVE::FPRIME: return fPrime();
        case MOVE::F2: return f2();
        case MOVE::B: return b();
        case MOVE::BPRIME: return bPrime();
        case MOVE::B2: return b2();
        default: return *this; // Add default return
    }
}

RubiksCube &RubiksCube::invert(MOVE ind) {
    switch (ind) {
        case MOVE::L: return lPrime();
        case MOVE::LPRIME: return l();
        case MOVE::L2: return l2();
        case MOVE::R: return rPrime();
        case MOVE::RPRIME: return r();
        case MOVE::R2: return r2();
        case MOVE::U: return uPrime();
        case MOVE::UPRIME: return u();
        case MOVE::U2: return u2();
        case MOVE::D: return dPrime();
        case MOVE::DPRIME: return d();
        case MOVE::D2: return d2();
        case MOVE::F: return fPrime();
        case MOVE::FPRIME: return f();
        case MOVE::F2: return f2();
        case MOVE::B: return bPrime();
        case MOVE::BPRIME: return b();
        case MOVE::B2: return b2();
        default: return *this; // Add default return
    }
}

void RubiksCube::print() const {
    std::cout << "Rubik Cube:\n\n";

    // UP face
    for (int row = 0; row < 3; row++) {
        for (int i = 0; i < 7; i++) std::cout << " ";
        for (int col = 0; col < 3; col++) {
            std::cout << getColorLetter(getColor(FACE::UP, row, col)) << " ";
        }
        std::cout << "\n";
    }

    std::cout << "\n";

    // LEFT, FRONT, RIGHT, BACK
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            std::cout << getColorLetter(getColor(FACE::LEFT, row, col)) << " ";
        }
        std::cout << " ";

        for (int col = 0; col < 3; col++) {
            std::cout << getColorLetter(getColor(FACE::FRONT, row, col)) << " ";
        }
        std::cout << " ";

        for (int col = 0; col < 3; col++) {
            std::cout << getColorLetter(getColor(FACE::RIGHT, row, col)) << " ";
        }
        std::cout << " ";

        for (int col = 0; col < 3; col++) {
            std::cout << getColorLetter(getColor(FACE::BACK, row, col)) << " ";
        }
        std::cout << "\n";
    }

    std::cout << "\n";

    // DOWN face
    for (int row = 0; row < 3; row++) {
        for (int i = 0; i < 7; i++) std::cout << " ";
        for (int col = 0; col < 3; col++) {
            std::cout << getColorLetter(getColor(FACE::DOWN, row, col)) << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

std::vector<RubiksCube::MOVE> RubiksCube::randomShuffleCube(unsigned int times) {
    std::vector<MOVE> moves;
    srand(time(0));
    for (unsigned int i = 0; i < times; i++) {
        unsigned int selectMove = (rand() % 18);
        moves.push_back(static_cast<MOVE>(selectMove));
        this->move(static_cast<MOVE>(selectMove));
    }
    return moves;
}


//Helper function returns string of corner
string RubiksCube::getCornerColorString(uint8_t ind) const {
    string str = "";

    switch (ind) {
//        UFR
        case 0:
            str += getColorLetter(getColor(FACE::UP, 2, 2));
            str += getColorLetter(getColor(FACE::FRONT, 0, 2));
            str += getColorLetter(getColor(FACE::RIGHT, 0, 0));
            break;

//            UFL
        case 1:
            str += getColorLetter(getColor(FACE::UP, 2, 0));
            str += getColorLetter(getColor(FACE::FRONT, 0, 0));
            str += getColorLetter(getColor(FACE::LEFT, 0, 2));
            break;

//            UBL
        case 2:
            str += getColorLetter(getColor(FACE::UP, 0, 0));
            str += getColorLetter(getColor(FACE::BACK, 0, 2));
            str += getColorLetter(getColor(FACE::LEFT, 0, 0));
            break;

//            UBR
        case 3:
            str += getColorLetter(getColor(FACE::UP, 0, 2));
            str += getColorLetter(getColor(FACE::BACK, 0, 0));
            str += getColorLetter(getColor(FACE::RIGHT, 0, 2));
            break;

//            DFR
        case 4:
            str += getColorLetter(getColor(FACE::DOWN, 0, 2));
            str += getColorLetter(getColor(FACE::FRONT, 2, 2));
            str += getColorLetter(getColor(FACE::RIGHT, 2, 0));
            break;

//            DFL
        case 5:
            str += getColorLetter(getColor(FACE::DOWN, 0, 0));
            str += getColorLetter(getColor(FACE::FRONT, 2, 0));
            str += getColorLetter(getColor(FACE::LEFT, 2, 2));
            break;

//            DBR
        case 6:
            str += getColorLetter(getColor(FACE::DOWN, 2, 2));
            str += getColorLetter(getColor(FACE::BACK, 2, 0));
            str += getColorLetter(getColor(FACE::RIGHT, 2, 2));
            break;

//            DBL
        case 7:
            str += getColorLetter(getColor(FACE::DOWN, 2, 0));
            str += getColorLetter(getColor(FACE::BACK, 2, 2));
            str += getColorLetter(getColor(FACE::LEFT, 2, 0));
            break;
    }
    return str;
}

uint8_t RubiksCube::getCornerIndex(uint8_t ind) const {
    string corner = getCornerColorString(ind);

    uint8_t ret = 0;
    for (auto c: corner) {
        if (c != 'W' && c != 'Y') continue;
        if (c == 'Y') {
            ret |= (1 << 2);
        }
    }

    for (auto c: corner) {
        if (c != 'R' && c != 'O') continue;
        if (c == 'O') {
            ret |= (1 << 1);
        }
    }

    for (auto c: corner) {
        if (c != 'B' && c != 'G') continue;
        if (c == 'G') {
            ret |= (1 << 0);
        }
    }
    return ret;
}

uint8_t RubiksCube::getCornerOrientation(uint8_t ind) const {
    string corner = getCornerColorString(ind);

    string actual_str = "";

    for (auto c: corner) {
        if (c != 'W' && c != 'Y') continue;
        actual_str.push_back(c);
    }

    if (corner[1] == actual_str[0]) {
        return 1;
    } else if (corner[2] == actual_str[0]) {
        return 2;
    } else return 0;
}