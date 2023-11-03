# image_proccesor
C++ console bmp file proccesor
# Functional
The program takes as input two paths(absolute/relative): path to BMP file and path for saving result and any quantity of filters as options. The program applies the specified filters to the image and saves the result.
|Filter|Filter_tag|Parameters|Type of parameters|
|------|----------|----------|------------------|
|Crop|-crop|width, height|int, int|
|Grayscale|-gs|-|-|
Negative|-neg|-|-|
Sharpening|-sharp|-|-|
Edge Detection|-edge|threshold|real|
Gaussian Blur|-blur|sigma|real|
Shuffle|-shuffle|horizontal_quantity, vertical_quantity|int, int|
