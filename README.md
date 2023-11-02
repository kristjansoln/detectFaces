# Face Detection using OpenCV Cascade Classifier

This repository performs face detection on images using OpenCV's Cascade Classifier.

`detectFaces.cpp` detects faces in images or a list of images and outputs the results to standard output. It uses the Haar Cascade Classifier for face detection. `runFDDB.sh` uses `detectFaces` to process multiple lists of images. The script processes configured image lists and replaces the contents of the `fddb_evaluation_docker/detections` directory with new results. Then, it runs the docker container, which then generates ROC curves.

`detectFaces` can be used on its own to perform face detection on images or lists of images. In the latter case, it expects the list to be in the format as mentioned in the container install guide. Run `detectFaces -h` for details on how to use the binary. Note: the only necessary parameter is `inputFilePath`, others can be ommited as they have a default value.

## Usage

The OpenCV C++ library must be installed. This was tested using version `4.8.0-dev`.

1. Clone the repository. Make sure you also clone the `fddb_evaluation_docker` submodule.
```sh
git clone https://github.com/kristjansoln/detectFaces.git --recurse-submodules 
```

2. Compile the program:

```sh
cmake .
make
```

3. Modify the `runEval.sh` script in order to select desired detector parameters. If needed, also modify the directories containing images, image lists and where to store results. These settings can be found at the beginning of the `runEval.sh` script. 

4. Configure the `fddb_evaluation_docker` submodule, as described in its documentation. Make sure to download the originalPics and FDDB-folds.

5. Run the script by running `./runEval.sh`. The script processes configured image lists and replaces the contents of the `fddb_evaluation_docker/detections` directory with new results. Then


