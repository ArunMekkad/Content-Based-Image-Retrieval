## CS5330-Proj2

### Project description

#### Task1 - baseline matching

I implemented two programs for image feature matching:

1. The first program, `src/feature_writer.cpp`, calculates the feature vectors of images using `get7x7square ` and stores them in a CSV file. It utilizes utility methods from `csv_util.cpp` provided by the professor.
2. The second program,  `image_matcher.cpp`,  reads the feature vectors from the CSV file and computes the **SSD ** `calculate_ssd `to measure the similarity between images. The distance calculation logic is organized in `distance_calculate.cpp`. All images are then sorted based on their distance, and the top matches are selected. For the matching image filenames, refer to ` find_topN_matches` function. Lastly, using the utility methods in `image_display_util.cpp`, I display the **Top N** matching images.

**Result:**

![image-20250128145323783](/Users/yuyangtian/Documents/NEU/CS5330/Proj2/report-img/task1-terminal.png)

<img src="/Users/yuyangtian/Documents/NEU/CS5330/Proj2/report-img/task1-image.png" alt="image-20250128145330877" style="zoom:50%;" />

#### Task2 - RGB histogram matching

The histogram matching is also implemented in two phrase fashion, similarly:

1. The first program, `src/feature_writer.cpp`, calculates the feature vectors of images using `calculateRGBHistogram ` and stores them in a CSV file. 

2. The second program,  `image_matcher.cpp`,  reads the feature vectors from the CSV file and computes the **Histogram intersection ** `calculate_histogramIntersection ` to measure the similarity between images. The distance calculation logic is organized in `distance_calculate.cpp`.

   <img src="/Users/yuyangtian/Documents/NEU/CS5330/Proj2/report-img/task2-terminal.png" alt="image-20250128160249621" style="zoom:50%;" />

<img src="/Users/yuyangtian/Documents/NEU/CS5330/Proj2/report-img/task2-image.png" alt="image-20250128160209428" style="zoom: 50%;" />

#### Task3 - Multi Histogram

Splits the image into top and bottom halves
Calculates separate RGB histograms for each half using 8 bins per channel
Concatenates the two histograms to form a single feature vector
Uses a weighted distance metric combining histogram intersections of both halves
Implemented in getMultiHistogramFeature and calculate_multiHist_distance functions

#### Task4 - Texture and color matching

Extracts RGB histogram for color (8 bins per channel)
Computes texture features using Sobel gradients and a 16-bin magnitude histogram
Concatenates color and texture histograms into a single feature vector
Uses a weighted distance metric combining color and texture similarities
Implemented in getTextureColorFeature and calculate_textureColor_distance functions

### Acknowledgement

https://mpatacchiola.github.io/blog/2016/11/12/the-simplest-classifier-histogram-intersection.html

https://www.youtube.com/watch?v=r565euxWZBs

https://www.youtube.com/watch?v=gpu9p3d53fg