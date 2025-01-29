//
// Created by Yuyang Tian on 2025/1/26.
// CS 5330 Computer Vision
// Spring 2025
// CPP functions for calculating feature vector of an image

#ifndef PROJ2_FEATURE_CALCULATE_H
#define PROJ2_FEATURE_CALCULATE_H

#include <vector>
#include <opencv2/opencv.hpp>
// Feature extraction function type
typedef int (*FeatureFunction)(char*, std::vector<float>&);

// Different feature types
enum class FeatureType {
    SQUARE_7X7,
    RGB_HISTOGRAM,
  // TODO: Please dd more feature type here, Arun
    HSV_HISTOGRAM,
    // Add more feature types as needed
};

// Helper function to get feature function based on type
FeatureFunction getFeatureFunction(FeatureType type);

/*
  Given an image filename and a reference to a vector to store image features,
  calculate the feature vector for the image. It extracts a 7x7 square from
  the center of the image and flattens it into a vector.

  The function returns a non-zero value in case of an error (e.g., image load failure).
*/
int get7x7square(char *image_filename, std::vector<float> &image_data);
/**
 * @brief Calculates a 3D RGB color histogram for an image.
 *
 * @param image Input image filename.
 * @param hist A 1D Vector representing the flattened 3D histogram (bins x bins*bins).
 * @return non-zero failure.
 */
int calculateRGBHistogram(char *image_filename, std::vector<float>& hist);

#endif //PROJ2_FEATURE_CALCULATE_H
