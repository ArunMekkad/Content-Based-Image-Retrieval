// ImageDisplay.h
// Created by Yuyang Tian on 2025/1/27.
// Header file for displaying images given a vector of filenames

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
