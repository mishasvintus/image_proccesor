#include "negative_filter.h"

bool NegativeFilter::Apply(BMP& bmp) {
    const std::vector<std::vector<BMP::PixelAdvanced>> old_matrix = bmp.GetPixelMatrix();
    const BMP::BMPInfoHeader image_header = bmp.GetInfoHeader();
    std::vector<std::vector<BMP::PixelAdvanced>> new_matrix(image_header.height);
    for (uint32_t i = 0; i < image_header.height; ++i) {
        new_matrix[i].resize(image_header.width);
        for (uint32_t j = 0; j < image_header.width; ++j) {

            new_matrix[i][j] = {1 - old_matrix[i][j].blue, 1 - old_matrix[i][j].green, 1 - old_matrix[i][j].red};
        }
    }
    bmp.SetPixelMatrix(new_matrix);
    return true;
}