#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <iostream>

using namespace std;
using namespace cv;

void detectAndDisplay(Mat frame);
CascadeClassifier cascade;

int main(int argc, const char **argv)
{
    // Parse command line input
    CommandLineParser parser(argc, argv,
                             "{help h||Display this message and exit.}"
                             "{@image|<none>|Path to image file.}"
                             "{@cascade|cascades/haarcascade_frontalface_alt.xml|Path to face cascade. Defaults to frontalface_alt.xml}"
                             "{display d||Display detections.}");
    parser.about("\nThis program detects faces on the provided image and outputs the results to standard output.");

    // If help argument is passed, print help and exit
    if (parser.has("help"))
    {
        parser.printMessage();
        return 0;
    }

    // Find specified parameters
    String image_path = samples::findFile(parser.get<String>("@image"));
    String cascade_path = samples::findFile(parser.get<String>("@cascade"));
    bool display_flag = parser.has("display");

    // Check for parsing errors
    if (!parser.check())
    {
        parser.printErrors();
        return 1;
    }

    // Load specified cascade and image
    if (!cascade.load(cascade_path))
    {
        cerr << "Error loading face cascade:" << cascade_path << "\n";
        return -1;
    }

    Mat image = imread(image_path, IMREAD_COLOR);
    if (image.empty())
    {
        std::cerr << "Error reading the image: " << image_path << "\n";
        return 1;
    }

    // Perform face detection
    Mat image_gray;
    std::vector<Rect> faces;
    cvtColor(image, image_gray, COLOR_BGR2GRAY);
    equalizeHist(image_gray, image_gray);
    cascade.detectMultiScale(image_gray, faces);
 
    // TODO: Generate output for each face
    for (size_t i = 0; i < faces.size(); i++)
    {
        Point p1(faces[i].x, faces[i].y);
        Point p2(faces[i].x + faces[i].width, faces[i].y + faces[i].height);
    }

    // TODO: Generate output

    // If requested, display image with detections
    if (display_flag)
    {
        for (size_t i = 0; i < faces.size(); i++)
        {
            Point p1(faces[i].x, faces[i].y);
            Point p2(faces[i].x + faces[i].width, faces[i].y + faces[i].height);
            rectangle(image, p1, p2, Scalar(0, 255, 255), 1, LINE_4);
        }
        imshow("Face detection", image);
        waitKey();
    }

    return 0;
}
