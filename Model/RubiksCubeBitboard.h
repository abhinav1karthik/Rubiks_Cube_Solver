#ifndef RUBIKSCUBEBITBOARD_H
#define RUBIKSCUBEBITBOARD_H

#include "RubiksCube.h"
#include <cstdint>

class RubiksCubeBitboard : public RubiksCube {
private:
    uint64_t all_colors[6];
    static constexpr int arr[3][3] = {{0, 1, 2}, {7, 8, 3}, {6, 5, 4}};

    // Bitmask constants
    static constexpr uint64_t one_8 = (1ULL << 8) - 1;
    static constexpr uint64_t one_16 = (1ULL << 16) - 1;
    static constexpr uint64_t one_24 = (1ULL << 24) - 1;
    static constexpr uint64_t one_48 = (1ULL << 48) - 1;

    void rotateFace(int ind);
    void rotateSide(int s1, int s1_1, int s1_2, int s1_3,
                   int s2, int s2_1, int s2_2, int s2_3);

public:
    uint64_t bitboard[6];
    RubiksCubeBitboard();

    COLOR getColor(FACE face, unsigned row, unsigned col) const override;
    bool isSolved() const override;

    // Clockwise moves
    RubiksCube &u() override;
    RubiksCube &l() override;
    RubiksCube &f() override;
    RubiksCube &r() override;
    RubiksCube &b() override;
    RubiksCube &d() override;

    // Counter-clockwise moves
    RubiksCube &uPrime() override;
    RubiksCube &lPrime() override;
    RubiksCube &fPrime() override;
    RubiksCube &rPrime() override;
    RubiksCube &bPrime() override;
    RubiksCube &dPrime() override;

    // Double moves
    RubiksCube &u2() override;
    RubiksCube &l2() override;
    RubiksCube &f2() override;
    RubiksCube &r2() override;
    RubiksCube &b2() override;
    RubiksCube &d2() override;
    bool operator==(const RubiksCubeBitboard &r1) const;
    RubiksCubeBitboard &operator=(const RubiksCubeBitboard &r1);
};

struct HashBitboard {
    size_t operator()(const RubiksCubeBitboard &r1) const;
};

#endif // RUBIKSCUBEBITBOARD_H