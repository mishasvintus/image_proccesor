cmake_minimum_required(VERSION 3.24)
project(image_processor)

set(CMAKE_CXX_STANDARD 17)

add_executable(
    image_processor
        project_folders/image_processor.cpp
        project_folders/arguments_processing/parser.cpp project_folders/bmp_processing/wrapper.cpp project_folders/filters/crop/crop_filter.cpp project_folders/arguments_processing/filter_pipeline.cpp project_folders/arguments_processing/filter_pipeline.h project_folders/arguments_processing/producers.cpp project_folders/arguments_processing/producers.h project_folders/filters/grayscale/grayscale_filter.cpp project_folders/filters/grayscale/grayscale_filter.h project_folders/filters/negative/negative_filter.cpp project_folders/filters/negative/negative_filter.h project_folders/filters/sharpening/sharpening_filter.cpp project_folders/filters/sharpening/sharpening_filter.h project_folders/filters/edge_detection/edge_detection_filter.cpp project_folders/filters/edge_detection/edge_detection_filter.h project_folders/filters/shuffle/my_filter.cpp project_folders/filters/shuffle/my_filter.h project_folders/filters/gaussian_blur/gaussian_blur_filter.cpp project_folders/filters/gaussian_blur/gaussian_blur_filter.h)
