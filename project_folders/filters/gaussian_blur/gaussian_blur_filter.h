#ifndef CPP_HSE_GAUSSIAN_BLUR_FILTER_H
#define CPP_HSE_GAUSSIAN_BLUR_FILTER_H

#include <stdint.h>
#include "../base_filter.h"
#include "../../bmp_processing/wrapper.h"

class GaussianBlurFilter : public BaseFilter {
public:
    explicit GaussianBlurFilter(double sigma);
    bool Apply(BMP& bmp) override;

private:
    double sigma_;
};
#endif
