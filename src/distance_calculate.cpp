/*
 * Authors: Yuyang Tian and Arun Mekkad
 * Date: January 26, 2025
 * Purpose: Function prototypes for distance calculation
 */

#include "../include/distance_calculate.h"
#include <cmath>

using namespace std;

using namespace std;

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

// Function to calculate distance between two concatenated histograms
//  * @param hist1 First concatenated histogram.
//  * @param hist2 Second concatenated histogram.
//  * @return float Distance value.

float calculate_multiHist_distance(std::vector<float> &hist1, std::vector<float> &hist2) {
    // Split concatenated histograms
    size_t mid = hist1.size()/2;
    std::vector<float> top1(hist1.begin(), hist1.begin()+mid);
    std::vector<float> bottom1(hist1.begin()+mid, hist1.end());
    
    std::vector<float> top2(hist2.begin(), hist2.begin()+mid);
    std::vector<float> bottom2(hist2.begin()+mid, hist2.end());
    
    // Calculate individual distances
    float d_top = 1 - calculate_histogramIntersection(top1, top2);
    float d_bottom = 1 - calculate_histogramIntersection(bottom1, bottom2);
    
    return 0.5 * d_top + 0.5 * d_bottom; // Equal weighting
}

// Function to calculate distance between two texture-color histograms
//  * @param hist1 First texture-color histogram.
//  * @param hist2 Second texture-color histogram.
//  * @return float Distance value.

float calculate_textureColor_distance(std::vector<float>& hist1, std::vector<float>& hist2) {
    // Split into color (first 512 elements) and texture (remaining)
    size_t color_size = 8*8*8; // 512 elements for 8-bin RGB
    std::vector<float> color1(hist1.begin(), hist1.begin()+color_size);
    std::vector<float> color2(hist2.begin(), hist2.begin()+color_size);
    
    std::vector<float> tex1(hist1.begin()+color_size, hist1.end());
    std::vector<float> tex2(hist2.begin()+color_size, hist2.end());

    // Compute individual distances
    float d_color = 1 - calculate_histogramIntersection(color1, color2);
    float d_tex = 1 - calculate_histogramIntersection(tex1, tex2);

    // Combine with equal weights
    return 0.5f * d_color + 0.5f * d_tex;
}
