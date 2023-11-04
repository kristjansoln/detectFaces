#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <fstream>

using namespace std;
using namespace cv;

void detectAndDisplay(Mat frame);
CascadeClassifier cascade;

int main(int argc, const char **argv)
{
    // Parse command line input
    CommandLineParser parser(argc, argv,
                             "{help h||Display this message and exit.}"
                             "{@inputFilePath|<none>|Path to an image or a txt file containing a list of images.}"
                             "{@cascade|cascades/haarcascade_frontalface_alt.xml|Path to face cascade. Defaults to frontalface_alt.xml}"
                             "{imageTopDir i|.|Top image directory, will be prepended to the provided image path. Do not add a trailing forward slash.}"
                             "{scale s|1.1|Scale factor specifies how much the image size is reduced at each image scale during detection.}"
                             "{neighbors n|3|Min. neighbors specifies how many neighbors each candidate rectangle should have to retain itself for the next stage.}"
                             "{display d||Display detections.}"
                             "{verbose v||Verbose output.}");
    parser.about("\nThis program detects faces on the provided image or list of images and outputs the results to standard output.\n");

    // If help argument is passed, print help and exit
    if (parser.has("help"))
    {
        parser.printMessage();
        return 0;
    }

    // Find specified parameters
    String inputFilePath = parser.get<String>("@inputFilePath");
    String cascadePath = samples::findFile(parser.get<String>("@cascade"));
    String imageTopDir = samples::findFile(parser.get<String>("imageTopDir"));
    bool displayFlag = parser.has("display");
    double scaleFactor = parser.get<double>("scale");
    int minNeighbors = parser.get<uint>("neighbors");

    // Check for parsing errors
    if (!parser.check())
    {
        parser.printErrors();
        return 1;
    }

    // Detect input file type
    vector<String> imageList; // used for outputting correct filenames
    vector<String> imagePathList;

    if (inputFilePath.length() >= 4 && inputFilePath.substr(inputFilePath.length() - 4) == ".txt")
    {
        // If the file ends in '.txt', parse it and prepare image paths for later processing
        ifstream inputFile(inputFilePath);
        if (!inputFile.is_open())
        {
            cerr << "Error: Unable to open the file.\n";
            return 1;
        }

        String line;
        String fwdslash("/");
        String extension(".jpg"); // TODO: This only works with JPGs now
        while (getline(inputFile, line))
        {
            imagePathList.push_back(imageTopDir + fwdslash + line + extension);
            imageList.push_back(line);
        }

        inputFile.close();
    }
    else
    {
        // Else assume the argument is an image file
        // Assume the provided inputFilePath is an entire path, eg. with file extension
        String fwdslash("/");
        imagePathList.push_back(imageTopDir + fwdslash + inputFilePath);
        imageList.push_back(inputFilePath);
    }

    if(parser.has("verbose"))
    {
        cout << "Using detector parameters:\n";
        cout << "inputFilePath: " << inputFilePath << "\n";
        cout << "cascadePath: " << cascadePath << "\n";
        cout << "imageTopDir: " << imageTopDir << "\n";
        cout << "displayFlag: " << displayFlag << "\n";
        cout << "scaleFactor: " << scaleFactor << "\n";
        cout << "minNeighbors: " << minNeighbors << "\n";
    }

    // Load specified cascade
    if (!cascade.load(cascadePath))
    {
        cerr << "Error loading face cascade:" << cascadePath << "\n";
        return -1;
    }

    // Process each image in the list
    for (int i = 0; i < imageList.size(); i++)
    {
        string imagePath = imagePathList.at(i);

        Mat image = imread(imagePath, IMREAD_COLOR);
        if (image.empty())
        {
            std::cerr << "Error reading the image: " << imagePath << "\n";
            return 1;
        }

        // Perform face detection
        Mat imageGray;
        std::vector<Rect> faces;
        std::vector<int> rejectLevels;
        std::vector<double> weights;
        cvtColor(image, imageGray, COLOR_BGR2GRAY);
        equalizeHist(imageGray, imageGray);
        // image, objects, rejectLevels, levelWeights, scaleFactor, minNeighbors, flags, minSize, maxSize, outputRejectLevels
        cascade.detectMultiScale(imageGray, faces, rejectLevels, weights, scaleFactor, minNeighbors, 0, Size(), Size(), true);

        // Generate output
        cout << imageList.at(i) << "\n";
        cout << faces.size() << "\n";
        for (size_t i = 0; i < faces.size(); i++)
        {
            cout << faces[i].x << " " << faces[i].y << " " << faces[i].width << " " << faces[i].height << " " << weights[i] << "\n";
        }

        // If requested, display image with detections
        if (displayFlag)
        {
            for (size_t i = 0; i < faces.size(); i++)
            {
                Point p1(faces[i].x, faces[i].y);
                Point p2(faces[i].x + faces[i].width, faces[i].y + faces[i].height);
                rectangle(image, p1, p2, Scalar(0, 255, 255), 1, LINE_4);
                Point p3(faces[i].x - 10, faces[i].y - 10);
                putText(image, to_string(weights[i]), p3, FONT_HERSHEY_SIMPLEX, 0.8, Scalar(0, 127, 255));
            }
            imshow("Face detection", image);
            waitKey();
        }
    }

    return 0;
}
