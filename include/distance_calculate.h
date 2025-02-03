/*
 * Authors: Yuyang Tian and Arun Mekkad
 * Date: January 26, 2025
 * Purpose: Function prototypes for distance calculation
 */

#ifndef PROJ2_DISTANCE_CALCULATE_H
#define PROJ2_DISTANCE_CALCULATE_H
#include <vector>
/**
 * @brief Computes the SSD between two normalized feature vectors.
 *
 * @param v1 First normalized feature vectors.
 * @param v2 Second normalized feature vectors.
 * @return float SSD value.
 */
float calculate_ssd(std::vector<float>& v1, std::vector<float>& v2);
/**
 * @brief Computes the histogram intersection between two normalized histograms.
 *
 * @param hist1 First normalized histogram.
 * @param hist2 Second normalized histogram.
 * @return float Histogram intersection value.
 */
float calculate_histogramIntersection(std::vector<float>& hist1, std::vector<float>& hist2);

/**
 * @brief Computes the Cosine Distance between two feature vectors.
 *
 * @param vec1 First feature vector.
 * @param vec2 Second feature vector.
 * @return Cosine Distance in the range [0, 1], where 0 means identical vectors.
 */
float calculate_cosine_distance(std::vector<float>& vec1, std::vector<float>& vec2);


// Function to calculate distance between two concatenated histograms
//  * @param hist1 First concatenated histogram.
//  * @param hist2 Second concatenated histogram.
//  * @return float Distance value.
float calculate_multiHist_distance(std::vector<float> &hist1, std::vector<float> &hist2);

// Function to calculate distance between two texture-color histograms
//  * @param hist1 First texture-color histogram.
//  * @param hist2 Second texture-color histogram.
//  * @return float Distance value.
float calculate_textureColor_distance(std::vector<float>& hist1, std::vector<float>& hist2);

#endif //PROJ2_DISTANCE_CALCULATE_H

