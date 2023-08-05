#ifndef CPP_HSE_BASE_FILTER_H
#define CPP_HSE_BASE_FILTER_H

#include "../bmp_processing/wrapper.h"

class BaseFilter {
public:
    virtual bool Apply(BMP&) = 0;
    virtual ~BaseFilter() = default;
};

#endif
