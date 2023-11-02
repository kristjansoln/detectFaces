#!/bin/bash

# Run FDDB evaluation of the cascade face detector.

# Directories
# Note: Don't add trailing '/'
list_directory="fddb_evaluation_docker/FDDB-folds"
image_directory="fddb_evaluation_docker/originalPics"
results_directory="fddb_evaluation_docker/detections"
# Detector parameters
cascade="cascades/haarcascade_frontalface_default.xml"
nNeighbors="3"
scale="1.1"

# Loop through the matching files and process them
for list_file in $list_directory/FDDB-fold-??.txt; do
    
    echo "Processing $list_file"

    # Generate results filename from the list filename
    results_file="${list_file#fddb_evaluation_docker/FDDB-folds/FDDB-}"
    results_file="${results_file%.txt}-out.txt"

    # Clear the results file if it already exists
    > "$results_directory/$results_file"
    if [ $? -ne 0 ]; then
      echo "Error: failed to create/clear the results file: "$results_directory/$results_file
      exit 1
    fi

    # Process the list file
    ./detectFaces -i=$image_directory -n=$nNeighbors -s=$scale $list_file $cascade >> "$results_directory/$results_file"
    if [ $? -ne 0 ]; then
        echo "Error: failed to process file: "$list_file
        exit 1
    fi

done

# Run the container for performance evaluation

cd fddb_evaluation_docker

export FDDB_HOME=`pwd`
docker run --rm -it -v ${FDDB_HOME}:/FDDB housebw/fddb-evaluator

if [ $? -ne 0 ]; then
    echo "Error: failed to run the container"
    exit 1
fi

cd ..

exit 0
