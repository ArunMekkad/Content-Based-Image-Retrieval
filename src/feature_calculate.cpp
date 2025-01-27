//
// Created by Yuyang Tian on 2025/1/26.
// CS 5330 Computer Vision
// Spring 2025
// CPP functions for calculating feature vector of an image

//ARUN IS HERE

#include "../include/feature_calculate.h"
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int get7x7square(char *image_filename, std::vector<float> &image_data) {
    // Step 1: read the image
    Mat image = imread(image_filename);
    // check
    if (image.empty()) {
        cerr <<  "Error loading image!" << endl;
        return -1; // Return non-zero in case of error
    }
    // Step 2: calculate the center
    int center_x = image.cols / 2;
    int center_y = image.rows / 2;

    // Step 3: Define the size of the square (7x7)
    int square_size = 7;
    int half_size = square_size / 2;

    // Step 4: Get the cropped image
    Rect roi(center_x - half_size, center_y - half_size, half_size, half_size);
    Mat square = image(roi);

    // Step 5: Get the feature vector
    for (int i = 0; i < square.rows; i++) {
        for (int j = 0; j < square.cols; j++) {
            Vec3b pixel = square.at<Vec3b>(i,j);
            // Convert the pixel to float and store in the image_data vector
            for (int c = 0; c < 3; c++) {
                image_data.push_back(static_cast<float>(pixel[c]));
            }
        }
    }

    return 0;
}
