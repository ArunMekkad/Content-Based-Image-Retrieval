# Project 2

## Project: [Report Link](https://docs.google.com/document/d/1GSQYxn4meG6uxprJJ3AXL2RdfmDbJhp-DzZt_QJ7Cxk/edit?addon_store&tab=t.0#heading=h.1ikululub2bq)

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
  # 5. RNN feature: 5
  # 7. Depth from DA2: 7
  ```
- **Example**:
  ```bash
  # Task1
  ../olympus/ ../data/feature_vector_1.csv 1
  
  # Task2
  ../olympus/ ../data/feature_vector_2.csv 2
  
  # Task3
  ../olympus/ ../data/feature_vector_3.csv 3
  
  # Task4
  ../olympus/ ../data/feature_vector_4.csv 4
  
  # Task7
  # Reminder: it takes relatively long time to compute these feature vectors, please feel free to use existed 'feature_vector_7.csv' for testing 
  ../olympus/ ../data/feature_vector_7.csv 7
  
  # Extension1 - banana detection
  ../olympus/ ../data/feature_vector_9.csv 9
  
  

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
  # 7. Texture-color with Depth mask: depth && RNN
  # 9. Banana
  # 10. Gabor response
  ```
- **Example**:
  ```bash
  # Task1
  ../olympus/pic.1016.jpg ../data/feature_vector_1.csv 3 ssd
  
  # Task2
  ../olympus/pic.0164.jpg ../data/feature_vector_2.csv 3 rgb-hist
  
  # Task3
  ../olympus/pic.0274.jpg ../data/feature_vector_3.csv 3 multi-hist
  
  # Task3
  ../olympus/pic.0274.jpg ../data/feature_vector_3.csv 3 multi-hist
  
  # Task4
  ../olympus/pic.0535.jpg ../data/feature_vector_4.csv 4 texture-color
  
  # Task5
  ../olympus/pic.0281.jpg ../olympus/ResNet18_olym.csv 5 cosine
  
  # Task7
  ../olympus/pic.0281.jpg ../data/feature_vector_7.csv 5 depth
  
  # Extension1 - banana detection
  ../olympus/pic.0344.jpg ../data/feature_vector_9.csv 5 banana
 