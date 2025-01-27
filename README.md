# Project 2

## Team Members
1. **Yuyang Tian**
2. **Arun Mekkad** *(Arun, please add your environment details below.)*

## Environment
- **Yuyang Tian**: macOS 10.13.1 + CLion + CMake
- **Arun**: *(Add details here.)*

## Instructions for Running Executables

### File Structure
  ```
  Proj2/
     ├── include/              # Header files
     ├── src/                  # Source files - most of them are executables.
     ├── data/                 # output CSV or test data files
     ├── olympus/              # complete image database B
     ├── olympus-test/         # a subset of image database B for testing, including 18 images
  ```



### Executables
#### **Proj2-task1-offline_loading**
- **Description**: Calculates and saves the image feature vector into the output file (Task 1).
- **Usage**:
  ```bash
  Proj2-task1-offline_loading [input_dir] [output_filename]
  ```
- **Example**:
  ```bash
  ../olympus-test/ ../data/feature_vector_1.csv
  ```# CS5330-25spring-proj2
#### **Proj2-task1-TopN_finding**
- **Description**: Calculates and saves the image feature vector into the output file (Task 1).
- **Usage**:
  ```bash
  Proj2-task1-TopN_finding [target_image][feature_file][N]
  ```
- **Example**:
  ```bash
  ../olympus-test//pic.1016.jpg ../data/feature_vector_1.csv 3
  ```# CS5330-25spring-proj2

