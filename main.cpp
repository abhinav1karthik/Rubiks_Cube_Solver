#include <iostream>
#include "Model/RubiksCube3dArray.h"
#include "Model/RubiksCube1dArray.h"

int main() {
    RubiksCube1dArray cube;  // Or RubiksCube3dArray
    cube.print();

    if (cube.isSolved()) std::cout << "SOLVED\n";
    else std::cout << "NOT SOLVED\n";

    cube.f().u().r().b().l().d().print();

    if (cube.isSolved()) std::cout << "SOLVED\n";
    else std::cout << "NOT SOLVED\n";

    cube.invert(RubiksCube1dArray::MOVE::D).invert(RubiksCube1dArray::MOVE::L).invert(RubiksCube1dArray::MOVE::B).invert(RubiksCube1dArray::MOVE::R).invert(RubiksCube1dArray::MOVE::U).invert(RubiksCube1dArray::MOVE::F).print();

    if (cube.isSolved()) std::cout << "SOLVED\n";
    else std::cout << "NOT SOLVED\n";

    return 0;
}