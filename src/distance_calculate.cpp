//
// Created by Yuyang Tian on 2025/1/26.
//

#include "../include/distance_calculate.h"
#include <cmath>

typedef float (*DistanceFunction)(std::vector<float>&,std::vector<float>&);
DistanceFunction select_distance_function(char* metric) {
    if (metric == "ssd") {
        return calculate_ssd;
    } else { // add more function here
        throw std::invalid_argument("Unknown distance metric: " + metric);
    }
}

float calculate_ssd(std::vector<float>& v1, std::vector<float>& v2) {
    float distance = 0.0f;
    for (int i = 0; i < v1.size(); i++) {
        float diff = v1[i] - v2[i];
        distance += diff * diff;
    }
    return std::sqrt(distance);
}