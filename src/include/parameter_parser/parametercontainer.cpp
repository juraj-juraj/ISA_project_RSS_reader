#include "parametercontainer.h"
#include "utils/exceptions.h"



void ParameterParser::ParameterContainer::parse(std::vector<std::string>::iterator &actualValue, const std::vector<std::string>::iterator &end)
{
    mParsed = true;
    try {
        for(const auto& parser : mParsers)
        {
            (*parser)(mValue, actualValue, end);
        }
    }
    catch(const feedreaderException::argumentParsing &err)
    {
        throw feedreaderException::argumentParsing("Argument %s : %s \n", mName.c_str(), err.what());
    }
}

void ParameterParser::ParameterContainer::setDefaultValue(std::string value)
{
    mValue = std::move(value);
}

std::string ParameterParser::ParameterContainer::getValue()
{
    return mValue;
}

std::string ParameterParser::ParameterContainer::getName()
{
    return mName;
}

bool ParameterParser::ParameterContainer::isValid()
{
    return mParsed;
}
