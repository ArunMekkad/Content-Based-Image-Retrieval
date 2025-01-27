//
// Created by Yuyang Tian on 2025/1/26.
// For task1 baseline matching, ssd is used for distance calculation
#include "../include/csv_util.h"
#include "../include/distance_calculate.h"
#include "../include/image_display_util.h"
#include <iostream>
#include <cstdlib> // for atoi
#include <cstdio>
#include <cstring>
#include <vector>


using namespace std;

/**
 * given a target image, the feature set, and the feature vector file.
 * It then computes the features for the target image, reads the feature vector file, and identifies the top N matches.
 * @param argc
 * @param argv
 * @return non-zero failure
 */
int find_topN_matches(char* target_image_filename, std::vector<char *> &filenames, std::vector<std::vector<float>> &data, int N, std::vector<char *> &output) {
    // data format is
    //  The image filename is written to the first position in the row of
    //  data. The values in image_data are all written to the file as
    //  floats.
    // Step1: find the target
    int target_index = -1;
    for (size_t i = 0; i < filenames.size(); i++) {
        if (strcmp(filenames[i], target_image_filename) == 0) {
            target_index = i;
            break;
        }
    }
    // If the target image is not found, return an error
    if (target_index == -1) {
        std::cerr << "Target image not found!" << std::endl;
        return -1;
    }

    // Step2: calculate the corresponding distance
    std::vector<float>& target_vector = data[target_index];
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
 * @return 0 on success, non-zero on failure.
 */
int main(int argc, char* argv[]) {
    // argv: target image, feature file, N
    char target_image[256];
    char feature_file[256];
    int N;

    // Step 1: check for sufficient arguments
    if(argc < 4) {
        printf("usage: %s <target_image> <feature_file> <N>\n", argv[0]);
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

    std::vector<char *> filenames;
    std::vector<std::vector<float>> data;
    int result = read_image_data_csv(feature_file, filenames, data);
    if (result != 0) {
        printf("Can not read the image csv file: %s\n", argv[2]);
        exit(-1);
    }

    // Step 5: process and sort the feature
    std::vector<char *> output;
    result = find_topN_matches(target_image, filenames, data, N, output); // Pass N here to find the top N matches
    if (result != 0) {
        printf("Can not process the files: ");
        for (const char* filename : filenames) {
            printf("%s ", filename);  // Print each file name in the vector
        }
        exit(-1);
    }
    // Print out the contents of the output vector
    std::cout << "Output filenames: ";
    for (const char* filename : output) {
        printf("%s ", filename);  // Print each file name in the vector
    }
    std::cout << std::endl;  // Move to the next line after printing
    displayGallery(output);
    return 0;
}
