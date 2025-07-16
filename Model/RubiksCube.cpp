#include "RubiksCube.h"

char RubiksCube::getColorLetter(COLOR color) const {
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

RubiksCube &RubiksCube::randomShuffleCube(unsigned int times) {
    for (unsigned int i = 0; i < times; i++) {
        unsigned int selectMove = (rand() % 18);
        this->move(static_cast<MOVE>(selectMove));
    }
    return *this;
}