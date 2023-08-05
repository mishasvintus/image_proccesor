#include "my_filter.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <ctime>

ShuffleFilter::ShuffleFilter(uint32_t horizontal_quantity, uint32_t vertical_quantity)
    : horizontal_quantity_(horizontal_quantity), vertical_quantity_(vertical_quantity) {
}

bool ShuffleFilter::Apply(BMP& bmp) {
    const std::vector<std::vector<BMP::PixelAdvanced>> old_matrix = bmp.GetPixelMatrix();
    const BMP::BMPInfoHeader& image_header = bmp.GetInfoHeader();
    if (image_header.width < horizontal_quantity_) {
        std::cerr
            << "Horizontal_quantity parameter in Mixing filter must be not more than width of the BMP file in pixels"
            << std::endl;
        return false;
    }
    if (image_header.height < vertical_quantity_) {
        std::cerr
            << "Vertical_quantity parameter in Mixing filter must be not more than width of the BMP file in pixels"
            << std::endl;
        return false;
    }

    const uint32_t x_remainder = image_header.width % horizontal_quantity_;
    const uint32_t y_remainder = image_header.height % vertical_quantity_;
    uint32_t tmp_y_remainder = y_remainder;
    uint32_t tmp_x_remainder = x_remainder;

    uint32_t vert_plate_size = image_header.height / vertical_quantity_;
    uint32_t hor_plate_size = image_header.width / horizontal_quantity_;
    uint32_t total_quantity = horizontal_quantity_ * vertical_quantity_;
    uint32_t new_height = vert_plate_size * vertical_quantity_;
    uint32_t new_width = hor_plate_size * horizontal_quantity_;

    std::vector<std::vector<BMP::PixelAdvanced>> new_matrix(new_height);
    for (std::vector<BMP::PixelAdvanced>& row : new_matrix) {
        row.resize(new_width);
    }

    std::vector<uint32_t> numbers_of_plates(total_quantity);  // номера плиток, которые мы перемешаем

    for (uint32_t i = 0; i < total_quantity; ++i) {
        numbers_of_plates[i] = i;
    }

    auto rng = std::default_random_engine{};
    rng.seed(static_cast<unsigned>(time(NULL)));
    std::shuffle(std::begin(numbers_of_plates), std::end(numbers_of_plates), rng);

    uint32_t x_shift = 0;
    uint32_t y_shift = 0;

    uint32_t plates_processed = 0;
    // Здесь мы расставляем пиксели из старой матрицы в новую сразу на новые места (место переехавшей плитки берем из
    // numbers_of_plates. То есть плитка под номером N переезжает в плитку под номером numbers_of_plates[N]
    // Также не забываем пропускать "остаточные" пиксели
    for (uint32_t vert_shift = 0; vert_shift < vertical_quantity_; ++vert_shift) {
        for (uint32_t hor_shift = 0; hor_shift < horizontal_quantity_; ++hor_shift) {
            for (uint32_t y0 = 0; y0 < vert_plate_size; ++y0) {
                for (uint32_t x0 = 0; x0 < hor_plate_size; ++x0) {
                    uint32_t y = y0 + vert_plate_size * vert_shift + y_shift;
                    uint32_t x = x0 + hor_plate_size * hor_shift + x_shift;

                    uint32_t new_y =
                        y0 + (numbers_of_plates[plates_processed] / horizontal_quantity_) * vert_plate_size;
                    uint32_t new_x = x0 + (numbers_of_plates[plates_processed] % horizontal_quantity_) * hor_plate_size;
                    new_matrix[new_y][new_x] = old_matrix[y][x];
                }
            }
            ++plates_processed;
            if (tmp_x_remainder > 0) {
                ++x_shift;
                --tmp_x_remainder;
            }
        }
        x_shift = 0;
        tmp_x_remainder = x_remainder;
        if (tmp_y_remainder > 0) {
            ++y_shift;
            --tmp_y_remainder;
        }
    }

    bmp.SetPixelMatrix(new_matrix, new_width, new_height);

    return true;
}