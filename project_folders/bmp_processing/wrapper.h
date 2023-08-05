#ifndef INC_20_WRAPPER_H
#define INC_20_WRAPPER_H

#include <cstdint>
#include <fstream>
#include <string>
#include <exception>
#include <vector>

class BMP {
public:
    using Byte = unsigned char;

    struct BMPFileHeader {
        uint16_t file_type;    // тип файла
        uint32_t file_size;    // размер файла
        uint16_t reserved1;    // зарезрвированное поле - должно быть 0
        uint16_t reserved2;    // зарезрвированное поле - должно быть 0
        uint32_t data_offset;  // смещение (в байтах) до начала пиксельных данных
    } __attribute__((packed));

    struct BMPInfoHeader {
        uint32_t size;    // размер заголовка в байтах
        uint32_t width;   // ширина изображения в пикселях
        uint32_t height;  // высота изображения в пикселях
        uint16_t planes;  // количество цветовых плоскостей - должно быть 1
        uint16_t bits_per_pixel;  // количество бит на пиксель (должно быть 24 для BMP-файлов без сжатия)
        uint32_t compression;  // тип сжатия (должно быть 0 для BMP-файлов без сжатия)
        uint32_t image_size;  // размер изображения в байтах (может быть 0 для BMP-файлов без сжатия)
        uint32_t x_pixels_per_meter;  // горизонтальное разрешение изображения (пикселей на метр)
        uint32_t y_pixels_per_meter;  // вертикальное разрешение изображения (пикселей на метр)
        uint32_t total_colors;  // количество цветов изображения (может быть 0 для BMP-файлов без цветовой таблицы)
        uint32_t important_colors;  // количество важных цветов (0 - все цвета важны)
    } __attribute__((packed));

    struct BMPColorHeader {
        int32_t red_mask{0x00ff0000};           // Bit mask for the red channel
        uint32_t green_mask{0x0000ff00};        // Bit mask for the green channel
        uint32_t blue_mask{0x000000ff};         // Bit mask for the blue channel
        uint32_t alpha_mask{0xff000000};        // Bit mask for the alpha channel
        uint32_t color_space_type{0x73524742};  // Default "sRGB" (0x73524742)
        uint32_t unused[16]{0};                 // Unused data for sRGB color space
    } __attribute__((packed));

    struct Pixel {
        unsigned char blue;
        unsigned char green;
        unsigned char red;
    };

    struct PixelAdvanced {
        double blue;
        double green;
        double red;
    };

public:
    bool ReadFile(const std::string& file_name);   // читает файл по данному пути
    bool WriteFile(const std::string& file_name);  // записывает файл по данному пути

    // геттеры
    const std::vector<std::vector<PixelAdvanced>>& GetPixelMatrix();
    const BMPInfoHeader& GetInfoHeader();

    // сеттеры
    void SetPixelMatrix(const std::vector<std::vector<PixelAdvanced>>& new_matrix, uint32_t width, uint32_t height);
    void SetPixelMatrix(const std::vector<std::vector<PixelAdvanced>>& new_matrix);

protected:
    bool ReadBMPFileHeader(std::fstream& file);  // Чтение первого заголовка BMP и возвращение - получилось/нет
    bool ReadBMPImageHeader(std::fstream& file);  // Чтение второго заголовка BMP и возвращение получилось/нет
    bool ReadBMPColorHeader(std::fstream& file);  // Чтение третьего заголовка BMP и возвращение получилось/нет
    bool ReadPixelData(std::fstream& file);  // Составляет двумерный массив из пикселей
    void ReadMatrix(std::fstream& file);
    void WriteMatrix(std::ostream& file);
    void WriteHeaders(std::ostream& file);


private:
    BMPFileHeader bmp_file_header_;
    BMPInfoHeader bmp_image_header_;
    BMPColorHeader bmp_color_header_;
    std::vector<std::vector<PixelAdvanced>> bmp_pixel_matrix_;

    const uint16_t bmp_type_const_ = 0x4d42;
    const double uchar_max_const_ = 255;
    const uint16_t bit_per_pixel_const_ = 24;

    uint32_t padding_number_ = 0;
    bool bmp_was_read_ = false;
};

#endif
