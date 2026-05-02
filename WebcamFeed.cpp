#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
#ifdef __APPLE__
    VideoCapture cap(0, CAP_AVFOUNDATION);
#else
    VideoCapture cap(0);
#endif

    if (!cap.isOpened()) {
        cerr << "Error: Could not open camera.\n";
#ifdef __APPLE__
        cerr << "On macOS, allow camera access for the app running this program "
                "in System Settings > Privacy & Security > Camera.\n";
#endif
        return -1;
    }

    cout << "Camera opened. Press ESC in the webcam window to close it.\n";

    Mat frame;

    while (true) {
        cap >> frame;

        if (frame.empty()) break;

        imshow("Webcam Feed", frame);

        if (waitKey(30) == 27) break;
    }

    cap.release();
    destroyAllWindows();

    return 0;
}
