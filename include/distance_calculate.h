//
// Created by Yuyang Tian on 2025/1/26.
// Function prototypes for different distance metrics

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
#endif //PROJ2_DISTANCE_CALCULATE_H
