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
                             "{scale s|1.1|Scale factor specifies how much the image size is reduced at each image scale during detection.}"
                             "{neighbors n|3|Min. neighbors specifies how many neighbors each candidate rectangle should have to retain itself for the next stage.}"
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
    double scaleFactor = parser.get<double>("scale");
    int minNeighbors = parser.get<uint>("neighbors");

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
    std::vector<int> rejectLevels;
    std::vector<double> weights;
    cvtColor(image, image_gray, COLOR_BGR2GRAY);
    equalizeHist(image_gray, image_gray);
    // image, objects, rejectLevels, levelWeights, scaleFactor, minNeighbors, flags, minSize, maxSize, outputRejectLevels
    cascade.detectMultiScale(image_gray, faces, rejectLevels, weights, scaleFactor, minNeighbors, 0, Size(), Size(), true);
 
    // Generate output
    cout << image_path << "\n";
    cout << faces.size() << "\n";
    for (size_t i = 0; i < faces.size(); i++)
    {
        cout << faces[i].x << " " << faces[i].y << " " << faces[i].width << " " << faces[i].height << " " <<  weights[i] << "\n";
    }

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
