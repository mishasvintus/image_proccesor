#ifndef CPP_HSE_PRODUCERS_H
#define CPP_HSE_PRODUCERS_H

#include "parser.h"
#include "../filters/base_filter.h"
#include "../filters/crop/crop_filter.h"
#include "../filters/grayscale/grayscale_filter.h"
#include "../filters/negative/negative_filter.h"
#include "../filters/sharpening/sharpening_filter.h"
#include "../filters/edge_detection/edge_detection_filter.h"
#include "../filters/gaussian_blur/gaussian_blur_filter.h"
#include "../filters/shuffle/my_filter.h"

class FilterProducers {
public:
    static BaseFilter* CreateCrop(const FilterDescriptor& fd);
    static BaseFilter* CreateGrayscale(const FilterDescriptor& fd);
    static BaseFilter* CreateNegative(const FilterDescriptor& fd);
    static BaseFilter* CreateSharpening(const FilterDescriptor& fd);
    static BaseFilter* CreateEdgeDetection(const FilterDescriptor& fd);
    static BaseFilter* CreateGaussianBlur(const FilterDescriptor& fd);
    static BaseFilter* CreateShuffle(const FilterDescriptor& fd);

private:
    static bool IsNumeric(const std::string& str);
    static bool CheckIsDigitParameters(const FilterDescriptor& fd);
    static bool IsConvertableToDouble(const std::string& str);
};

#endif