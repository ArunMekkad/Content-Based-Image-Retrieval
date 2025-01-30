//
// Created by Yuyang Tian on 2025/1/26.
//

#include "../include/distance_calculate.h"
#include <cmath>

/**
 * @brief Computes the SSD between two normalized feature vectors.
 *
 * @param v1 First normalized feature vectors.
 * @param v2 Second normalized feature vectors.
 * @return float SSD value.
 */
float calculate_ssd(std::vector<float>& v1, std::vector<float>& v2) {
    float distance = 0.0f;
    for (int i = 0; i < v1.size(); i++) {
        float diff = v1[i] - v2[i];
        distance += diff * diff;
    }
    return std::sqrt(distance);
}
/**
 * @brief Computes the histogram intersection between two normalized histograms.
 *
 * @param hist1 First normalized histogram.
 * @param hist2 Second normalized histogram.
 * @return float Histogram intersection value.
 */
float calculate_histogramIntersection(std::vector<float>& hist1, std::vector<float>& hist2) {
    float intersection = 0.0f;

    // Ensure histograms are of same size
    if (hist1.size() != hist2.size()) {
        return 0.0f;  // Return 0 for no intersection if sizes differ
    }

    // Calculate histogram intersection
    for (size_t i = 0; i < hist1.size(); i++) {
        intersection += std::min(hist1[i], hist2[i]);
    }

    // Since the histograms are normalized, intersection will be between 0 and 1
    // where 1 means identical histograms and 0 means no overlap
    return intersection;
}