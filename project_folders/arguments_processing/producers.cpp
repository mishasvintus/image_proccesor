#include "producers.h"
#include <exception>

bool FilterProducers::CheckIsDigitParameters(const FilterDescriptor& fd) {

    for (const std::string& str : fd.parameters) {
        if (!IsNumeric(str)) {
            return false;
        }
    }
    return true;
}

bool FilterProducers::IsNumeric(const std::string& str) {
    for (char letter : str) {
        if (!isdigit(letter)) {
            return false;
        }
    }
    return true;
}

bool FilterProducers::IsConvertableToDouble(const std::string& str) {
    if (!isdigit(str[0])) {
        return false;
    }

    bool last_was_point = false;
    bool was_point = false;
    for (char letter : str) {
        if (isdigit(letter)) {
            last_was_point = false;
            continue;
        }
        if (letter == '.' && !was_point) {
            was_point = true;
            last_was_point = true;
            continue;
        }
        return false;
    }
    if (last_was_point) {
        return false;
    }
    return true;
}

BaseFilter* FilterProducers::CreateCrop(const FilterDescriptor& fd) {
    if (fd.parameters.size() != 2) {
        throw std::invalid_argument("Crop filter must have two arguments: width, height");
    }
    if (!CheckIsDigitParameters(fd)) {
        throw std::invalid_argument("Crop filter's parameters must be integers");
    }
    uint32_t width = stoi(fd.parameters[0]);
    uint32_t height = stoi(fd.parameters[1]);

    BaseFilter* result = new (std::nothrow) CropFilter(width, height);

    if (result == nullptr) {
        throw std::bad_alloc();
    }

    return result;
}

BaseFilter* FilterProducers::CreateGrayscale(const FilterDescriptor& fd) {
    if (!fd.parameters.empty()) {
        throw std::invalid_argument("Grayscale filter doesn't have any parameters");
    }
    BaseFilter* result = new (std::nothrow) GrayscaleFilter();

    if (result == nullptr) {
        throw std::bad_alloc();
    }

    return result;
}

BaseFilter* FilterProducers::CreateNegative(const FilterDescriptor& fd) {
    if (!fd.parameters.empty()) {
        throw std::invalid_argument("Negative filter doesn't have any parameters");
    }
    BaseFilter* result = new (std::nothrow) NegativeFilter();

    if (result == nullptr) {
        throw std::bad_alloc();
    }

    return result;
}

BaseFilter* FilterProducers::CreateSharpening(const FilterDescriptor& fd) {
    if (!fd.parameters.empty()) {
        throw std::invalid_argument("Sharpening filter doesn't have any parameters");
    }
    BaseFilter* result = new (std::nothrow) SharpeningFilter();

    if (result == nullptr) {
        throw std::bad_alloc();
    }

    return result;
}

BaseFilter* FilterProducers::CreateEdgeDetection(const FilterDescriptor& fd) {
    if (fd.parameters.size() != 1) {
        throw std::invalid_argument("Edge Detection filter must have one argument: threshold");
    }
    if (!IsConvertableToDouble(fd.parameters[0])) {
        throw std::invalid_argument(
            "Edge detection filter's parameter must be real number: /some digits/./some digits/");
    }
    double threshold = stod(fd.parameters[0]);
    BaseFilter* result = new (std::nothrow) EdgeDetectionFilter(threshold);

    if (result == nullptr) {
        throw std::bad_alloc();
    }

    return result;
}

BaseFilter* FilterProducers::CreateGaussianBlur(const FilterDescriptor& fd) {
    if (fd.parameters.size() != 1) {
        throw std::invalid_argument("Gaussian Blur filter must have one argument: sigma");
    }
    if (!IsConvertableToDouble(fd.parameters[0])) {
        throw std::invalid_argument(
            "Gaussian Blur filter's parameter must be real number: /some digits/./some digits/");
    }
    double sigma = stod(fd.parameters[0]);

    BaseFilter* result = new (std::nothrow) GaussianBlurFilter(sigma);

    if (result == nullptr) {
        throw std::bad_alloc();
    }

    return result;
}

BaseFilter* FilterProducers::CreateShuffle(const FilterDescriptor& fd) {
    if (fd.parameters.size() != 2) {
        throw std::invalid_argument("Shuffle filter must have two arguments: horizontal_quantity, vertical_quantity");
    }
    if (!CheckIsDigitParameters(fd)) {
        throw std::invalid_argument("Shuffle filter's parameters must be positive integers");
    }
    if (fd.parameters[0] == "0" || fd.parameters[1] == "0") {
        throw std::invalid_argument("Shuffle filter's parameters must be positive integers");
    }
    uint32_t horizontal_quantity = stoi(fd.parameters[0]);
    uint32_t vertical_quantity = stoi(fd.parameters[1]);

    BaseFilter* result = new (std::nothrow) ShuffleFilter(horizontal_quantity, vertical_quantity);

    if (result == nullptr) {
        throw std::bad_alloc();
    }

    return result;
}
