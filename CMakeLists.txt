cmake_minimum_required(VERSION 3.20)
project(rubiks_cube_solver)

# Set C++11 standard to fix enum class warnings
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include all directories
include_directories(Model)

# List all source files
add_executable(rubiks_cube_solver
        Model/RubiksCube.cpp
        Model/RubiksCube.h
        Model/RubiksCube3dArray.cpp
        Model/RubiksCube3dArray.h
        Model/RubiksCube1dArray.cpp
        Model/RubiksCube1dArray.h
        main.cpp
        Model/RubiksCubeBitboard.h
        Model/RubiksCubeBitboard.cpp
        Solver/DFSSolver.h
        Solver/BFSSolver.h
        Solver/IDDFSSolver.h
        Model/PatternDatabase/PatternDatabase.h
        Solver/IDAstarSolver.h
        PatternDatabases/NibbleArray.cpp PatternDatabases/NibbleArray.h
        PatternDatabases/PatternDatabase.cpp PatternDatabases/PatternDatabase.h
)