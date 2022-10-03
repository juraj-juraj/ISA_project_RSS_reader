#pragma once

#include <string>
#include <vector>
#include <memory>

#include <parameter_parser/parametercheckers.h>

namespace ParameterParser
{
class ParameterContainer
{
public:
    void parse(std::vector<std::string>::iterator& actualValue,const std::vector<std::string>::iterator& end);

    void setDefaultValue(std::string value);

    void addParser(std::shared_ptr<ParserFunctor::parseFunctorI> parser)
    {
        mParsers.emplace_back(parser);
    }

    std::string getValue();

    bool isValid();

private:
    std::vector<std::shared_ptr<ParserFunctor::parseFunctorI>> mParsers;
    std::string mValue;
    bool mParsed;
};

}
