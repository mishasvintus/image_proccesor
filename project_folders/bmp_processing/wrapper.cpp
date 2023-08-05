#include <fstream>
#include <iostream>
#include <cmath>
#include <algorithm>

#include "wrapper.h"

bool BMP::ReadFile(const std::string& file_name) {
    std::fstream file(file_name, std::ios_base::in | std::ios_base::binary);  // открыли файл
    if (!file.is_open()) {  // проверка, что он открыт
        return false;
    }
    if (!ReadBMPFileHeader(file)) {  // открытие файл-хедера + проверка
        return false;
    }
    if (!ReadBMPImageHeader(file)) {  // открытие инфо-хедера + проверка
        return false;
    }
    if (!ReadBMPColorHeader(file)) {  // открытие колор-хедера + проврека
        return false;
    }
    if (!ReadPixelData(file)) {
        return false;
    }
    file.close();

    bmp_was_read_ = true;

    return true;
}

bool BMP::ReadBMPFileHeader(std::fstream& file) {
    if (!file.read(reinterpret_cast<std::fstream::char_type*>(&bmp_file_header_), sizeof(bmp_file_header_))) {
        return false;
    }
    if (bmp_file_header_.file_type != bmp_type_const_) {  // проверка, что тип файла соответствует BMP
        return false;
    }

    return true;
}

bool BMP::ReadBMPImageHeader(std::fstream& file) {
    if (!file.read(reinterpret_cast<std::fstream::char_type*>(&bmp_image_header_), sizeof(bmp_image_header_))) {
        return false;
    }

    return true;
}

bool BMP::ReadBMPColorHeader(std::fstream& file) {
    if (!file.read(reinterpret_cast<std::fstream::char_type*>(&bmp_color_header_), sizeof(bmp_color_header_))) {
        return false;
    }

    return true;
}

void BMP::ReadMatrix(std::fstream& file) {
    for (uint32_t y0 = 0; y0 < bmp_image_header_.height; ++y0) {
        for (uint32_t x = 0; x < bmp_image_header_.width; ++x) {
            unsigned char pixel_colors[3];
            uint32_t y = bmp_image_header_.height - y0 - 1;  // считываем вверх ногами, тк так хранятся байты в бмп
            file.read(reinterpret_cast<char*>(pixel_colors), 3);
            bmp_pixel_matrix_[y][x].blue = static_cast<double>(pixel_colors[0]) / uchar_max_const_;
            bmp_pixel_matrix_[y][x].green = static_cast<double>(pixel_colors[1]) / uchar_max_const_;
            bmp_pixel_matrix_[y][x].red = static_cast<double>(pixel_colors[2]) / uchar_max_const_;
        }
        file.ignore(padding_number_);
    }
}

void BMP::WriteMatrix(std::ostream& file) {
    // фиктивный пиксель пустой пиксель, константа
    unsigned char bmp_padding[3] = {0, 0, 0};

    for (uint32_t i = 0; i < bmp_image_header_.height; ++i) {
        for (uint32_t j = 0; j < bmp_image_header_.width; ++j) {
            double lo = 0.0;
            double hi = 1.0;
            uint32_t y = bmp_image_header_.height - 1 - i;  // сохраняя переворачиваем, тк считывали переворачивая
            Pixel tmp = {
                static_cast<uint8_t>(std::round(std::clamp(bmp_pixel_matrix_[y][j].blue, lo, hi) * uchar_max_const_)),
                static_cast<uint8_t>(std::round(std::clamp(bmp_pixel_matrix_[y][j].green, lo, hi) * uchar_max_const_)),
                static_cast<uint8_t>(std::round(std::clamp(bmp_pixel_matrix_[y][j].red, lo, hi) * uchar_max_const_))};
            file.write(reinterpret_cast<std::fstream::char_type*>(&tmp), sizeof(Pixel));
        }
        file.write(reinterpret_cast<std::fstream::char_type*>(&bmp_padding), padding_number_);
    }
}

void BMP::WriteHeaders(std::ostream& file) {
    file.write(reinterpret_cast<std::fstream::char_type*>(&bmp_file_header_), sizeof(bmp_file_header_));
    file.write(reinterpret_cast<std::fstream::char_type*>(&bmp_image_header_), sizeof(bmp_image_header_));
    file.write(reinterpret_cast<std::fstream::char_type*>(&bmp_color_header_), sizeof(bmp_color_header_));
}

bool BMP::ReadPixelData(std::fstream& file) {
    if (bmp_image_header_.bits_per_pixel != bit_per_pixel_const_) {
        std::cerr << "Unavailable pixel's format" << std::endl;
        return false;
    }

    uint32_t pixel_matrix_row_size = bmp_image_header_.width * sizeof(Pixel);  // размер одной строки пикселей в байтах
    padding_number_ =
        (4 - (pixel_matrix_row_size % 4)) % 4;  // количество "добавочных" (фиктивных) пикселей в каждой строке

    bmp_pixel_matrix_.resize(bmp_image_header_.height);
    for (uint32_t i = 0; i < bmp_image_header_.height; ++i) {
        bmp_pixel_matrix_[i].resize(bmp_image_header_.width);
    }

    //смещаемся к считыванию pixel_data и считываем
    file.seekg(bmp_file_header_.data_offset, std::ios::beg);
    ReadMatrix(file);

    return true;
}

bool BMP::WriteFile(const std::string& file_name) {
    if (!bmp_was_read_) {
        std::cerr << "You must read a file at first to write it then" << std::endl;
        return false;
    }
    std::ofstream file(file_name, std::ios_base::out | std::ios_base::binary);

    if (!file.is_open()) {
        std::cerr << "Unable to use output path" << std::endl;
        return false;
    }

    // Выравниваем смещение pixel_data
    bmp_file_header_.data_offset = sizeof(bmp_file_header_) + sizeof(bmp_image_header_) + sizeof(bmp_color_header_);
    WriteHeaders(file);
    WriteMatrix(file);

    file.close();
    return true;
}

const std::vector<std::vector<BMP::PixelAdvanced>>& BMP::GetPixelMatrix() {
    return bmp_pixel_matrix_;
}

void BMP::SetPixelMatrix(const std::vector<std::vector<PixelAdvanced>>& new_matrix, uint32_t new_width,
                         uint32_t new_height) {
    uint32_t new_padding = (4 - (new_width * sizeof(Pixel)) % 4) % 4;
    uint32_t new_image_size = (new_width * sizeof(Pixel) + new_padding) * new_height;
    uint32_t new_file_size = bmp_file_header_.file_size - bmp_image_header_.image_size + new_image_size;

    padding_number_ = new_padding;
    bmp_image_header_.image_size = new_image_size;
    bmp_file_header_.file_size = new_file_size;
    bmp_pixel_matrix_ = new_matrix;
    bmp_image_header_.height = new_height;
    bmp_image_header_.width = new_width;
}

void BMP::SetPixelMatrix(const std::vector<std::vector<PixelAdvanced>>& new_matrix) {
    bmp_pixel_matrix_ = new_matrix;
}

const BMP::BMPInfoHeader& BMP::GetInfoHeader() {
    return bmp_image_header_;
}