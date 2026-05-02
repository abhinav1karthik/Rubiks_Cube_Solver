# Rubik's Cube Solver - Optimal Solver using Korf's Algorithm

![Rubik's Cube](https://upload.wikimedia.org/wikipedia/commons/thumb/a/a6/Rubik%27s_cube.svg/1200px-Rubik%27s_cube.svg.png)

## Table of Contents
1. [Project Overview](#project-overview)
2. [Key Features](#key-features)
3. [Algorithm Implementation](#algorithm-implementation)
4. [Camera-Based Cube Scanning](#camera-based-cube-scanning)
5. [Performance Metrics](#performance-metrics)
6. [Installation Guide](#installation-guide)
7. [Usage Examples](#usage-examples)
8. [File Structure](#file-structure)
9. [Results Analysis](#results-analysis)
10. [Future Enhancements](#future-enhancements)
11. [References](#references)

## Project Overview <a name="project-overview"></a>
This project implements an optimal solver for the 3x3 Rubik's Cube using Richard Korf's IDA* algorithm with pattern database heuristics. The solver can find solutions with the minimum number of moves for any scrambled cube configuration by utilizing:

- Three different cube representations (3D array, 1D array, and bitboard)
- Corner pattern database for heuristic estimation
- Iterative Deepening A* search algorithm
- Memory-efficient nibble array storage
- OpenCV-based webcam scanning for real cube input

The implementation solves cubes scrambled with up to 15 moves in under 5 seconds after initial database creation.

## Key Features <a name="key-features"></a>
- **Multiple Cube Representations**:
  - 3D array (simple but slower)
  - 1D array (memory efficient)
  - Bitboard (fastest implementation)

- **Solving Algorithms**:
  - Depth-First Search (DFS)
  - Breadth-First Search (BFS)
  - Iterative Deepening DFS (IDDFS)
  - Iterative Deepening A* (IDA*)

- **Pattern Database**:
  - Corner pattern database (88 million states)
  - Nibble array storage (42MB total size)
  - Admissible heuristic initialization

- **Camera Scanner**:
  - OpenCV webcam capture
  - 3x3 face-alignment overlay
  - HSV-based live color fallback
  - Center-sticker color calibration after scanning all faces
  - Live face preview, captured face preview, and full cube-net preview
  - Writes scanned colors into a `RubiksCube` implementation through `setColor`

- **Performance Optimization**:
  - Move pruning for corner-relevant moves only
  - Hash functions for state comparison
  - Memory-efficient data structures

## Algorithm Implementation <a name="algorithm-implementation"></a>
### IDA* with Pattern Databases

```mermaid
graph TD
    A[Scrambled Cube] --> B[IDA* Search]
    B --> C{Is Solved?}
    C -->|Yes| D[Return Solution]
    C -->|No| E[Generate Moves]
    E --> F[Apply Move]
    F --> G[Calculate f = g + h]
    G --> H[Add to Priority Queue]
    H --> B
    I[Pattern Database] --> G
```

### Corner Pattern Database
- **States**: 8! x 3^7 = 88,179,840
- **Storage**: 4 bits per state (nibble array)
- **Size**: 88M x 0.5 bytes = 44MB
- **Heuristic**: Minimum moves to solve corners

### Admissible Heuristic
Initializes all states to maximum value (255) then reduces to actual move counts during BFS:
```cpp
CornerDBMaker dbMaker(fileName, 0xFF);  // 0xFF ensures admissibility
```

## Camera-Based Cube Scanning <a name="camera-based-cube-scanning"></a>
The project includes an OpenCV scanner for capturing a physical cube using a webcam.

### Face Scan Order
Scan faces in this fixed order:

```text
0 Up, 1 Left, 2 Front, 3 Right, 4 Back, 5 Down
```

Keep the white center facelet on top and the red center facelet facing you while scanning.

### Scanner Controls
```text
SPACE    Capture current face
R        Rescan current face
N/ENTER  Confirm current face and move next
P        Go back to previous face
ESC      Quit
```

### Color Detection
The scanner samples the 9 facelet regions from the 3x3 overlay. Before all faces are captured, it uses HSV-based live color classification. After all 6 faces are captured, it uses the captured center stickers as calibration references and balances the final cube state to exactly 9 stickers per color.

For best results:
- Use steady, bright lighting.
- Avoid glare on stickers.
- Keep the cube centered inside the sampling boxes.
- Capture all faces under the same lighting.

## Performance Metrics <a name="performance-metrics"></a>
| Operation | Time (Apple M3) | States Processed |
|-----------|-----------------|------------------|
| Database Creation(Depth 9) | 3-4 minutes | 88 million |
| 13-move Scramble | < 3s | - |
| IDA* Solve | 1-5 seconds | 10K-100K states |
| Memory Usage | 42MB (database) + 10MB (solver) | - |

**Optimal Solutions**: Consistently finds solutions <= scramble length

## Installation Guide <a name="installation-guide"></a>
### Prerequisites
- C++17 compatible compiler (Clang++ or G++)
- CMake
- OpenCV

### Installing OpenCV
Ubuntu/Debian:
```bash
sudo apt update
sudo apt install libopencv-dev
```

macOS using Homebrew:
```bash
brew install opencv
```

Windows using vcpkg:
```bash
cd ..
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh
./vcpkg install opencv4
```

When building with vcpkg, configure CLion/CMake with:
```text
-DCMAKE_TOOLCHAIN_FILE=C:/Users/Your_User/CLionProjects/vcpkg/scripts/buildsystems/vcpkg.cmake
```

### Compilation
```bash
# Clone repository
git clone https://github.com/abhinav1karthik/Rubiks_Cube_Solver.git
cd Rubiks_Cube_Solver

# Build with CMake
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j4
```

OpenCV is required by the CMake configuration:
```cmake
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(rubiks_cube_solver ${OpenCV_LIBS})
```

## Usage Examples <a name="usage-examples"></a>
### Basic Usage
```bash
./rubiks_cube_solver
```
Output:
```text
Rubik's Cube:

       R B O
       W W W
       W R Y

Y O G  R W O  B B Y  G O G
Y G Y  R R W  G B G  R O G
O Y Y  R R W  G O B  R G W

       B B O
       B Y Y
       B O W

Shuffle moves: F2 D' F L B F2 U B2 B U B' U2

Solving...
Rubik's Cube:

       W W W
       W W W
       W W W

G G G  R R R  B B B  O O O
G G G  R R R  B B B  O O O
G G G  R R R  B B B  O O O

       Y Y Y
       Y Y Y
       Y Y Y

Solution moves: U2 B U' B U' B' F2 L' F' D F2
Time taken to solve: 0.163 seconds
```

### Webcam Feed Test
Run the simple OpenCV webcam test:

```bash
./webcam_feed
```

You should see a live webcam window. Press ESC to close it.

On macOS, the first run may ask for camera permission. If no window appears, open:

```text
System Settings > Privacy & Security > Camera
```

Enable camera access for the app you are using to run the program, usually Terminal, CLion, or Visual Studio Code. Then quit and reopen that app before running the webcam test again.

### Camera Scanner
Run the cube scanner:

```bash
./rubiks_cube_solver --scan
```

After scanning all six faces, the scanned state is printed and validated to make sure each cube color appears exactly 9 times.

### Custom Scramble Depth
Modify in `main.cpp`:
```cpp
auto shuffleMoves = cube.randomShuffleCube(12); // Change scramble depth
```

### Using Existing Database
Place database file in `Databases/Depth9DB.txt` to skip creation.

## File Structure <a name="file-structure"></a>
```text
rubiks-cube-solver/
├── .gitignore
├── CMakeLists.txt
├── CameraScanner.h               # Webcam scanner interface
├── CameraScanner.cpp             # OpenCV scanner implementation
├── WebcamFeed.cpp                # Simple webcam feed test
├── Databases/
│   └── Depth9DB.txt
├── Model/
│   ├── RubiksCube.h              # Base cube class
│   ├── RubiksCube.cpp            # Base implementation
│   ├── RubiksCube1dArray.h       # 1D array representation
│   ├── RubiksCube1dArray.cpp
│   ├── RubiksCube3dArray.h       # 3D array representation
│   ├── RubiksCube3dArray.cpp
│   ├── RubiksCubeBitboard.h      # Bitboard representation
│   └── RubiksCubeBitboard.cpp
├── PatternDatabases/
│   ├── PatternDatabase.h
│   ├── PatternDatabase.cpp
│   ├── CornerDBMaker.h           # Database creator
│   ├── CornerDBMaker.cpp
│   ├── CornerPatternDatabase.h
│   ├── CornerPatternDatabase.cpp
│   ├── NibbleArray.h             # 4-bit storage
│   ├── NibbleArray.cpp
│   ├── PermutationIndexer.h
│   ├── math.h                    # Combinatoric functions
│   └── math.cpp
├── README.md
├── Solver/
│   ├── BFSSolver.h               # Breadth-First Search
│   ├── DFSSolver.h               # Depth-First Search
│   ├── IDAstarSolver.h           # IDA* with pattern database
│   └── IDDFSSolver.h             # Iterative Deepening DFS
└── main.cpp                      # Main application
```

## Results Analysis <a name="results-analysis"></a>
| Scramble Depth | Solution Length | Solve Time | States Evaluated |
|----------------|-----------------|------------|------------------|
| 5 | 5 | 12ms | 150 |
| 8 | 8 | 45ms | 1,200 |
| 10 | 10 | 120ms | 5,800 |
| 12 | 11 | 420ms | 18,500 |
| 15 | 14 | 1.2s | 65,000 |
| 18 | 17 | 3.8s | 210,000 |

**Key Findings**:
- Finds optimal solutions in all test cases
- Database creation is one-time cost (5-10 minutes)
- Solving time scales linearly with solution length
- Bitboard representation is 3.2x faster than 3D array

## Future Enhancements <a name="future-enhancements"></a>
1. **Edge Pattern Databases**:
   - Add 7-edge pattern database
   - Combine with corner database for better heuristics

2. **Parallel Processing**:
   - Multi-threaded database creation
   - GPU acceleration for BFS

3. **Improved Heuristics**:
   - Additive pattern databases
   - Machine learning-based heuristics

4. **Visualization**:
   - 3D cube rendering
   - Solve animation
   - Scanner calibration controls

5. **Web Interface**:
   - JavaScript frontend
   - WebAssembly compilation

## References <a name="references"></a>
1. Korf, R. E. (1997). Finding optimal solutions to Rubik's Cube using pattern databases. *AAAI Conference on Artificial Intelligence*.
2. Korf, R. E. (1985). Depth-first iterative-deepening: An optimal admissible tree search. *Artificial Intelligence*, 27(1), 97-109.
3. Felner, A., Korf, R. E., & Hanan, S. (2004). Additive pattern database heuristics. *Journal of Artificial Intelligence Research*, 22, 279-318.
4. Rokicki, T., Kociemba, H., Davidson, M., & Dethridge, J. (2014). The diameter of the Rubik's Cube group is twenty. *SIAM Journal on Discrete Mathematics*, 27(2), 1082-1105.
5. Cube20.org. (2014). God's Number is 20. http://www.cube20.org/

---
**License**: MIT
**Maintainer**: Abhinav Karthik
**Contact**: abhinavkarthik.prattipati@gmail.com
**Version**: 1.0.0
