#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include "Model/RubiksCube3dArray.h"
#include "Model/RubiksCube1dArray.h"
#include "Model/RubiksCubeBitboard.h"
#include "Solver/DFSSolver.h"

using namespace std;

int main() {
    RubiksCube1dArray cube;
    cube.print();
    RubiksCube1dArray myrc=cube;
    vector<RubiksCube::MOVE> shuffle_moves = cube.randomShuffleCube(5);
    for (auto move: shuffle_moves) {
        cout << cube.getMove(move) << " ";
        myrc.move(move);
        myrc.print();
    }
    cout << "\n";
    myrc.print();

    DFSSolver<RubiksCube1dArray, Hash1d> dfsSolver(cube, 6);
    vector<RubiksCube::MOVE> solve_moves = dfsSolver.solve();

    for (auto move: solve_moves) {
        cout << cube.getMove(move) << " ";
        myrc.move(move);
        myrc.print();
    }
    cout << "\n";
    dfsSolver.rubiksCube.print();

    return 0;
}