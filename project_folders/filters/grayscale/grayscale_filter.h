#ifndef CPP_HSE_GRAYSCALE_FILTER_H
#define CPP_HSE_GRAYSCALE_FILTER_H

#include <stdint.h>
#include "../base_filter.h"
#include "../../bmp_processing/wrapper.h"

class GrayscaleFilter : public BaseFilter {
public:
    bool Apply(BMP& bmp) override;

private:
    const double red_coef_ = 0.299;
    const double green_coef_ = 0.587;
    const double blue_coef_ = 0.114;
};

#endif
