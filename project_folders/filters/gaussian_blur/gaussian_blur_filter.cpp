#include "gaussian_blur_filter.h"
#include <cmath>

GaussianBlurFilter::GaussianBlurFilter(double sigma) : sigma_(sigma) {
}

bool GaussianBlurFilter::Apply(BMP& bmp) {
    const std::vector<std::vector<BMP::PixelAdvanced>>& old_matrix = bmp.GetPixelMatrix();
    const BMP::BMPInfoHeader& image_header = bmp.GetInfoHeader();

    std::vector<std::vector<BMP::PixelAdvanced>> new_matrix(image_header.height);
    for (uint32_t i = 0; i < image_header.height; ++i) {
        new_matrix[i].resize(image_header.width);
    }
    int triple_sigma = static_cast<int>(3 * sigma_);
    std::vector<double> kernel;
    kernel.reserve(std::ceil(6 * sigma_ + 1));                               // NOLINT
    for (int i = 0; i < static_cast<int>(std::ceil(6 * sigma_ + 1)); i++) {  // NOLINT
        kernel.push_back(static_cast<double>(
            expf(-powf(static_cast<float>(i - triple_sigma), 2) / (2 * powf(static_cast<float>(sigma_), 2)))));
    }
    double summ = 0;
    for (double i : kernel) {
        summ += i;
    }
    for (double& i : kernel) {
        i /= summ;
    }
    for (int y = 0; y < image_header.height; y++) {
        for (int x = 0; x < image_header.width; x++) {
            double blue_sum = 0.0;
            double green_sum = 0.0;
            double red_sum = 0.0;
            for (int i = -triple_sigma; i <= triple_sigma; i++) {
                int yi = std::max(0, std::min(y + i, static_cast<int>(image_header.height) - 1));
                blue_sum += kernel[i + triple_sigma] * old_matrix[yi][x].blue;
                green_sum += kernel[i + triple_sigma] * old_matrix[yi][x].green;
                red_sum += kernel[i + triple_sigma] * old_matrix[yi][x].red;
            }
            new_matrix[y][x] = {blue_sum, green_sum, red_sum};
        }
    }

    bmp.SetPixelMatrix(new_matrix);

    const std::vector<std::vector<BMP::PixelAdvanced>>& new_old_matrix = bmp.GetPixelMatrix();
    const BMP::BMPInfoHeader& new_image_header = bmp.GetInfoHeader();

    for (int x = 0; x < new_image_header.width; x++) {
        for (int y = 0; y < new_image_header.height; y++) {
            double blue_sum = 0.0;
            double green_sum = 0.0;
            double red_sum = 0.0;
            for (int i = -triple_sigma; i <= triple_sigma; i++) {
                int xi = std::max(0, std::min(x + i, static_cast<int>(new_image_header.width) - 1));
                blue_sum += kernel[i + triple_sigma] * new_old_matrix[y][xi].blue;
                green_sum += kernel[i + triple_sigma] * new_old_matrix[y][xi].green;
                red_sum += kernel[i + triple_sigma] * new_old_matrix[y][xi].red;
            }
            new_matrix[y][x] = {blue_sum, green_sum, red_sum};
        }
    }
    bmp.SetPixelMatrix(new_matrix);

    return true;
}