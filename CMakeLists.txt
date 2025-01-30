cmake_minimum_required(VERSION 3.0)
project(Proj2)

set(CMAKE_CXX_STANDARD 14)
find_package(OpenCV REQUIRED)

# Function for OpenCV-only executables
function(add_opencv_executable exe_name)
    add_executable(${exe_name} ${ARGN})
    target_link_libraries(${exe_name} PRIVATE ${OpenCV_LIBS})
    target_include_directories(${exe_name} PRIVATE ${OpenCV_INCLUDE_DIRS})
endfunction()

add_opencv_executable(Proj2-offline_loading
        "../src/feature_writer.cpp"
        "../src/feature_calculate.cpp"
        "../src/csv_util.cpp"
)
add_opencv_executable(Proj2-TopN_finding
        "../src/image_matcher.cpp"
        "../src/distance_calculate.cpp"
        "../src/csv_util.cpp"
        "../src/image_display_util.cpp"
)


