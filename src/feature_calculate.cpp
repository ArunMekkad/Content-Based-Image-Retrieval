//
// Created by Yuyang Tian on 2025/1/26.
// CS 5330 Computer Vision
// Spring 2025
// CPP functions for calculating feature vector of an image

#include "../include/feature_calculate.h"
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

// Helper function to get the appropriate feature function
FeatureFunction getFeatureFunction(FeatureType type) {
    switch (type) {
        case FeatureType::SQUARE_7X7:
            return get7x7square;
        case FeatureType::RGB_HISTOGRAM:
            return calculateRGBHistogram;
        case FeatureType::MULTI_HISTOGRAM:
            return getMultiHistogramFeature;
        default:
            return nullptr;
    }
}

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
/**
 * @brief Calculates a 3D RGB color histogram for an image.
 *
 * @param image Input image filename.
 * @param hist A 1D Vector representing the flattened 3D histogram (bins x bins*bins).
 * @return non-zero failure.
 */
int calculateRGBHistogram(char *image_filename, std::vector<float>& hist) {
    const int BINs = 8; // 8 bins for each channel
    const int BIN_SIZE = 256 / BINs;
    // Step 1: read the image
    Mat img = imread(image_filename);
    if (img.empty()) {
        cerr << "can not open image: " << image_filename << endl;
        return -1;
    }
    // Initiate the 3D histogram -> flatten 1D histogram for R, G, B bins
    hist.clear();
    hist.resize(BINs * BINs * BINs, 0.0f);

    // Step 2: calculate the frequency of each pixel
    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            Vec3b color = img.at<Vec3b>(i, j);
            int binB = color[0] / BIN_SIZE;
            int binG = color[1] / BIN_SIZE;
            int binR = color[2] / BIN_SIZE;
            // mapping 3d index to the mat
            int binIndex = binR * BINs * BINs + binG * BINs + binB;
            hist[binIndex]++;
        }
    }
    // Step 3: Normalize the frequency
    float total_pixels = static_cast<float>(img.rows * img.cols);
    for (int i = 0; i < hist.size(); i++) {
        hist[i] /= total_pixels;
    }
    // Success!
    return 0;
}

int calculateMultiHistogram(const cv::Mat& image, std::vector<float>& hist, int bins) {
    const int BIN_SIZE = 256 / bins;
    
    // Initialize the histogram
    hist.clear();
    hist.resize(bins * bins * bins, 0.0f);

    // Calculate the frequency of each pixel
    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
            cv::Vec3b color = image.at<cv::Vec3b>(i, j);
            int binB = color[0] / BIN_SIZE;
            int binG = color[1] / BIN_SIZE;
            int binR = color[2] / BIN_SIZE;
            int binIndex = binR * bins * bins + binG * bins + binB;
            hist[binIndex]++;
        }
    }

    // Normalize the frequency
    float total_pixels = static_cast<float>(image.rows * image.cols);
    for (float& bin : hist) {
        bin /= total_pixels;
    }

    return 0;
}

int getMultiHistogramFeature(char *image_filename, std::vector<float> &image_data) {
    int bins = 8;
    // Read the image
    cv::Mat image = cv::imread(image_filename);
    if (image.empty()) {
        std::cerr << "Error loading image: " << image_filename << std::endl;
        return -1;
    }
    
    // Split image into top/bottom halves
    cv::Mat top_half = image(cv::Rect(0, 0, image.cols, image.rows/2));
    cv::Mat bottom_half = image(cv::Rect(0, image.rows/2, image.cols, image.rows/2));
    
    // Calculate histograms for each region
    std::vector<float> top_hist, bottom_hist;
    calculateMultiHistogram(top_half, top_hist, bins);
    calculateMultiHistogram(bottom_half, bottom_hist, bins);
    
    // Concatenate histograms
    image_data.clear();
    image_data.insert(image_data.end(), top_hist.begin(), top_hist.end());
    image_data.insert(image_data.end(), bottom_hist.begin(), bottom_hist.end());
    
    return 0;
}
