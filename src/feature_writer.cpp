//
// Created by Yuyang Tian on 2025/1/26.
//
#include <opencv2/opencv.hpp>
#include "../include/feature_calculate.h"
#include "../include/csv_util.h"
#include <vector>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <dirent.h>

/**
* @brief Extracts features from an image file and appends them to a CSV file.
*
* @param image_filename Path to the image file.
* @param feature_function Pointer to a function that extracts features from the image.
*                         The function takes an image filename and a reference to a
*                         vector of floats and returns 0 on success.
* @param output_filename Path to the output CSV file where features will be saved.
* @return int Returns 0 on success, or -1 on failure.
*/
int extract_and_save_features(char* image_filename,
                              int (*feature_function)(char *, std::vector<float> &),
                              char* output_filename) {
    std::vector<float> features;
    // Try to extract features
    int result = feature_function(image_filename, features);
    if (result != 0) {
        fprintf(stderr, "Error: Failed to extract features from '%s'\n", image_filename);
        return -1;
    }
    // If this is the first file, reset the output file
    static bool first_file = true;
    int reset = first_file ? 1 : 0;
    first_file = false;

    result = append_image_data_csv(output_filename, image_filename, features, reset);
    if (result != 0) {
        fprintf(stderr, "Error: Failed to save features to '%s'\n", output_filename);
        return -1;
    }

    return 0;
}


/**
 * @brief Main function to process a directory of image files and extract their features.
 *
 * Scans the given directory for image files and processes each file to extract features.
 * The extracted features are saved in the specified output CSV file.
 *
 * @param argc Number of command-line arguments.
 * @param argv Command-line arguments.
 *             argv[1] should be the directory path,
 *             argv[2] should be the output CSV file path.
 * @return int Returns 0 on success, or -1 on failure.
 */
int main(int argc, char *argv[]) {
    char dirname[256];
    char buffer[256];
    FILE *fp;
    DIR *dirp;
    struct dirent *dp;
    int i;

    // check for sufficient arguments
    if( argc < 3) {
        printf("usage: %s <directory path>\n", argv[0]);
        exit(-1);
    }

    // get the directory path
    strcpy(dirname, argv[1]);
    printf("Processing directory %s\n", dirname );

    // open the directory
    dirp = opendir( dirname );
    if( dirp == NULL) {
        printf("Cannot open directory %s\n", dirname);
        exit(-1);
    }
    // Validate the output file name
    char* output_file = argv[2];
    if (strlen(output_file) == 0) {
        fprintf(stderr, "Error: Output CSV file name is invalid.\n");
        return -1;
    }

    // loop over all the files in the image file listing
    while( (dp = readdir(dirp)) != NULL ) {

        // check if the file is an image
        if( strstr(dp->d_name, ".jpg") ||
            strstr(dp->d_name, ".png") ||
            strstr(dp->d_name, ".ppm") ||
            strstr(dp->d_name, ".tif") ) {

            printf("processing image file: %s\n", dp->d_name);

            // build the overall filename
            strcpy(buffer, dirname);
            strcat(buffer, "/");
            strcat(buffer, dp->d_name);

            printf("full path name: %s\n", buffer);
            extract_and_save_features(buffer, get7x7square, output_file);
        }
    }

    printf("Terminating\n");

    return(0);
}


