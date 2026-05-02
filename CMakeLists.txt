cmake_minimum_required(VERSION 3.20)
project(rubiks_cube_solver)

# Set C++11 standard to fix enum class warnings
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

# Include all directories
include_directories(Model)
include_directories(${OpenCV_INCLUDE_DIRS})

# List all source files
add_executable(rubiks_cube_solver
        Model/RubiksCube.cpp
        Model/RubiksCube.h
        Model/RubiksCube3dArray.cpp
        Model/RubiksCube3dArray.h
        Model/RubiksCube1dArray.cpp
        Model/RubiksCube1dArray.h
        CameraScanner.cpp
        CameraScanner.h
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
        PatternDatabases/PermutationIndexer.h
        PatternDatabases/CornerPatternDatabase.cpp PatternDatabases/CornerPatternDatabase.h
        PatternDatabases/PermutationIndexer.h PatternDatabases/CornerPatternDatabase.cpp PatternDatabases/CornerPatternDatabase.h PatternDatabases/CornerDBMaker.cpp PatternDatabases/CornerDBMaker.h
        PatternDatabases/math.cpp PatternDatabases/math.h
)

target_compile_definitions(rubiks_cube_solver PRIVATE HAVE_OPENCV)
target_link_libraries(rubiks_cube_solver ${OpenCV_LIBS})

add_executable(webcam_feed WebcamFeed.cpp)
target_link_libraries(webcam_feed ${OpenCV_LIBS})
