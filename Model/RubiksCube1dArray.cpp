#include "RubiksCube1dArray.h"
#include <cstdlib>

inline int RubiksCube1dArray::getIndex(int ind, int row, int col) const {
    return (ind * 9) + (row * 3) + col;
}

void RubiksCube1dArray::rotateFace(int ind) {
    char temp_arr[9] = {};
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            temp_arr[i * 3 + j] = cube[getIndex(ind, i, j)];
        }
    }
    for (int i = 0; i < 3; i++) cube[getIndex(ind, 0, i)] = temp_arr[(2 - i) * 3 + 0];
    for (int i = 0; i < 3; i++) cube[getIndex(ind, i, 2)] = temp_arr[0 * 3 + i];
    for (int i = 0; i < 3; i++) cube[getIndex(ind, 2, 2 - i)] = temp_arr[i * 3 + 2];
    for (int i = 0; i < 3; i++) cube[getIndex(ind, 2 - i, 0)] = temp_arr[2 * 3 + (2 - i)];
}

RubiksCube1dArray::RubiksCube1dArray() {
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                cube[i * 9 + j * 3 + k] = getColorLetter(COLOR(i));
            }
        }
    }
}

RubiksCube::COLOR RubiksCube1dArray::getColor(FACE face, unsigned row, unsigned col) const {
    char color = cube[getIndex(int(face), row, col)];
    switch (color) {
        case 'B': return COLOR::BLUE;
        case 'R': return COLOR::RED;
        case 'G': return COLOR::GREEN;
        case 'O': return COLOR::ORANGE;
        case 'Y': return COLOR::YELLOW;
        case 'W': return COLOR::WHITE;
        default: return COLOR::WHITE;
    }
}

bool RubiksCube1dArray::isSolved() const {
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                if (cube[getIndex(i, j, k)] != getColorLetter(COLOR(i))) return false;
            }
        }
    }
    return true;
}

RubiksCube& RubiksCube1dArray::u() {
    this->rotateFace(0);

    char temp_arr[3] = {};
    for (int i = 0; i < 3; i++) temp_arr[i] = cube[getIndex(4, 0, 2 - i)];
    for (int i = 0; i < 3; i++) cube[getIndex(4, 0, 2 - i)] = cube[getIndex(1, 0, 2 - i)];
    for (int i = 0; i < 3; i++) cube[getIndex(1, 0, 2 - i)] = cube[getIndex(2, 0, 2 - i)];
    for (int i = 0; i < 3; i++) cube[getIndex(2, 0, 2 - i)] = cube[getIndex(3, 0, 2 - i)];
    for (int i = 0; i < 3; i++) cube[getIndex(3, 0, 2 - i)] = temp_arr[i];

    return *this;
}

RubiksCube& RubiksCube1dArray::uPrime() {
    this->u();
    this->u();
    this->u();
    return *this;
}

RubiksCube& RubiksCube1dArray::u2() {
    this->u();
    this->u();
    return *this;
}

RubiksCube& RubiksCube1dArray::l() {
    this->rotateFace(1);

    char temp_arr[3] = {};
    for (int i = 0; i < 3; i++) temp_arr[i] = cube[getIndex(0, i, 0)];
    for (int i = 0; i < 3; i++) cube[getIndex(0, i, 0)] = cube[getIndex(4, 2 - i, 2)];
    for (int i = 0; i < 3; i++) cube[getIndex(4, 2 - i, 2)] = cube[getIndex(5, i, 0)];
    for (int i = 0; i < 3; i++) cube[getIndex(5, i, 0)] = cube[getIndex(2, i, 0)];
    for (int i = 0; i < 3; i++) cube[getIndex(2, i, 0)] = temp_arr[i];

    return *this;
}

RubiksCube& RubiksCube1dArray::lPrime() {
    this->l();
    this->l();
    this->l();
    return *this;
}

RubiksCube& RubiksCube1dArray::l2() {
    this->l();
    this->l();
    return *this;
}

RubiksCube& RubiksCube1dArray::f() {
    this->rotateFace(2);

    char temp_arr[3] = {};
    for (int i = 0; i < 3; i++) temp_arr[i] = cube[getIndex(0, 2, i)];
    for (int i = 0; i < 3; i++) cube[getIndex(0, 2, i)] = cube[getIndex(1, 2 - i, 2)];
    for (int i = 0; i < 3; i++) cube[getIndex(1, 2 - i, 2)] = cube[getIndex(5, 0, 2 - i)];
    for (int i = 0; i < 3; i++) cube[getIndex(5, 0, 2 - i)] = cube[getIndex(3, i, 0)];
    for (int i = 0; i < 3; i++) cube[getIndex(3, i, 0)] = temp_arr[i];

    return *this;
}

RubiksCube& RubiksCube1dArray::fPrime() {
    this->f();
    this->f();
    this->f();
    return *this;
}

RubiksCube& RubiksCube1dArray::f2() {
    this->f();
    this->f();
    return *this;
}

RubiksCube& RubiksCube1dArray::r() {
    this->rotateFace(3);

    char temp_arr[3] = {};
    for (int i = 0; i < 3; i++) temp_arr[i] = cube[getIndex(0, 2 - i, 2)];
    for (int i = 0; i < 3; i++) cube[getIndex(0, 2 - i, 2)] = cube[getIndex(2, 2 - i, 2)];
    for (int i = 0; i < 3; i++) cube[getIndex(2, 2 - i, 2)] = cube[getIndex(5, 2 - i, 2)];
    for (int i = 0; i < 3; i++) cube[getIndex(5, 2 - i, 2)] = cube[getIndex(4, i, 0)];
    for (int i = 0; i < 3; i++) cube[getIndex(4, i, 0)] = temp_arr[i];

    return *this;
}

RubiksCube& RubiksCube1dArray::rPrime() {
    this->r();
    this->r();
    this->r();
    return *this;
}

RubiksCube& RubiksCube1dArray::r2() {
    this->r();
    this->r();
    return *this;
}

RubiksCube& RubiksCube1dArray::b() {
    this->rotateFace(4);

    char temp_arr[3] = {};
    for (int i = 0; i < 3; i++) temp_arr[i] = cube[getIndex(0, 0, 2 - i)];
    for (int i = 0; i < 3; i++) cube[getIndex(0, 0, 2 - i)] = cube[getIndex(3, 2 - i, 2)];
    for (int i = 0; i < 3; i++) cube[getIndex(3, 2 - i, 2)] = cube[getIndex(5, 2, i)];
    for (int i = 0; i < 3; i++) cube[getIndex(5, 2, i)] = cube[getIndex(1, i, 0)];
    for (int i = 0; i < 3; i++) cube[getIndex(1, i, 0)] = temp_arr[i];

    return *this;
}

RubiksCube& RubiksCube1dArray::bPrime() {
    this->b();
    this->b();
    this->b();
    return *this;
}

RubiksCube& RubiksCube1dArray::b2() {
    this->b();
    this->b();
    return *this;
}

RubiksCube& RubiksCube1dArray::d() {
    this->rotateFace(5);

    char temp_arr[3] = {};
    for (int i = 0; i < 3; i++) temp_arr[i] = cube[getIndex(2, 2, i)];
    for (int i = 0; i < 3; i++) cube[getIndex(2, 2, i)] = cube[getIndex(1, 2, i)];
    for (int i = 0; i < 3; i++) cube[getIndex(1, 2, i)] = cube[getIndex(4, 2, i)];
    for (int i = 0; i < 3; i++) cube[getIndex(4, 2, i)] = cube[getIndex(3, 2, i)];
    for (int i = 0; i < 3; i++) cube[getIndex(3, 2, i)] = temp_arr[i];

    return *this;
}

RubiksCube& RubiksCube1dArray::dPrime() {
    this->d();
    this->d();
    this->d();
    return *this;
}

RubiksCube& RubiksCube1dArray::d2() {
    this->d();
    this->d();
    return *this;
}