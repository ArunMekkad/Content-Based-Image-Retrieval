/*
 * Authors: Yuyang Tian and Arun Mekkad
 * Date: February 2, 2025
 * Purpose: Filters header file
 */
#ifndef FILTERS_H
#define FILTERS_H

#include <opencv2/opencv.hpp>

int sobelX3x3(cv::Mat &src, cv::Mat &dst);
int sobelY3x3(cv::Mat &src, cv::Mat &dst); 
int magnitude(cv::Mat &sx, cv::Mat &sy, cv::Mat &dst);

#endif
