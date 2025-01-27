//
// Created by Yuyang Tian on 2025/1/26.
// CS 5330 Computer Vision
// Spring 2025
// CPP functions for calculating feature vector of an image

#ifndef PROJ2_FEATURE_CALCULATE_H
#define PROJ2_FEATURE_CALCULATE_H

#include <vector>
/*
  Given an image filename and a reference to a vector to store image features,
  calculate the feature vector for the image. It extracts a 7x7 square from
  the center of the image and flattens it into a vector.

  The function returns a non-zero value in case of an error (e.g., image load failure).
*/
int get7x7square(char *image_filename, std::vector<float> &image_data);
#endif //PROJ2_FEATURE_CALCULATE_H
