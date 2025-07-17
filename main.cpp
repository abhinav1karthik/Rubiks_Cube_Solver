#include <iostream>
#include "Model/RubiksCube3dArray.h"
#include "Model/RubiksCube1dArray.h"
#include "Model/RubiksCubeBitboard.h"

int main() {
    RubiksCubeBitboard cube;
    cube.print();

    // Test moves
    cube.f().print();

    // Test solving
    if (cube.isSolved()) {
        std::cout << "Cube is solved!" << std::endl;
    } else {
        std::cout << "Cube is NOT solved!" << std::endl;
    }

    return 0;
}