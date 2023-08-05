#include "edge_detection_filter.h"

EdgeDetectionFilter::EdgeDetectionFilter(double threshold) : threshold_(threshold) {
}
bool EdgeDetectionFilter::Apply(BMP& bmp) {
    GrayscaleFilter gray_filter;
    gray_filter.Apply(bmp);  // переводим в оттенки серого
    const std::vector<std::vector<BMP::PixelAdvanced>>& old_matrix = bmp.GetPixelMatrix();
    const BMP::BMPInfoHeader& image_header = bmp.GetInfoHeader();
    std::vector<std::vector<BMP::PixelAdvanced>> new_matrix(image_header.height);
    for (uint32_t y = 0; y < image_header.height; ++y) {
        new_matrix[y].resize(image_header.width);
        for (uint32_t x = 0; x < image_header.width; ++x) {
            const BMP::PixelAdvanced& middle_pixel = old_matrix[y][x];
            const BMP::PixelAdvanced& top_pixel = (y > 0) ? old_matrix[y - 1][x] : middle_pixel;
            const BMP::PixelAdvanced& left_pixel = (x > 0) ? old_matrix[y][x - 1] : middle_pixel;
            const BMP::PixelAdvanced& right_pixel =
                (x < image_header.width - 1) ? old_matrix[y][x + 1] : middle_pixel;
            const BMP::PixelAdvanced& bottom_pixel =
                (y < image_header.height - 1) ? old_matrix[y + 1][x] : middle_pixel;

            double blue = std::min(
                1.0,
                std::max(0.0, top_pixel.blue * filter_matrix_[0][1] + left_pixel.blue * filter_matrix_[1][0] +
                                  middle_pixel.blue * filter_matrix_[1][1] + right_pixel.blue * filter_matrix_[1][2] +
                                  bottom_pixel.blue * filter_matrix_[2][1]));
            double green = std::min(
                1.0,
                std::max(0.0, top_pixel.green * filter_matrix_[0][1] + left_pixel.green * filter_matrix_[1][0] +
                                  middle_pixel.green * filter_matrix_[1][1] + right_pixel.green * filter_matrix_[1][2] +
                                  bottom_pixel.green * filter_matrix_[2][1]));
            double red = std::min(
                1.0,
                std::max(0.0, top_pixel.red * filter_matrix_[0][1] + left_pixel.red * filter_matrix_[1][0] +
                                  middle_pixel.red * filter_matrix_[1][1] + right_pixel.red * filter_matrix_[1][2] +
                                  bottom_pixel.red * filter_matrix_[2][1]));
            new_matrix[y][x] = {(blue > threshold_) ? 1.0 : 0.0, (green > threshold_) ? 1.0 : 0.0,
                                (red > threshold_) ? 1.0 : 0.0};
        }
    }

    bmp.SetPixelMatrix(new_matrix);

    return true;
}