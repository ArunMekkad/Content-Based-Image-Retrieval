/*
 * Authors: Yuyang Tian and Arun Mekkad
 * Date: January 26, 2025
 * Purpose: Find and display the top N matching images based on feature vectors
 */
#include "../include/csv_util.h"
#include "../include/distance_calculate.h"
#include "../include/image_display_util.h"
#include <iostream>
#include <cstdlib> // for atoi
#include <cstdio>
#include <cstring>
#include <vector>

using namespace cv;
using namespace std;

int find_target_index_cosine(const char *target_image_filename, std::vector<char *> &filenames) 
{  
      
    int target_index = -1;    
    for (size_t i = 0; i < filenames.size(); i++) 
    {        
        string dirname = "../olympus/";
        size_t len = dirname.length() + strlen(filenames[i]) + 1;
        char *fullpath = new char[len];
        strcpy(fullpath, dirname.c_str());
        strcat(fullpath, filenames[i]);

        if (strcmp(fullpath, target_image_filename) == 0)
        {      
            target_index = i;            
        break;        
        }    
    }    
        return target_index;
}



int find_target_index(const char *target_image_filename, vector<char *> &filenames) {
    int target_index = -1;
    for (size_t i = 0; i < filenames.size(); i++) {
        if (strcmp(filenames[i], target_image_filename) == 0) {
            target_index = i;
            break;
        }
    }
    return target_index;
}

/**
 * Function to find top N matches using SSD distance
 * @return non-zero failure
 */
int find_topN_matches_ssd(char *target_image_filename, std::vector<char *> &filenames,
                          std::vector<std::vector<float>> &data, int N, std::vector<char *> &output) {
    // data format is
    //  The image filename is written to the first position in the row of data.
    //  The values in image_data are all written to the file as floats.
    // Step1: find the target
    int target_index = find_target_index(target_image_filename, filenames);
    // If the target image is not found, return an error
    if (target_index == -1) {
        std::cerr << "Target image not found!" << std::endl;
        return -1;
    }

    // Step2: calculate the corresponding distance
    std::vector<float> &target_vector = data[target_index];
    vector<pair<float, int>> distances; // Pair of distance and index

    for (size_t i = 0; i < data.size(); i++) {
        if (i == target_index) {
            continue;
        }
        float dist = calculate_ssd(data[i], target_vector);
        distances.push_back({dist, static_cast<int>(i)});
    }

    // Step 3: Sort the pair
    sort(distances.begin(), distances.end());

    // Step 4: get N of them and return
    for (int i = 0; i < N && i < distances.size(); i++) {
        int match_index = distances[i].second;
        output.push_back(filenames[match_index]);
    }
    return 0;
}

/**
 * Function to find top N matches using RGB histogram intersection
 * @return non-zero failure
 */
int find_topN_matches_hist(char *target_image_filename, std::vector<char *> &filenames,
                               std::vector<std::vector<float>> &data, int N, std::vector<char *> &output) {
    // Step1: find the target
    int target_index = find_target_index(target_image_filename, filenames);
    // If the target image is not found, return an error
    if (target_index == -1) {
        std::cerr << "Target image not found!" << std::endl;
        return -1;
    }

    // Step2: calculate the corresponding distance
    std::vector<float> &target_vector = data[target_index];
    vector<pair<float, int>> distances; // Pair of distance and index

    for (size_t i = 0; i < data.size(); i++) {
        if (i == target_index) {
            continue;
        }
        float dist = calculate_histogramIntersection(data[i], target_vector);
        distances.push_back({dist, static_cast<int>(i)});
    }

    // Step 3: Sort the pair,
    sort(distances.rbegin(), distances.rend());

    // Step 4: get N of them and return
    for (int i = 0; i < N && i < distances.size(); i++) {
        int match_index = distances[i].second;
        output.push_back(filenames[match_index]);
    }
    return 0;
}

// Function to find top N matches using multi histogram distance

int find_topN_matches_multiHist(char *target_image_filename, std::vector<char *> &filenames,
                                std::vector<std::vector<float>> &data, int N, std::vector<char *> &output) {
    // Step1: find the target
    int target_index = find_target_index(target_image_filename, filenames);
    // If the target image is not found, return an error
    if (target_index == -1) {
        std::cerr << "Target image not found!" << std::endl;
        return -1;
    }

    // Step2: calculate the corresponding distance
    std::vector<float> &target_vector = data[target_index];
    vector<pair<float, int>> distances; // Pair of distance and index

    for (size_t i = 0; i < data.size(); i++) {
        if (i == target_index) {
            continue;
        }
        float dist = calculate_multiHist_distance(data[i], target_vector);
        distances.push_back({dist, static_cast<int>(i)});
    }

    // Step 3: Sort the pair,
    sort(distances.begin(), distances.end());
    // Step 4: get N of them and return
    for (int i = 0; i < N && i < distances.size(); i++) {
        int match_index = distances[i].second;
        output.push_back(filenames[match_index]);
    }
    return 0;
}

/**
 * Function to find top N matches using texture color distance
 */
int find_topN_matches_textureColor(char* target_image_filename, std::vector<char*>& filenames,
                                   std::vector<std::vector<float>>& data, int N, std::vector<char*>& output) 
{
    int target_index = find_target_index(target_image_filename, filenames);
    if (target_index == -1) return -1;

    std::vector<float> &target = data[target_index];
    std::vector<std::pair<float, int>> distances;

    for (size_t i = 0; i < data.size(); i++) {
        if (i == target_index) continue;
        float dist = calculate_textureColor_distance(data[i], target);
        distances.push_back({dist, static_cast<int>(i)});
    }

    // Sort by ascending order
    std::sort(distances.begin(), distances.end());

    // Clear output vector before inserting new values
    output.clear();
    for (int i = 0; i < N && i < distances.size(); i++) {
        output.push_back(filenames[distances[i].second]);
    }
    return 0;
}


int find_topN_matches_cosine(char* target_image_filename, std::vector<char *> &filenames,
                             std::vector<std::vector<float>> &data, int N,std::vector<char *> &output) 
{
    int target_index = find_target_index_cosine(target_image_filename, filenames);
    if (target_index == -1) return -1;

    std::vector<float> target = data[target_index];
    // l2_norm(target);

    std::vector<std::pair<float, int>> distances;

    for(size_t i = 0; i < data.size(); i++) {
        if(i == target_index) continue;
        std::vector<float> vec = data[i];
        // l2_norm(vec);
        float dist = calculate_cosine_distance(vec, target);
        distances.push_back({dist, static_cast<int>(i)});
    }

    std::sort(distances.begin(), distances.end());
    
    output.clear();
    for(int i = 0; i < N && i < distances.size(); i++) {
        output.push_back(filenames[distances[i].second]);
    }

    return 0;
}
int find_topN_matches_depthDNN(char* target_image_filename, std::vector<char *> &filenames,
                             std::vector<std::vector<float>> &data, std::vector<std::vector<float>> &rnnData , int N,std::vector<char *> &output) {

    int target_index = find_target_index_cosine(target_image_filename, filenames);
    if (target_index == -1) return -1;
    if (rnnData.size() != data.size()) {
        cerr << "RNN data and data size is not the same! \n";
        cerr << "rnn size is" << rnnData.size() << " And data size is " << data.size();
    }
    std::vector<float> targetTexColor = data[target_index];
    std::vector<float> targetRNN = rnnData[target_index];
    // l2_norm(target);

    std::vector<std::pair<float, int>> distances;

    for(size_t i = 0; i < rnnData.size(); i++) {
        if(i == target_index) continue;
        std::vector<float> rnn = rnnData[i];
        std::vector<float> vec = data[i];
        // l2_norm(vec);
        float dist1 = calculate_cosine_distance(rnn, targetRNN) * 0.8;
        float dist2 = calculate_textureColor_distance(vec, targetTexColor) * 0.2;
//        clog << "dist1-rnn is " << dist1 << ", dist2-texture-color is " << dist2 << endl;
        distances.push_back({dist1 + dist2, static_cast<int>(i)});
    }

    std::sort(distances.begin(), distances.end());

    output.clear();
    for(int i = 0; i < N && i < distances.size(); i++) {
        output.push_back(filenames[distances[i].second]);
    }

    return 0;
}

int find_topN_matches_banana(char* target_image_filename, std::vector<char *> &filenames,
                               std::vector<std::vector<float>> &data, std::vector<std::vector<float>> &rnnData , int N,std::vector<char *> &output) {

    int target_index = find_target_index_cosine(target_image_filename, filenames);
    if (target_index == -1) return -1;
    if (rnnData.size() != data.size()) {
        cerr << "RNN data and data size is not the same! \n";
        cerr << "rnn size is" << rnnData.size() << " And data size is " << data.size();
    }
    std::vector<float> target = data[target_index];
    std::vector<float> targetRNN = rnnData[target_index];

    std::vector<std::pair<float, int>> distances;
    int col = data[0].size();
    // 0.5 blob histogram intersection + 0.5 rnn
    for(size_t i = 0; i < rnnData.size(); i++) {
        if(i == target_index || data[i][col-1] == 0) continue;
        std::vector<float> rnn = rnnData[i];
        std::vector<float> vec = data[i];
        // l2_norm(vec);
        float dist1 = calculate_cosine_distance(rnn, targetRNN) * 0.5;
        float dist2 = calculate_histogramIntersection(vec, target) * 0.5;
//        clog << "dist1-rnn is " << dist1 << ", dist2-texture-color is " << dist2 << endl;
        distances.push_back({dist1 + dist2, static_cast<int>(i)});
    }

    std::sort(distances.begin(), distances.end());

    output.clear();
    for(int i = 0; i < N && i < distances.size(); i++) {
        output.push_back(filenames[distances[i].second]);
    }

    return 0;
}
/**
 * Main function that finds and displays the top N matching images based on feature vectors.
 *
 * This function processes the command line arguments to extract the target image file path,
 * feature file path, and the number N of top matches to find. It reads the image feature
 * data from a CSV file and uses the `find_topN_matches` function to compute the top N
 * matching images. It then displays the filenames of the matching images and renders them.
 *
 * @param argc The number of command-line arguments.
 * @param argv The command-line arguments. It expects:
 *             argv[1] - Target image filename
 *             argv[2] - Feature file filename
 *             argv[3] - Integer N representing the number of top matches to find
 *             argv[4] - Distance_metric representing the matching method
 * @return 0 on success, non-zero on failure.
 */
int main(int argc, char *argv[]) {
    char target_image[256];
    char feature_file[256];
    int N;
    std::string distance_metric;

    // Step 1: check for sufficient arguments
    if (argc < 5) {
        printf("usage: %s <target_image> <feature_file> <N> <distance_metric>\n", argv[0]);
        printf("distance_metric options: ssd, rgb-hist, multi-hist, texture-color, cosine, depth, gabor\n");
        exit(-1);
    }

    // Step 2: get the target_image path
    strcpy(target_image, argv[1]);
    printf("Find similar images for image %s ", target_image);

    // Step 3: read from the feature file
    strcpy(feature_file, argv[2]);
    printf("from feature file %s\n", feature_file);

    // Step 4: Read N
    N = atoi(argv[3]);
    if (N <= 0) {
        printf("Invalid value for N: %d. N must be a positive integer.\n", N);
        exit(-1);
    }

    // Step 5: Get distance metric
    distance_metric = argv[4];
    // TODO: Add other metrics here
    if (distance_metric != "ssd" && distance_metric != "rgb-hist" && distance_metric != "multi-hist" && distance_metric != "texture-color" && distance_metric != "cosine" && distance_metric != "depth" && distance_metric != "banana" && distance_metric != "gabor" ) {
        printf("Invalid distance metric: %s. Must be 'ssd', 'intersection', 'multi-hist' , 'texture-color', 'cosine' or 'depth' or 'banana' or 'gabor'\n", argv[4]);
        exit(-1);
    }
    printf("Using distance metric: %s\n", distance_metric.c_str());

    std::vector<char *> filenames;
    std::vector<std::vector<float>> data;
    int result = read_image_data_csv(feature_file, filenames, data);

    if (result != 0) {
        printf("Can not read the image csv file: %s\n", argv[2]);
        exit(-1);
    }
    // Step 6: process and sort the feature
    std::vector<char *> output;
    std::vector<char *> cosine_output;
    result = -1;
    // TODO: Add other metrics here
    if (distance_metric == "ssd") {
        result = find_topN_matches_ssd(target_image, filenames, data, N, output);
    } else if (distance_metric == "rgb-hist") {
        result = find_topN_matches_hist(target_image, filenames, data, N, output);
    } else if (distance_metric == "multi-hist") {
        result = find_topN_matches_multiHist(target_image, filenames, data, N, output);
    } else if (distance_metric == "texture-color") {
        result = find_topN_matches_textureColor(target_image, filenames, data, N, output);
    } else if (distance_metric == "depth") { // texture-color with a depth mask
        std::vector<std::vector<float>> RNNdata;
        result = read_image_data_csv("../olympus/ResNet18_olym.csv", filenames, RNNdata);
        if (result != 0) {
            cerr << "Can not read the RNN image csv file: %s\n";
            exit(-1);
        }
        result = find_topN_matches_depthDNN(target_image, filenames, data, RNNdata,N, output);
    } else if (distance_metric == "cosine") {
        result = find_topN_matches_cosine(target_image, filenames, data, N, output);
    } else if (distance_metric == "banana") { // just use ssd
        std::vector<std::vector<float>> RNNdata;
        result = read_image_data_csv("../olympus/ResNet18_olym.csv", filenames, RNNdata);
        if (result != 0) {
            cerr << "Can not read the RNN image csv file: %s\n";
            exit(-1);
        }
        result = find_topN_matches_banana(target_image, filenames, data, RNNdata,N, output);
    }

    // Step 7: verify the output
    if (result != 0) {
        printf("Can not process the files: ");
        exit(-1);
    }

    std::cout << "Output filenames: ";
    for (const char* filename : output) {
        if(distance_metric == "cosine" || distance_metric == "depth" || distance_metric == "banana")
        {
            std::string fullpath = "../olympus/" + std::string(filename);
            cosine_output.push_back(strdup(fullpath.c_str()));  // Add the path to the image since only name is provided in ResNet18.csv
        }
        printf("%s ", filename);
    }
    std::cout << std::endl;
    // Display target and result image
    cv::Mat target = cv::imread(target_image);
    if (target.empty()) {
        printf("Target image empty!\n");
        exit(-1);
    }
    cv::imshow("target", target);
    if(distance_metric == "cosine" || distance_metric == "depth" || distance_metric == "banana")
    {
        displayGallery(cosine_output); //Display for cosine since different format for image directory
    }
    else
    displayGallery(output);
    return 0;
}
