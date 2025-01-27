## CS5330-Proj2

### Project description

#### Task1 - baseline matching

I implemented two programs for image feature matching:

1. The first program, `src/feature_writer.cpp`, calculates the feature vectors of images and stores them in a CSV file. It utilizes utility methods from `csv_util.cpp` provided by the professor.
2. The second program,  `image_matcher_baseline.cpp`,  reads the feature vectors from the CSV file and computes the **SSD **to measure the similarity between images. The distance calculation logic is organized in `distance_calculate.cpp`. All images are then sorted based on their distance, and the top matches are selected. For the matching image filenames, refer to ` find_topN_matches` function. Lastly, using the utility methods in `image_display_util.cpp`, I display the **Top N** matching images.