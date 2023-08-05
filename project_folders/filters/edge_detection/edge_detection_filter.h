#ifndef CPP_HSE_EDGE_DETECTION_FILTER_H
#define CPP_HSE_EDGE_DETECTION_FILTER_H

#include <stdint.h>
#include "../base_filter.h"
#include "../grayscale/grayscale_filter.h"
#include "../../bmp_processing/wrapper.h"

class EdgeDetectionFilter : public BaseFilter {
public:
    explicit EdgeDetectionFilter(double threshold);
    bool Apply(BMP& bmp) override;

private:
    double threshold_;
    const double filter_matrix_[3][3] = {{0, -1, 0}, {-1, 4, -1}, {0, -1, 0}};
};

#endif
