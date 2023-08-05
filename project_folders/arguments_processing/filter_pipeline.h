#ifndef CPP_HSE_FILTER_PIPELINE_H
#define CPP_HSE_FILTER_PIPELINE_H

#include "parser.h"
#include "../filters/base_filter.h"

class FilterPipeline {
public:
    void Append(BaseFilter*);
    bool Apply(BMP& bmp);
    FilterPipeline() = default;
    ~FilterPipeline();
    FilterPipeline& operator=(const FilterPipeline&) = delete;
    FilterPipeline(const FilterPipeline&) = delete;
private:
    std::vector<BaseFilter*> filters_;
};
#endif
