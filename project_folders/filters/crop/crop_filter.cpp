#include "crop_filter.h"

CropFilter::CropFilter(uint32_t width, uint32_t height) : width_(width), height_(height) {
}

bool CropFilter::Apply(BMP& bmp) {
    std::vector<std::vector<BMP::PixelAdvanced>> old_matrix = bmp.GetPixelMatrix();
    BMP::BMPInfoHeader image_header = bmp.GetInfoHeader();
    uint32_t width = std::min(width_, image_header.width);
    uint32_t height = std::min(height_, image_header.height);

    std::vector<std::vector<BMP::PixelAdvanced>> new_matrix(height);

    for (int i = 0; i < height; ++i) {
        new_matrix[i].resize(width);
        for (int j = 0; j < width; ++j) {
            new_matrix[i][j] = old_matrix[i][j];
        }
    }

    bmp.SetPixelMatrix(new_matrix, width, height);

    return true;
}
