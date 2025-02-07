/*
 * Authors: Yuyang Tian and Arun Mekkad
 * Date: January 26, 2025
 * Purpose: Method definitions for feature extraction
 */

#include "../include/feature_calculate.h"
#include "../include/filters.h"
#include "../include/DA2Network.hpp"
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
        case FeatureType::TEXTURE_COLOR:
            return getTextureColorFeature;
        case FeatureType::DEPTH:
            return getTextureColorFeatureWithDepth;
        case FeatureType::BANANA:
            return getBananaFeature;
        default:
            return nullptr;
    }
}

static DA2Network& initializeDA2() {
    static DA2Network da_net("../include/model_fp16.onnx");  // Static: Created only once
    return da_net;  // Return reference to the same object
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
    int bins = 8;
    const int BIN_SIZE = 256 / bins;
    // Step 1: read the image
    Mat img = imread(image_filename);
    if (img.empty()) {
        cerr << "can not open image: " << image_filename << endl;
        return -1;
    }
    // Initiate the 3D histogram -> flatten 1D histogram for R, G, B bins
    hist.clear();
    hist.resize(bins * bins * bins, 0.0f);

    // Step 2: calculate the frequency of each pixel
    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            Vec3b color = img.at<Vec3b>(i, j);
            int binB = color[0] / BIN_SIZE;
            int binG = color[1] / BIN_SIZE;
            int binR = color[2] / BIN_SIZE;
            // mapping 3d index to the mat
            int binIndex = binR * bins * bins + binG * bins + binB;
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

// Function to calculate multi-histogram by splitting the image into two 
// halves, calculating histograms for each half and concatenating them

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

// Function to get multi-histogram feature

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

// Function to compute texture feature using Sobel gradients and histogram

int computeTextureFeature(const cv::Mat& image, std::vector<float>& tex_hist, int bins) {
    // Convert to grayscale
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

    // Compute Sobel gradients
    cv::Mat sobelX, sobelY;
    sobelX3x3(gray, sobelX);
    sobelY3x3(gray, sobelY);

    // Compute gradient magnitude
    cv::Mat gradient_mag;
    magnitude(sobelX, sobelY, gradient_mag);

    // Normalize to [0,255] and convert to 8-bit
    normalize(gradient_mag, gradient_mag, 0, 255, NORM_MINMAX);
    gradient_mag.convertTo(gradient_mag, CV_8U);

    // Initialize histogram
    tex_hist.clear();
    tex_hist.resize(bins, 0.0f);
    int bin_size = 256 / bins;

    // Compute histogram manually
    for (int i = 0; i < gradient_mag.rows; i++) {
        for (int j = 0; j < gradient_mag.cols; j++) {
            int bin_idx = gradient_mag.at<uchar>(i, j) / bin_size;
            tex_hist[bin_idx]++;
        }
    }

    // Normalize to [0,255] and convert to 8UC1
    cv::normalize(gradient_mag, gradient_mag, 0, 255, cv::NORM_MINMAX);
    gradient_mag.convertTo(gradient_mag, CV_8UC1);

    // Normalize histogram
    float total_pixels = static_cast<float>(gradient_mag.rows * gradient_mag.cols);
    for (float& bin : tex_hist) {
        bin /= total_pixels;
    }

    return 0;
}


// Function to get texture-color feature by combining color and texture histograms

int getTextureColorFeature(char* image_filename, std::vector<float>& feature) {
    int bins = 16;

    // Read image
    cv::Mat image = cv::imread(image_filename);
    if (image.empty()) return -1;

    // Get color histogram
    std::vector<float> color_hist;
    calculateRGBHistogram(image_filename, color_hist);
    // Get texture histogram
    std::vector<float> tex_hist;
    computeTextureFeature(image, tex_hist, bins);

    // Concatenate features: color first, then texture
    feature.clear();
    feature.insert(feature.end(), color_hist.begin(), color_hist.end());
    feature.insert(feature.end(), tex_hist.begin(), tex_hist.end());

    return 0;
}




void computeDepthMaskFromDA2(cv::Mat& src, cv::Mat& depth, cv::Mat& mask) {
    // Flatten depth values into a vector
    DA2Network& da2Network = initializeDA2();
    da2Network.set_input(src, 1);
    da2Network.run_network(depth, src.size());
    std::vector<float> depth_values;  // DA2 depth may use 16-bit
    for (int i = 0; i < depth.rows; i++) {
        for (int j = 0; j < depth.cols; j++) {
            depth_values.push_back(depth.at<float>(i, j));
        }
    }
    // Sort depth values
    std::sort(depth_values.begin(), depth_values.end());

    // Compute median depth (50th percentile)
    int median_index = depth_values.size() * 0.50;
    float median_value = depth_values[median_index];
//    // Define a depth range (50% close)
//    ushort min_depth = median_depth * 0.5;
//    ushort max_depth = median_depth * 1.5;

    // Create a binary mask: Keep pixels where depth is within this range
//    mask = (depth >= min_depth) & (depth <= max_depth);
    mask = (depth <= median_value);
}
int computeGradientMagnitude(cv::Mat& gray, cv::Mat& gradient_mag) {
    // Compute Sobel gradients using manual functions
    cv::Mat sobelX, sobelY;
    sobelX3x3(gray, sobelX);
    sobelY3x3(gray, sobelY);

    // Pre-allocate gradient magnitude matrix
    gradient_mag.create(gray.size(), CV_32F);

    // Compute gradient magnitude
    for (int i = 0; i < gray.rows; i++) {
        for (int j = 0; j < gray.cols; j++) {
            float x = static_cast<float>(sobelX.at<short>(i, j));
            float y = static_cast<float>(sobelY.at<short>(i, j));
            gradient_mag.at<float>(i, j) = std::sqrt(x * x + y * y);
        }
    }

    // Normalize to [0,1]
    cv::normalize(gradient_mag, gradient_mag, 0, 1, cv::NORM_MINMAX);
    return 0;
}
// Overloading Function to compute the RGB histogram for selected pixels
int calculateRGBHistogram(const cv::Mat& image, const cv::Mat& mask, std::vector<float>& hist, int bins) {
    const int BIN_SIZE = 256 / bins;

    hist.clear();
    hist.resize(bins * bins * bins, 0.0f);

    int valid_pixel_count = 0;

    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
            if (mask.at<uchar>(i, j) == 0) continue; // Ignore pixels not in depth range

            cv::Vec3b color = image.at<cv::Vec3b>(i, j);
            int binB = color[0] / BIN_SIZE;
            int binG = color[1] / BIN_SIZE;
            int binR = color[2] / BIN_SIZE;
            int binIndex = binR * bins * bins + binG * bins + binB;
            hist[binIndex]++;
            valid_pixel_count++;
        }
    }

    // Normalize the histogram
    if (valid_pixel_count > 0) {
        for (float &val : hist) val /= valid_pixel_count;
    }

    return 0;
}

// Overloading Function to compute texture histogram with depth filtering
int computeTextureFeature(cv::Mat& image, cv::Mat& mask, std::vector<float>& tex_hist, int bins) {
    // Convert to grayscale
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

    // Compute gradient magnitude
    cv::Mat gradient_mag;
    computeGradientMagnitude(gray, gradient_mag);

    // Initialize histogram
    tex_hist.assign(bins, 0.0f);
    int valid_pixel_count = 0;

    // Compute histogram using optimized iteration
    for (int i = 0; i < gray.rows; i++) {
        for (int j = 0; j < gray.cols; j++) {
            if (mask.at<uchar>(i, j) == 0) continue; // Ignore pixels outside depth threshold

            int bin_idx = static_cast<int>(gradient_mag.at<float>(i, j) * (bins - 1));
            tex_hist[bin_idx]++;
            valid_pixel_count++;
        }
    }
    // Normalize histogram
    if (valid_pixel_count > 0) {
        for (float &val : tex_hist) val /= valid_pixel_count;
    }

    return 0;
}
//Texture color with a mask based on depth closeness (50% range around median)
int getTextureColorFeatureWithDepth(char* image_filename, std::vector<float>& feature) {
    // Load RGB image
    cv::Mat image = cv::imread(image_filename);
    if (image.empty()) return -1;

    // Load DA2 depth map
    cv::Mat depth;
    // Compute mask based on depth closeness (50% range around median)
    cv::Mat mask;
    computeDepthMaskFromDA2(image, depth, mask);

    // Compute histograms only for valid pixels
    int bins = 8;
    std::vector<float> color_hist, tex_hist;
    calculateRGBHistogram(image, mask, color_hist, bins);
    computeTextureFeature(image, mask, tex_hist, bins);

    // Concatenate features
    feature.clear();
    feature.insert(feature.end(), color_hist.begin(), color_hist.end());
    feature.insert(feature.end(), tex_hist.begin(), tex_hist.end());

    return 0;
}

int getBananaFeature(char *image_filename, std::vector<float>& hist) {
    // Read and process image as before
    cv::Mat image = cv::imread(image_filename);
    if (image.empty()) {
        std::cerr << "Error loading image: " << image_filename << std::endl;
        return -1;
    }

    // HSV conversion and mask creation
    cv::Mat hsv, mask;
    cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);
    cv::Scalar lower_yellow(22, 150, 150);
    cv::Scalar upper_yellow(28, 255, 255);
    cv::inRange(hsv, lower_yellow, upper_yellow, mask);

    // Find connected components
    cv::Mat labels, stats, centroids;
    const int MIN_AREA = 2000;
    const int MAX_AREA = 10000;
    int nComponents = cv::connectedComponentsWithStats(mask, labels, stats, centroids);

    // Create 3D histogram: x-position (4 bins) × y-position (4 bins) × size (4 bins)
    const int SPATIAL_BINS = 4;  // bins for each spatial dimension
    const int SIZE_BINS = 4;     // bins for blob sizes
    const int TOTAL_BINS = SPATIAL_BINS * SPATIAL_BINS * SIZE_BINS;
    hist.assign(TOTAL_BINS, 0);

    float size_bin_width = (MAX_AREA - MIN_AREA) / static_cast<float>(SIZE_BINS);
    float x_bin_width = static_cast<float>(image.cols) / SPATIAL_BINS;
    float y_bin_width = static_cast<float>(image.rows) / SPATIAL_BINS;


    // Process each blob
    for (int i = 1; i < nComponents; i++) {
        int area = stats.at<int>(i, cv::CC_STAT_AREA);
        if (area >= MIN_AREA && area <= MAX_AREA) {
            // Get size bin
            int size_bin = std::min(static_cast<int>((area - MIN_AREA) / size_bin_width), SIZE_BINS - 1);

            // Scan through the entire image to find pixels belonging to this blob
            for (int y = 0; y < labels.rows; y++) {
                for (int x = 0; x < labels.cols; x++) {
                    // Check if this pixel belongs to the current blob
                    if (labels.at<int>(y, x) == i) {
                        // Calculate spatial bins for this pixel
                        int x_bin = std::min(static_cast<int>(x / x_bin_width), SPATIAL_BINS - 1);
                        int y_bin = std::min(static_cast<int>(y / y_bin_width), SPATIAL_BINS - 1);

                        // Calculate flat index
                        int flat_idx = x_bin + y_bin * SPATIAL_BINS + size_bin * SPATIAL_BINS * SPATIAL_BINS;
                        hist[flat_idx] += 1.0f;
                    }
                }
            }
        }
    }
    // Normalize histogram
    float total = 0;
    for (float count : hist) {
        total += count;
    }
    // Normalize histogram
    if (total > 0) {
        for (float& count : hist) {
            count /= total;
        }
    }
    hist.push_back(total);
    clog << "The valid total blobs are " << total << endl;
    return 0;
}