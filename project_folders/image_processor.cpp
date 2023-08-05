#include <iostream>
#include <vector>
#include <map>

#include "arguments_processing/parser.h"
#include "bmp_processing/wrapper.h"
#include "arguments_processing/filter_pipeline.h"
#include "arguments_processing/producers.h"

using ProducerPtr = BaseFilter* (*)(const FilterDescriptor&);

void Reference() {
    std::cout << "Your request should have the following format: \n";
    std::cout << "\"/BMP file read path/ /BMP file write(save) path/ /filters and their parameters in a row/\"\n\n";

    std::cout << "Filter:        Filter_tag:       Parameters:                                Type of parameters\n";
    std::cout << "_______\n";
    std::cout << "Crop           -crop             width, height                              int, int\n";
    std::cout << "Grayscale      -gs               -                                          -\n";
    std::cout << "Negative       -neg              -                                          -\n";
    std::cout << "Sharpening     -sharp            -                                          -\n";
    std::cout << "Edge Detection -edge             threshold                                  real\n";
    std::cout << "Gaussian Blur  -blur             sigma                                      real\n";
    std::cout << "Shuffle(My)    -shuffle          horizontal_quantity, vertical_quantity     int, int" << std::endl;
}

std::map<std::string, ProducerPtr> MakeMapProducers() {
    std::map<std::string, ProducerPtr> result;

    result["crop"] = &FilterProducers::CreateCrop;
    result["gs"] = &FilterProducers::CreateGrayscale;
    result["neg"] = &FilterProducers::CreateNegative;
    result["sharp"] = &FilterProducers::CreateSharpening;
    result["edge"] = &FilterProducers::CreateEdgeDetection;
    result["blur"] = &FilterProducers::CreateGaussianBlur;
    result["shuffle"] = &FilterProducers::CreateShuffle;

    return result;
}



int main(int argc, char** argv) {
    if (argc == 1) {
        Reference();
        return 0;
    }

    if (argc == 2) {
        std::cerr << "The path to the output file is missing";
        return 1;
    }

    AnalyzedArguments analyzed_argv(argc, argv);

    BMP bmp_image;

    if (!bmp_image.ReadFile(analyzed_argv.GetPathIn())) {
        std::cerr << "There is no possibility to open the given file";
        return 1;
    }

    FilterPipeline filter_pipeline;

    std::map<std::string, ProducerPtr> map_producers = MakeMapProducers();
    try {
        for (const FilterDescriptor& fd : analyzed_argv.GetVectorFilterDescriptor()) {
            if (map_producers.find(fd.name) != map_producers.end()) {
                filter_pipeline.Append(map_producers[fd.name](fd));
            } else {
                throw std::invalid_argument("'" + fd.name + "'" + " filter_tag doesn't exist");
            }
        }
    } catch (const std::invalid_argument& e) {
        std::cout << e.what() << std::endl;
        return 1;
    } catch (const std::bad_alloc& e) {
        std::cout << "Error due to dynamic memory allocation. May be you don't have enough free memory" << std::endl;
    }

    if (!filter_pipeline.Apply(bmp_image)) {
        return 1;
    }

    if (!bmp_image.WriteFile(analyzed_argv.GetPathOut())) {
        std::cerr << "There is no possibility to write a file in the given output path";
        return 1;
    }
    return 0;
}
