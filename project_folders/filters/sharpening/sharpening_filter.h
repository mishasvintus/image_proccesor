#ifndef CPP_HSE_SHARPENING_FILTER_H
#define CPP_HSE_SHARPENING_FILTER_H

#include <stdint.h>
#include "../base_filter.h"
#include "../../bmp_processing/wrapper.h"

class SharpeningFilter : public BaseFilter {
public:
    bool Apply(BMP& bmp) override;

private:
    const double filter_matrix_[3][3] = {{0, -1, 0}, {-1, 5, -1}, {0, -1, 0}};
};

#endif
