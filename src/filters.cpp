/*
 * Authors: Yuyang Tian and Arun Mekkad
 * Date: February 2, 2025
 * Purpose: Filter definitions
 */

#include <opencv2/opencv.hpp> // OpenCV library
#include <iostream>
#include "../include/filters.h"


using namespace cv;  // OpenCV namespace
using namespace std; // Standard C++ namespace

/*
 * The sobelX3x3() function applies a Sobel filter in the X direction to the src image and stores the result in dst.
 * It uses a Sobel kernel to calculate the new pixel values.
 * The new pixel values are calculated using the Sobel kernel and stored in a 16-bit signed image.
 */

int sobelX3x3(Mat &src, Mat &dst) {
    if (src.empty()) {
        return -1;
    }

    Mat temp;
    temp.create(src.size(), CV_16SC3); // Create temp with the same size as src but with 16-bit signed channels
    dst.create(src.size(), CV_16SC3); // Create dst with the same size as src but with 16-bit signed channels

    // Horizontal pass
    for (int i = 0; i < src.rows; i++) {
        for (int j = 1; j < src.cols - 1; j++) {
            for (int c = 0; c < 3; c++) {
                short sum = 
                    -1 * src.at<Vec3b>(i, j-1)[c] +
                     1 * src.at<Vec3b>(i, j+1)[c]; 
                temp.at<Vec3s>(i, j)[c] = sum;
            }
        }
    }

    // Vertical pass
    for (int i = 1; i < src.rows - 1; i++) {
        for (int j = 1; j < src.cols - 1; j++) {
            for (int c = 0; c < 3; c++) {
                short sum = 
                     1 * temp.at<Vec3s>(i-1, j)[c] +
                     2 * temp.at<Vec3s>(i, j)[c] +
                     1 * temp.at<Vec3s>(i+1, j)[c]; 
                dst.at<Vec3s>(i, j)[c] = sum;
            }
        }
    }

    return 0;
}


/*
 * The sobelY3x3() function applies a Sobel filter in the Y direction to the src image and stores the result in dst.
 * It uses a Sobel kernel to calculate the new pixel values.
 * The new pixel values are calculated using the Sobel kernel and stored in a 16-bit signed image.
 */

int sobelY3x3(Mat &src, Mat &dst) {
    
    if (src.empty()) {
        return -1;
    }

    Mat temp;
    
    temp.create(src.size(), CV_16SC3); // Create temp with the same size as src but with 16-bit signed channels
    dst.create(src.size(), CV_16SC3); // Create dst with the same size as src but with 16-bit signed channels

    // Horizontal pass
    for (int i = 1; i < src.rows - 1; i++) {
        for (int j = 0; j < src.cols; j++) {
            for (int c = 0; c < 3; c++) {
                short sum = 
                    1 * src.at<Vec3b>(i, j-1)[c] +
                    2 * src.at<Vec3b>(i, j)[c] +
                    1 * src.at<Vec3b>(i, j+1)[c]; 
                temp.at<Vec3s>(i, j)[c] = sum;
            }
        }
    }

    // Vertical pass

    for (int i = 1; i < src.rows - 1; i++) {
        for (int j = 1; j < src.cols - 1; j++) {
            for (int c = 0; c < 3; c++) {
                short sum = 
                    -1 * temp.at<Vec3s>(i-1, j)[c] +
                     1 * temp.at<Vec3s>(i+1, j)[c]; 
                dst.at<Vec3s>(i, j)[c] = sum;
            }
        }
    }

    return 0;
}

int magnitude(Mat &sx, Mat &sy, Mat &dst) {

    // Create a destination image of type CV_8UC3 for display
    dst.create(sx.size(), CV_8UC3);

    /*Loops through each pixel in the input images (s_x and s_y) row by row and column  
    by column where i and j represnts row and column respectively.
    */
    for (int i = 0; i < sx.rows; i++) {
        for (int j = 0; j < sx.cols; j++) {
            Vec3s pixel_sx = sx.at<Vec3s>(i, j);
            Vec3s pixel_sy = sy.at<Vec3s>(i, j);
            Vec3b pixel_dst;

            /*
            Loops through each of the color channels. It will apply Euclidean distance and take 
            square root from together added s_x squared and s_y squared for each c 0, 1 and 2.
            */
            for (int c = 0; c < 3; c++) {
                float magnitude = sqrt(pow(pixel_sx[c], 2) + pow(pixel_sy[c], 2));
                pixel_dst[c] = saturate_cast<uchar>(magnitude); // Clamp values to [0, 255]
            }

            dst.at<Vec3b>(i, j) = pixel_dst;
        }
    }

    return 0;
}




