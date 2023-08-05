#include "grayscale_filter.h"

bool GrayscaleFilter::Apply(BMP& bmp) {
    const std::vector<std::vector<BMP::PixelAdvanced>>& old_matrix = bmp.GetPixelMatrix();
    const BMP::BMPInfoHeader& image_header = bmp.GetInfoHeader();
    std::vector<std::vector<BMP::PixelAdvanced>> new_matrix(image_header.height);
    for (uint32_t i = 0; i < image_header.height; ++i) {
        new_matrix[i].resize(image_header.width);
        for (uint32_t j = 0; j < image_header.width; ++j) {
            double gray_color = old_matrix[i][j].blue * blue_coef_ + old_matrix[i][j].green * green_coef_ +
                                old_matrix[i][j].red * red_coef_;
            new_matrix[i][j] = {gray_color, gray_color, gray_color};
        }
    }
    bmp.SetPixelMatrix(new_matrix);
    return true;
}