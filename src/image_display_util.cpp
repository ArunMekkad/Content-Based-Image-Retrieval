/*
 * Authors: Yuyang Tian and Arun Mekkad
 * Date: January 26, 2025
 * Purpose: Display the image given a list of vectors
 */
#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
// Displays all images in a single gallery window
void displayGallery(const std::vector<char*>& filenames) {
    std::vector<cv::Mat> images;
    int total_width = 0, max_height = 0;

    // Load images
    for (const char* filename : filenames) {
        cv::Mat img = cv::imread(filename); // Read image
        if (img.empty()) {
            std::cerr << "Could not read: " << filename << std::endl;
            continue;
        }
        images.push_back(img);
        total_width += img.cols; // Accumulate total width
        max_height = std::max(max_height, img.rows); // Update max height
    }

    // Combine images into one canvas
    cv::Mat gallery(max_height, total_width, CV_8UC3, cv::Scalar(0, 0, 0));
    int x_offset = 0;
    for (const auto& img : images) {
        cv::Mat roi = gallery(cv::Rect(x_offset, 0, img.cols, img.rows));
        img.copyTo(roi);
        x_offset += img.cols;
    }

    // Display the gallery
    cv::imshow("Gallery", gallery);
    cv::waitKey(0); // Wait until a key is pressed
}
// Displays images one by one in a single window
void displayOneByOne(const std::vector<char*>& filenames) {
    for (const char* filename : filenames) {
        cv::Mat img = cv::imread(filename); // Read image
        if (img.empty()) {
            std::cerr << "Could not read: " << filename << std::endl;
            continue;
        }
        cv::imshow("Image", img);
        if (cv::waitKey(0) == 27) { // Wait for key press; break if 'Esc' is pressed
            break;
        }
    }
}