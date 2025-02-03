/*
 * Authors: Yuyang Tian and Arun Mekkad
 * Date: January 26, 2025
 * Purpose: Image display header file
 */

#ifndef IMAGE_DISPLAY_H
#define IMAGE_DISPLAY_H

#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>

// Displays all images in a single gallery window
void displayGallery(const std::vector<char*>& filenames);

// Displays images one by one in a single window
void displayOneByOne(const std::vector<char*>& filenames);

#endif // IMAGE_DISPLAY_H
