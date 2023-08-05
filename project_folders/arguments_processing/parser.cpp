#include "parser.h"
#include <stdexcept>

AnalyzedArguments::AnalyzedArguments(int argc, char** argv) : pathin_(argv[1]), pathout_(argv[2]) {

    for (int i = 3; i < argc; ++i) {
        std::string current_filter_name = argv[i];

        if (current_filter_name[0] != '-') {
            throw std::invalid_argument("Wrong parameters, filter's name should start with '-'");
        }

        current_filter_name = ++argv[i];
        ++i;
        std::vector<std::string> parameters;

        while (i < argc) {
            std::string current_parameter = argv[i];
            if (current_parameter[0] == '-') {
                --i;
                break;
            }
            parameters.push_back(current_parameter);
            ++i;
        }

        fd_vector_.push_back(FilterDescriptor(current_filter_name, parameters));
    }
}

const std::string& AnalyzedArguments::GetPathIn() const {
    return pathin_;
}

const std::string& AnalyzedArguments::GetPathOut() const {
    return pathout_;
}

const std::vector<FilterDescriptor>& AnalyzedArguments::GetVectorFilterDescriptor() const {
    return fd_vector_;
}