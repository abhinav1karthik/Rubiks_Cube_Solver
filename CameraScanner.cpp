#include "CameraScanner.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#ifdef HAVE_OPENCV
#include <opencv2/opencv.hpp>

namespace {
typedef RubiksCube::COLOR Color;
typedef RubiksCube::FACE Face;

const char *faceNames[6] = {"Up", "Left", "Front", "Right", "Back", "Down"};
const char *faceTips[6] = {
        "White center",
        "Green center",
        "Red center",
        "Blue center",
        "Orange center",
        "Yellow center"
};

cv::Scalar bgrForColor(Color color) {
    switch (color) {
        case Color::WHITE: return cv::Scalar(245, 245, 245);
        case Color::GREEN: return cv::Scalar(40, 170, 40);
        case Color::RED: return cv::Scalar(30, 30, 220);
        case Color::BLUE: return cv::Scalar(220, 90, 30);
        case Color::ORANGE: return cv::Scalar(20, 130, 255);
        case Color::YELLOW: return cv::Scalar(40, 230, 240);
        default: return cv::Scalar(80, 80, 80);
    }
}

Color classifyHsv(const cv::Scalar &hsv) {
    double h = hsv[0];
    double s = hsv[1];
    double v = hsv[2];

    if (v > 145 && s < 70) return Color::WHITE;
    if (h < 4 || h >= 170) return Color::RED;
    if (h < 24) return Color::ORANGE;
    if (h < 42) return Color::YELLOW;
    if (h < 85) return Color::GREEN;
    return Color::BLUE;
}

Color classifyHsvFromBgr(const cv::Vec3d &bgr) {
    cv::Mat bgrMat(1, 1, CV_8UC3, cv::Scalar(bgr[0], bgr[1], bgr[2]));
    cv::Mat hsvMat;
    cv::cvtColor(bgrMat, hsvMat, cv::COLOR_BGR2HSV);
    cv::Vec3b hsv = hsvMat.at<cv::Vec3b>(0, 0);
    return classifyHsv(cv::Scalar(hsv[0], hsv[1], hsv[2]));
}

cv::Vec3d bgrToLab(const cv::Vec3d &bgr) {
    cv::Mat bgrMat(1, 1, CV_32FC3, cv::Scalar(bgr[0] / 255.0, bgr[1] / 255.0, bgr[2] / 255.0));
    cv::Mat labMat;
    cv::cvtColor(bgrMat, labMat, cv::COLOR_BGR2Lab);
    cv::Vec3f lab = labMat.at<cv::Vec3f>(0, 0);
    return cv::Vec3d(lab[0], lab[1], lab[2]);
}

double labDistance(const cv::Vec3d &a, const cv::Vec3d &b) {
    double dl = a[0] - b[0];
    double da = a[1] - b[1];
    double db = a[2] - b[2];
    return std::sqrt(dl * dl + da * da + db * db);
}

int capturedFaceCount(bool hasFace[6]) {
    int count = 0;
    for (int face = 0; face < 6; ++face) {
        if (hasFace[face]) ++count;
    }
    return count;
}

Color classifyCalibrated(const cv::Vec3d &bgr, cv::Vec3d samples[6][3][3], bool hasFace[6]) {
    if (capturedFaceCount(hasFace) < 6) {
        return classifyHsvFromBgr(bgr);
    }

    cv::Vec3d target = bgrToLab(bgr);
    double bestDistance = 1e9;
    Color bestColor = classifyHsvFromBgr(bgr);

    for (int face = 0; face < 6; ++face) {
        if (!hasFace[face]) continue;
        cv::Vec3d center = bgrToLab(samples[face][1][1]);
        double distance = labDistance(target, center);
        if (distance < bestDistance) {
            bestDistance = distance;
            bestColor = static_cast<Color>(face);
        }
    }

    return bestColor;
}

cv::Vec3d sampleFaceletBgr(const cv::Mat &frame, int x, int y) {
    int half = 14;
    int left = std::max(0, x - half);
    int top = std::max(0, y - half);
    int right = std::min(frame.cols - 1, x + half);
    int bottom = std::min(frame.rows - 1, y + half);
    cv::Rect sampleRect(left, top, right - left + 1, bottom - top + 1);

    cv::Scalar mean = cv::mean(frame(sampleRect));
    return cv::Vec3d(mean[0], mean[1], mean[2]);
}

void drawFaceGrid(cv::Mat &image, int x, int y, int cell, Color face[3][3], bool filled) {
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            cv::Rect rect(x + col * cell, y + row * cell, cell, cell);
            cv::rectangle(image, rect, filled ? bgrForColor(face[row][col]) : cv::Scalar(235, 235, 235), cv::FILLED);
            cv::rectangle(image, rect, cv::Scalar(45, 45, 45), 1);
            if (filled) {
                std::string letter(1, RubiksCube::getColorLetter(face[row][col]));
                cv::putText(image, letter, cv::Point(rect.x + cell / 2 - 6, rect.y + cell / 2 + 7),
                            cv::FONT_HERSHEY_SIMPLEX, 0.55, cv::Scalar(20, 20, 20), 2);
            }
        }
    }
}

void drawProgress(cv::Mat &image, bool hasFace[6], int currentFace) {
    int x = 24;
    int y = 690;
    for (int face = 0; face < 6; ++face) {
        cv::Scalar fill = hasFace[face] ? bgrForColor(static_cast<Color>(face)) : cv::Scalar(225, 225, 225);
        cv::Rect rect(x + face * 105, y, 92, 34);
        cv::rectangle(image, rect, fill, cv::FILLED);
        cv::rectangle(image, rect, face == currentFace ? cv::Scalar(20, 20, 20) : cv::Scalar(120, 120, 120), face == currentFace ? 3 : 1);
        cv::putText(image, faceNames[face], cv::Point(rect.x + 8, rect.y + 23),
                    cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(20, 20, 20), 1);
    }
}

void drawCubeNet(cv::Mat &image, Color scanned[6][3][3], bool hasFace[6], int x, int y, int cell) {
    const int netPositions[6][2] = {
            {1, 0},
            {0, 1},
            {1, 1},
            {2, 1},
            {3, 1},
            {1, 2}
    };

    for (int face = 0; face < 6; ++face) {
        int fx = x + netPositions[face][0] * cell * 3;
        int fy = y + netPositions[face][1] * cell * 3;
        drawFaceGrid(image, fx, fy, cell, scanned[face], hasFace[face]);
    }
}

void sampleCurrentFace(const cv::Mat &frame, int gridX, int gridY, int cell, cv::Vec3d out[3][3]) {
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            int x = gridX + col * cell + cell / 2;
            int y = gridY + row * cell + cell / 2;
            out[row][col] = sampleFaceletBgr(frame, x, y);
        }
    }
}

void classifyFace(cv::Vec3d samples[3][3], cv::Vec3d allSamples[6][3][3], bool hasFace[6], Color out[3][3]) {
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            out[row][col] = classifyCalibrated(samples[row][col], allSamples, hasFace);
        }
    }
}

void classifyAllFaces(cv::Vec3d samples[6][3][3], bool hasFace[6], Color out[6][3][3]) {
    for (int face = 0; face < 6; ++face) {
        for (int row = 0; row < 3; ++row) {
            for (int col = 0; col < 3; ++col) {
                out[face][row][col] = classifyCalibrated(samples[face][row][col], samples, hasFace);
            }
        }
        out[face][1][1] = static_cast<Color>(face);
    }
}

void classifyAllFacesBalanced(cv::Vec3d samples[6][3][3], bool hasFace[6], Color out[6][3][3]) {
    if (capturedFaceCount(hasFace) < 6) {
        classifyAllFaces(samples, hasFace, out);
        return;
    }

    struct Candidate {
        double distance;
        int face;
        int row;
        int col;
        int color;
    };

    std::vector<Candidate> candidates;
    int counts[6] = {0, 0, 0, 0, 0, 0};
    for (int color = 0; color < 6; ++color) {
        out[color][1][1] = static_cast<Color>(color);
        counts[color] = 1;
    }

    cv::Vec3d centers[6];
    for (int color = 0; color < 6; ++color) {
        centers[color] = bgrToLab(samples[color][1][1]);
    }

    for (int face = 0; face < 6; ++face) {
        for (int row = 0; row < 3; ++row) {
            for (int col = 0; col < 3; ++col) {
                if (row == 1 && col == 1) continue;
                cv::Vec3d target = bgrToLab(samples[face][row][col]);
                for (int color = 0; color < 6; ++color) {
                    candidates.push_back({labDistance(target, centers[color]), face, row, col, color});
                }
            }
        }
    }

    std::sort(candidates.begin(), candidates.end(), [](const Candidate &a, const Candidate &b) {
        return a.distance < b.distance;
    });

    bool assigned[6][3][3] = {};
    for (int face = 0; face < 6; ++face) {
        assigned[face][1][1] = true;
    }

    for (const Candidate &candidate : candidates) {
        if (assigned[candidate.face][candidate.row][candidate.col]) continue;
        if (counts[candidate.color] >= 9) continue;

        out[candidate.face][candidate.row][candidate.col] = static_cast<Color>(candidate.color);
        assigned[candidate.face][candidate.row][candidate.col] = true;
        counts[candidate.color]++;
    }
}

void drawSamplingOverlay(cv::Mat &canvas, int gridX, int gridY, int cell, Color liveFace[3][3]) {
    int gridSize = cell * 3;
    cv::rectangle(canvas, cv::Rect(gridX, gridY, gridSize, gridSize), cv::Scalar(255, 255, 255), 3);
    for (int i = 1; i < 3; ++i) {
        cv::line(canvas, cv::Point(gridX + i * cell, gridY),
                 cv::Point(gridX + i * cell, gridY + gridSize), cv::Scalar(255, 255, 255), 2);
        cv::line(canvas, cv::Point(gridX, gridY + i * cell),
                 cv::Point(gridX + gridSize, gridY + i * cell), cv::Scalar(255, 255, 255), 2);
    }

    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            int x = gridX + col * cell + cell / 2;
            int y = gridY + row * cell + cell / 2;
            cv::rectangle(canvas, cv::Rect(x - 16, y - 16, 32, 32), cv::Scalar(25, 25, 25), 2);
            cv::circle(canvas, cv::Point(x, y), 9, bgrForColor(liveFace[row][col]), cv::FILLED);
            cv::circle(canvas, cv::Point(x, y), 9, cv::Scalar(20, 20, 20), 1);
        }
    }
}

void writeToCube(RubiksCube &cube, Color scanned[6][3][3]) {
    for (int face = 0; face < 6; ++face) {
        for (int row = 0; row < 3; ++row) {
            for (int col = 0; col < 3; ++col) {
                cube.setColor(static_cast<Face>(face), row, col, scanned[face][row][col]);
            }
        }
    }
}
}

bool captureCubeState(RubiksCube &cube, int cameraIndex) {
#ifdef __APPLE__
    cv::VideoCapture camera(cameraIndex, cv::CAP_AVFOUNDATION);
#else
    cv::VideoCapture camera(cameraIndex);
#endif
    if (!camera.isOpened()) {
        std::cerr << "Could not open webcam at index " << cameraIndex << ".\n";
#ifdef __APPLE__
        std::cerr << "On macOS, allow camera access for the app running this program "
                     "in System Settings > Privacy & Security > Camera.\n";
#endif
        return false;
    }

    cv::Vec3d sampleBgr[6][3][3];
    Color scanned[6][3][3];
    bool hasFace[6] = {false, false, false, false, false, false};
    for (int face = 0; face < 6; ++face) {
        for (int row = 0; row < 3; ++row) {
            for (int col = 0; col < 3; ++col) {
                cv::Scalar defaultBgr = bgrForColor(static_cast<Color>(face));
                sampleBgr[face][row][col] = cv::Vec3d(defaultBgr[0], defaultBgr[1], defaultBgr[2]);
                scanned[face][row][col] = static_cast<Color>(face);
            }
        }
    }

    int currentFace = 0;
    const int windowWidth = 1180;
    const int windowHeight = 760;
    cv::namedWindow("Rubik Cube Scanner", cv::WINDOW_AUTOSIZE);

    while (currentFace < 6) {
        cv::Mat frame;
        camera >> frame;
        if (frame.empty()) {
            std::cerr << "Camera returned an empty frame.\n";
            return false;
        }

        cv::resize(frame, frame, cv::Size(720, 540));
        cv::Mat canvas(windowHeight, windowWidth, CV_8UC3, cv::Scalar(250, 250, 250));
        frame.copyTo(canvas(cv::Rect(20, 90, frame.cols, frame.rows)));

        int gridSize = 300;
        int cell = gridSize / 3;
        int gridX = 20 + (frame.cols - gridSize) / 2;
        int gridY = 90 + (frame.rows - gridSize) / 2;

        cv::Vec3d liveSamples[3][3];
        Color liveFace[3][3];
        sampleCurrentFace(canvas(cv::Rect(20, 90, frame.cols, frame.rows)),
                          gridX - 20, gridY - 90, cell, liveSamples);
        classifyFace(liveSamples, sampleBgr, hasFace, liveFace);
        liveFace[1][1] = static_cast<Color>(currentFace);

        Color displayFaces[6][3][3];
        classifyAllFacesBalanced(sampleBgr, hasFace, displayFaces);
        drawSamplingOverlay(canvas, gridX, gridY, cell, liveFace);

        cv::rectangle(canvas, cv::Rect(20, 18, 720, 56), cv::Scalar(242, 242, 242), cv::FILLED);
        cv::putText(canvas, "Scan " + std::to_string(currentFace + 1) + "/6: " + std::string(faceNames[currentFace]) +
                            " face - " + faceTips[currentFace],
                    cv::Point(34, 50), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(20, 20, 20), 2);
        cv::putText(canvas, "Align the 9 stickers inside the boxes. Centers calibrate color under your lighting.",
                    cv::Point(24, 660), cv::FONT_HERSHEY_SIMPLEX, 0.55, cv::Scalar(35, 35, 35), 1);
        cv::putText(canvas, "SPACE capture   ENTER/N next   R rescan   P previous   ESC quit",
                    cv::Point(24, 682), cv::FONT_HERSHEY_SIMPLEX, 0.55, cv::Scalar(35, 35, 35), 1);
        drawProgress(canvas, hasFace, currentFace);

        cv::putText(canvas, "Live detection", cv::Point(800, 74), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(25, 25, 25), 2);
        drawFaceGrid(canvas, 800, 94, 54, liveFace, true);

        cv::putText(canvas, "Captured face", cv::Point(800, 300), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(25, 25, 25), 2);
        drawFaceGrid(canvas, 800, 320, 54, displayFaces[currentFace], hasFace[currentFace]);

        cv::putText(canvas, "Cube net", cv::Point(780, 535), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(25, 25, 25), 2);
        drawCubeNet(canvas, displayFaces, hasFace, 780, 555, 27);

        cv::imshow("Rubik Cube Scanner", canvas);
        int key = cv::waitKey(20);
        if (key == 27) {
            return false;
        }
        if (key == ' ' || key == 32) {
            for (int row = 0; row < 3; ++row) {
                for (int col = 0; col < 3; ++col) {
                    sampleBgr[currentFace][row][col] = liveSamples[row][col];
                }
            }
            hasFace[currentFace] = true;
            classifyAllFacesBalanced(sampleBgr, hasFace, scanned);
        } else if (key == 'r' || key == 'R') {
            hasFace[currentFace] = false;
        } else if ((key == 'p' || key == 'P') && currentFace > 0) {
            --currentFace;
        } else if ((key == 'n' || key == 'N' || key == 13 || key == 10) && hasFace[currentFace]) {
            ++currentFace;
        }
    }

    classifyAllFacesBalanced(sampleBgr, hasFace, scanned);
    writeToCube(cube, scanned);
    cv::destroyWindow("Rubik Cube Scanner");
    return true;
}

#else

bool captureCubeState(RubiksCube &, int) {
    std::cerr << "OpenCV support is not enabled. Install OpenCV and configure with CMake so find_package(OpenCV) succeeds.\n";
    return false;
}

#endif
