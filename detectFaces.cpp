#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <iostream>

using namespace std;
using namespace cv;

void detectAndDisplay(Mat frame);
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;

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
    String image_name = samples::findFile(parser.get<String>("@image"));
    String cascade_name = samples::findFile(parser.get<String>("@cascade"));
    bool display_flag = parser.has("display");

    // Check for parsing errors
    if (!parser.check())
    {
        parser.printErrors();
        return 1;
    }

    // Print parameter values and exit
    cout << "image name: " << image_name << "\n";
    cout << "cascade name: " << cascade_name << "\n";
    if(display_flag)
        cout << "Display flag ON" << "\n";
    
    return 0;
