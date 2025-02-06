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


/**
 * @brief Computes the Cosine Distance between two feature vectors.
 *
 * @param vec1 First feature vector.
 * @param vec2 Second feature vector.
 * @return Cosine Distance in the range [0, 1], where 0 means identical vectors.
 */
float calculate_cosine_distance(std::vector<float>& vec1, std::vector<float>& vec2) {
    // Check if vectors are valid (same size and non-empty)
    if (vec1.size() != vec2.size() || vec1.empty()) {
        return 1.0f;  // Return maximum distance (completely dissimilar) if invalid
    }

    float dotProduct = 0.0f; // Sum of element-wise multiplication
    float norm1 = 0.0f;
    float norm2 = 0.0f;

    // Compute dot product and norms (L2 norm squared)
    for (size_t i = 0; i < vec1.size(); i++) {
        dotProduct += vec1[i] * vec2[i]; // a · b
        norm1 += vec1[i] * vec1[i];      // ||a||^2
        norm2 += vec2[i] * vec2[i];      // ||b||^2
    }

    // Avoid division by zero
    if (norm1 == 0.0f || norm2 == 0.0f) {
        return 1.0f;  // Return max distance if either vector is zero
    }

    // Compute cosine similarity: cos(θ) = (a · b) / (||a|| * ||b||)
    float cosineSimilarity = dotProduct / (std::sqrt(norm1) * std::sqrt(norm2));

    // Cosine Distance = 1 - Cosine Similarity
    return 1.0f - cosineSimilarity;
}


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

    // Dynamically determine split point (assuming equal weight for color and texture)
    size_t split_index = hist1.size() / 2;

    std::vector<float> color1(hist1.begin(), hist1.begin() + split_index);
    std::vector<float> color2(hist2.begin(), hist2.begin() + split_index);
    
    std::vector<float> tex1(hist1.begin() + split_index, hist1.end());
    std::vector<float> tex2(hist2.begin() + split_index, hist2.end());

    // Compute individual distances
    float d_color = 1 - calculate_histogramIntersection(color1, color2);
    float d_tex = 1 - calculate_histogramIntersection(tex1, tex2);

    // Combine with equal weights
    return 0.5f * d_color + 0.5f * d_tex;
}

