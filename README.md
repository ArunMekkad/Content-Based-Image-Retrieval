# Project 2

## Team Members
1. **Yuyang Tian**
2. **Arun Mekkad**

## Environment
- **Yuyang Tian**: macOS 10.13.1 + CLion + CMake
- **Arun**: (Ubuntu 22.04 LTS + VS Code + CMake)

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
#### **Proj2-offline_loading**

- **Description**: Calculates and saves the image feature vector into the output file.
- **Usage**:
  ```bash
  Proj2-offline_loading [input_dir] [output_filename][feature type]
  # feature type option
  # 1. 7x7 square:  1
  # 2. RGB histogram: 2
  # 3. Multi histogram: 3
  # 4. Texture-color: 4
  ```
- **Example**:
  ```bash
  # Task1
  ../olympus-test/ ../data/feature_vector_1.csv 1

  # Task2
  ../olympus-test/ ../data/feature_vector_2.csv 2

  # Task3
  ../olympus/ ../data/feature_vector_3.csv 3

  # Task4
  ../olympus/ ../data/feature_vector_4.csv 4

#### **Proj2-TopN_finding**

- **Description**: Calculates and saves the image feature vector into the output file.
- **Usage**:
  ```bash
  Proj2-TopN_finding [target_image][feature_file][N][distance_metrics]
  # distance metrics option
  # 1. sum-of-squared-difference: ssd
  # 2. RGB histogram: rgb-hist
  # 3. Multi histogram: multi-hist
  # 4. Texture-color: texture-color
  ```
- **Example**:
  ```bash
  # Task1
  ../olympus-test/pic.1016.jpg ../data/feature_vector_1.csv 3 ssd

  # Task2
  ../olympus-test/pic.0164.jpg ../data/feature_vector_2.csv 3 rgb-hist
  
  # Task3
  ../olympus/pic.0274.jpg ../data/feature_vector_3.csv 3 multi-hist
  
  # Task4
  ../olympus/pic.0535.jpg ../data/feature_vector_4.csv 4 texture-color

 