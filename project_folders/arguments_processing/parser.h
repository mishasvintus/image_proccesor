#ifndef CPP_HSE_PARSER_H
#define CPP_HSE_PARSER_H

#include <string>
#include <vector>

struct FilterDescriptor {
    FilterDescriptor(std::string name, std::vector<std::string> parameters) : name(name), parameters(parameters) {
    }

    std::string name;
    std::vector<std::string> parameters;
};

class AnalyzedArguments {
public:
    AnalyzedArguments(int argc, char** argv);

    const std::string& GetPathIn() const;
    const std::string& GetPathOut() const;
    const std::vector<FilterDescriptor>& GetVectorFilterDescriptor() const;

private:
    std::vector<FilterDescriptor> fd_vector_;
    std::string pathin_;
    std::string pathout_;
};

#endif
