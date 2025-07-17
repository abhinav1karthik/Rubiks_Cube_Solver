#include <iostream>
#include <vector>           // vector
#include <unordered_map>    // unordered_map

#include "Model/RubiksCube3dArray.h"
#include "Model/RubiksCube1dArray.h"
#include "Model/RubiksCubeBitboard.h"

using namespace std;

int main() {
    // Example usage with 3D array representation
    RubiksCube3dArray cube1;
    RubiksCube3dArray cube2;

    // Example usage with 1D array representation
    // RubiksCube1dArray cube1;
    // RubiksCube1dArray cube2;

    // Example usage with bitboard representation
    // RubiksCubeBitboard cube1;
    // RubiksCubeBitboard cube2;

    // Equality and assignment test
    if(cube1 == cube2) cout << "Cubes are equal\n";
    else cout << "Cubes are not equal\n";

    // Random shuffle
    vector<RubiksCube::MOVE> moves = cube1.randomShuffleCube(5);
    cout << "Shuffled with moves: ";
    for(auto move : moves) cout << cube1.getMove(move) << " ";
    cout << endl;

    if(cube1 == cube2) cout << "Cubes are equal\n";
    else cout << "Cubes are not equal\n";

    cube2 = cube1;
    if(cube1 == cube2) cout << "Cubes are equal after assignment\n";
    else cout << "Cubes are not equal after assignment\n";

    // Unordered_map test
    unordered_map<RubiksCube3dArray, bool, Hash3d> mp1;
    // For 1D: unordered_map<RubiksCube1dArray, bool, Hash1d> mp1;
    // For Bitboard: unordered_map<RubiksCubeBitboard, bool, HashBitboard> mp1;

    mp1[cube1] = true;
    cube2.randomShuffleCube(8);

    if (mp1[cube1]) cout << "Cube1 is present\n";
    else cout << "Cube1 is not present\n";

    if (mp1[cube2]) cout << "Cube2 is present\n";
    else cout << "Cube2 is not present\n";

    return 0;
}