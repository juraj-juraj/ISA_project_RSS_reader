#include "parametercontainer.h"



void ParameterParser::ParameterContainer::parse(std::vector<std::string>::iterator &actualValue, const std::vector<std::string>::iterator &end)
{
    mParsed = true;
    for(const auto &parser : mParsers)
    {
        (*parser)(mValue, actualValue, end);
    }
}

void ParameterParser::ParameterContainer::setDefaultValue(std::string value)
{
    mValue = std::move(value);
}

std::string ParameterParser::ParameterContainer::getValue()
{
    return std::move(mValue);
}

bool ParameterParser::ParameterContainer::isValid()
{
    return mParsed;
}
