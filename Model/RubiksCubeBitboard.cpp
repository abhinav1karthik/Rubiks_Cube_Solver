#include "RubiksCubeBitboard.h"
#include <functional>
typedef RubiksCube::FACE FACE;
typedef RubiksCube::COLOR COLOR;

RubiksCubeBitboard::RubiksCubeBitboard() {
    for (int side = 0; side < 6; side++) {
        uint64_t clr = 1ULL << side;
        bitboard[side] = 0;
        for (int idx = 0; idx < 8; idx++) {
            bitboard[side] |= clr << (8 * idx);
        }
        all_colors[side] = bitboard[side];
    }
}

void RubiksCubeBitboard::rotateFace(int ind) {
    uint64_t side = bitboard[ind] >> (8 * 6);
    bitboard[ind] = (bitboard[ind] << 16) | side;
}

void RubiksCubeBitboard::rotateSide(int s1, int s1_1, int s1_2, int s1_3,
                                   int s2, int s2_1, int s2_2, int s2_3) {
    uint64_t clr1 = (bitboard[s2] >> (8 * s2_1)) & one_8;
    uint64_t clr2 = (bitboard[s2] >> (8 * s2_2)) & one_8;
    uint64_t clr3 = (bitboard[s2] >> (8 * s2_3)) & one_8;

    bitboard[s1] &= ~(one_8 << (8 * s1_1));
    bitboard[s1] |= (clr1 << (8 * s1_1));

    bitboard[s1] &= ~(one_8 << (8 * s1_2));
    bitboard[s1] |= (clr2 << (8 * s1_2));

    bitboard[s1] &= ~(one_8 << (8 * s1_3));
    bitboard[s1] |= (clr3 << (8 * s1_3));
}

COLOR RubiksCubeBitboard::getColor(FACE face, unsigned row, unsigned col) const {
    int idx = arr[row][col];
    if (idx == 8) return static_cast<COLOR>(static_cast<int>(face));

    uint64_t color = (bitboard[static_cast<int>(face)] >> (8 * idx)) & one_8;
    int bit_pos = 0;
    while (color) {
        color >>= 1;
        bit_pos++;
    }
    return static_cast<COLOR>(bit_pos - 1);
}

bool RubiksCubeBitboard::isSolved() const {
    for (int i = 0; i < 6; i++) {
        if (all_colors[i] != bitboard[i]) return false;
    }
    return true;
}

RubiksCube& RubiksCubeBitboard::u() {
    this->rotateFace(0);
    uint64_t temp = bitboard[2] & one_24;
    bitboard[2] = (bitboard[2] & ~one_24) | (bitboard[3] & one_24);
    bitboard[3] = (bitboard[3] & ~one_24) | (bitboard[4] & one_24);
    bitboard[4] = (bitboard[4] & ~one_24) | (bitboard[1] & one_24);
    bitboard[1] = (bitboard[1] & ~one_24) | temp;
    return *this;
}

RubiksCube& RubiksCubeBitboard::uPrime() {
    this->u();
    this->u();
    this->u();
    return *this;
}

RubiksCube& RubiksCubeBitboard::u2() {
    this->u();
    this->u();
    return *this;
}

RubiksCube& RubiksCubeBitboard::l() {
    this->rotateFace(1);
    uint64_t clr1 = (bitboard[2] >> (8 * 0)) & one_8;
    uint64_t clr2 = (bitboard[2] >> (8 * 6)) & one_8;
    uint64_t clr3 = (bitboard[2] >> (8 * 7)) & one_8;

    this->rotateSide(2, 0, 7, 6, 0, 0, 7, 6);
    this->rotateSide(0, 0, 7, 6, 4, 4, 3, 2);
    this->rotateSide(4, 4, 3, 2, 5, 0, 7, 6);

    bitboard[5] &= ~(one_8 << (8 * 0));
    bitboard[5] |= (clr1 << (8 * 0));

    bitboard[5] &= ~(one_8 << (8 * 6));
    bitboard[5] |= (clr2 << (8 * 6));

    bitboard[5] &= ~(one_8 << (8 * 7));
    bitboard[5] |= (clr3 << (8 * 7));

    return *this;
}

RubiksCube& RubiksCubeBitboard::lPrime() {
    this->l();
    this->l();
    this->l();
    return *this;
}

RubiksCube& RubiksCubeBitboard::l2() {
    this->l();
    this->l();
    return *this;
}

RubiksCube& RubiksCubeBitboard::f() {
    this->rotateFace(2);
    uint64_t clr1 = (bitboard[0] >> (8 * 4)) & one_8;
    uint64_t clr2 = (bitboard[0] >> (8 * 5)) & one_8;
    uint64_t clr3 = (bitboard[0] >> (8 * 6)) & one_8;

    this->rotateSide(0, 4, 5, 6, 1, 2, 3, 4);
    this->rotateSide(1, 2, 3, 4, 5, 0, 1, 2);
    this->rotateSide(5, 0, 1, 2, 3, 6, 7, 0);

    bitboard[3] &= ~(one_8 << (8 * 6));
    bitboard[3] |= (clr1 << (8 * 6));

    bitboard[3] &= ~(one_8 << (8 * 7));
    bitboard[3] |= (clr2 << (8 * 7));

    bitboard[3] &= ~(one_8 << (8 * 0));
    bitboard[3] |= (clr3 << (8 * 0));

    return *this;
}

RubiksCube& RubiksCubeBitboard::fPrime() {
    this->f();
    this->f();
    this->f();
    return *this;
}

RubiksCube& RubiksCubeBitboard::f2() {
    this->f();
    this->f();
    return *this;
}

RubiksCube& RubiksCubeBitboard::r() {
    this->rotateFace(3);
    uint64_t clr1 = (bitboard[0] >> (8 * 2)) & one_8;
    uint64_t clr2 = (bitboard[0] >> (8 * 3)) & one_8;
    uint64_t clr3 = (bitboard[0] >> (8 * 4)) & one_8;

    this->rotateSide(0, 2, 3, 4, 2, 2, 3, 4);
    this->rotateSide(2, 2, 3, 4, 5, 2, 3, 4);
    this->rotateSide(5, 2, 3, 4, 4, 7, 6, 0);

    bitboard[4] &= ~(one_8 << (8 * 7));
    bitboard[4] |= (clr1 << (8 * 7));

    bitboard[4] &= ~(one_8 << (8 * 6));
    bitboard[4] |= (clr2 << (8 * 6));

    bitboard[4] &= ~(one_8 << (8 * 0));
    bitboard[4] |= (clr3 << (8 * 0));

    return *this;
}

RubiksCube& RubiksCubeBitboard::rPrime() {
    this->r();
    this->r();
    this->r();
    return *this;
}

RubiksCube& RubiksCubeBitboard::r2() {
    this->r();
    this->r();
    return *this;
}

RubiksCube& RubiksCubeBitboard::b() {
    this->rotateFace(4);
    uint64_t clr1 = (bitboard[0] >> (8 * 0)) & one_8;
    uint64_t clr2 = (bitboard[0] >> (8 * 1)) & one_8;
    uint64_t clr3 = (bitboard[0] >> (8 * 2)) & one_8;

    this->rotateSide(0, 0, 1, 2, 3, 2, 3, 4);
    this->rotateSide(3, 2, 3, 4, 5, 4, 5, 6);
    this->rotateSide(5, 4, 5, 6, 1, 6, 7, 0);

    bitboard[1] &= ~(one_8 << (8 * 6));
    bitboard[1] |= (clr1 << (8 * 6));

    bitboard[1] &= ~(one_8 << (8 * 7));
    bitboard[1] |= (clr2 << (8 * 7));

    bitboard[1] &= ~(one_8 << (8 * 0));
    bitboard[1] |= (clr3 << (8 * 0));

    return *this;
}

RubiksCube& RubiksCubeBitboard::bPrime() {
    this->b();
    this->b();
    this->b();
    return *this;
}

RubiksCube& RubiksCubeBitboard::b2() {
    this->b();
    this->b();
    return *this;
}

RubiksCube& RubiksCubeBitboard::d() {
    this->rotateFace(5);
    uint64_t clr1 = (bitboard[2] >> (8 * 4)) & one_8;
    uint64_t clr2 = (bitboard[2] >> (8 * 5)) & one_8;
    uint64_t clr3 = (bitboard[2] >> (8 * 6)) & one_8;

    this->rotateSide(2, 4, 5, 6, 1, 4, 5, 6);
    this->rotateSide(1, 4, 5, 6, 4, 4, 5, 6);
    this->rotateSide(4, 4, 5, 6, 3, 4, 5, 6);

    bitboard[3] &= ~(one_8 << (8 * 4));
    bitboard[3] |= (clr1 << (8 * 4));

    bitboard[3] &= ~(one_8 << (8 * 5));
    bitboard[3] |= (clr2 << (8 * 5));

    bitboard[3] &= ~(one_8 << (8 * 6));
    bitboard[3] |= (clr3 << (8 * 6));

    return *this;
}

RubiksCube& RubiksCubeBitboard::dPrime() {
    this->d();
    this->d();
    this->d();
    return *this;
}

RubiksCube& RubiksCubeBitboard::d2() {
    this->d();
    this->d();
    return *this;
}

bool RubiksCubeBitboard::operator==(const RubiksCubeBitboard &r1) const {
    for (int i = 0; i < 6; i++) {
        if (bitboard[i] != r1.bitboard[i]) return false;
    }
    return true;
}

RubiksCubeBitboard &RubiksCubeBitboard::operator=(const RubiksCubeBitboard &r1) {
    for (int i = 0; i < 6; i++) {
        bitboard[i] = r1.bitboard[i];
    }
    return *this;
}

size_t HashBitboard::operator()(const RubiksCubeBitboard &r1) const {
    uint64_t final_hash = r1.bitboard[0];
    for (int i = 1; i < 6; i++) final_hash ^= r1.bitboard[i];
    return static_cast<size_t>(final_hash);
}