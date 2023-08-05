#include "filter_pipeline.h"

void FilterPipeline::Append(BaseFilter* filter) {
    filters_.push_back(filter);
}

bool FilterPipeline::Apply(BMP& bmp) {
    for (BaseFilter* filter : filters_) {
        if (!filter->Apply(bmp)) {
            return false;
        }
    }
    return true;
}

FilterPipeline::~FilterPipeline() {
    for (BaseFilter* filter : filters_) {
        delete filter;
    }
}