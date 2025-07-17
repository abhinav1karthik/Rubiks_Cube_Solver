#ifndef RUBIKSCUBE1DARRAY_H
#define RUBIKSCUBE1DARRAY_H

#include "RubiksCube.h"

class RubiksCube1dArray : public RubiksCube {
public:
    char cube[54];
    static inline int getIndex(int ind, int row, int col){
        return (ind * 9) + (row * 3) + col;
    }

    void rotateFace(int ind);
    RubiksCube1dArray();
    COLOR getColor(FACE face, unsigned row, unsigned col) const override;
    bool isSolved() const override;

    RubiksCube& u() override;
    RubiksCube& uPrime() override;
    RubiksCube& u2() override;
    RubiksCube& l() override;
    RubiksCube& lPrime() override;
    RubiksCube& l2() override;
    RubiksCube& f() override;
    RubiksCube& fPrime() override;
    RubiksCube& f2() override;
    RubiksCube& r() override;
    RubiksCube& rPrime() override;
    RubiksCube& r2() override;
    RubiksCube& b() override;
    RubiksCube& bPrime() override;
    RubiksCube& b2() override;
    RubiksCube& d() override;
    RubiksCube& dPrime() override;
    RubiksCube& d2() override;
    bool operator==(const RubiksCube1dArray &r1) const;
    RubiksCube1dArray &operator=(const RubiksCube1dArray &r1);
};

struct Hash1d {
    size_t operator()(const RubiksCube1dArray &r1) const;
};

#endif