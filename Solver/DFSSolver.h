#ifndef RUBIKS_CUBE_SOLVER_DFSSOLVER_H
#define RUBIKS_CUBE_SOLVER_DFSSOLVER_H

#include <unordered_map>
#include <vector>
#include "../Model/RubiksCube.h"

template<typename T, typename H>
class DFSSolver {
private:
    std::unordered_map<T, bool, H> visited;
    std::unordered_map<T, int, H>  depth;
    std::vector<RubiksCube::MOVE>  moves;
    int                            max_search_depth;

    bool dfs(int dep) {
        if (rubiksCube.isSolved())     return true;
        if (dep > max_search_depth)    return false;
        visited[rubiksCube] = true;
        depth[rubiksCube]   = dep;
        for (int i = 0; i < 18; i++) {
            rubiksCube.move(RubiksCube::MOVE(i));
            moves.push_back(RubiksCube::MOVE(i));
            if ((!visited[rubiksCube] || depth[rubiksCube] > dep)
                 && dfs(dep + 1))
                return true;
            moves.pop_back();
            rubiksCube.invert(RubiksCube::MOVE(i));
        }
        return false;
    }

public:
    T rubiksCube;

    DFSSolver(T _rubiksCube, int _max_search_depth = 8)
      : rubiksCube(_rubiksCube),
        max_search_depth(_max_search_depth)
    {}

    std::vector<RubiksCube::MOVE> solve() {
        dfs(1);
        return moves;
    }
};

#endif // RUBIKS_CUBE_SOLVER_DFSSOLVER_H
