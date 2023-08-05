#ifndef CPP_HSE_NEGATIVE_FILTER_H
#define CPP_HSE_NEGATIVE_FILTER_H

#include <stdint.h>
#include "../base_filter.h"
#include "../../bmp_processing/wrapper.h"

class NegativeFilter : public BaseFilter {
public:
    bool Apply(BMP& bmp) override;
};

#endif
