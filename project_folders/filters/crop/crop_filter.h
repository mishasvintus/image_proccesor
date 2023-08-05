#ifndef CPP_HSE_CROP_FILTER_H
#define CPP_HSE_CROP_FILTER_H

#include <stdint.h>
#include "../base_filter.h"
#include "../../bmp_processing/wrapper.h"

class CropFilter : public BaseFilter {
public:
    CropFilter(uint32_t width, uint32_t height);
    bool Apply(BMP& bmp) override;

private:
    uint32_t width_;
    uint32_t height_;
};

#endif
