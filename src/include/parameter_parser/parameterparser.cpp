#include "parameterparser.h"
#include "utils/exceptions.h"

#include <iterator>
#include <iostream>


ParameterParser::ParserSetup ParameterParser::ParameterParser::addParameter(const std::string &paramName)
{
    auto tempContainer = std::make_shared<ParameterContainer>(paramName);
    mKeyContainer.insert({paramName, tempContainer});
    return {tempContainer};
}

ParameterParser::ValueParserSetup ParameterParser::ParameterParser::addValueParameter(const std::string &paramName)
{
    auto tempContainer = std::make_shared<ParameterContainer>(paramName);
    mKeyContainer.insert({paramName, tempContainer});
    ValueParserSetup parserSetup(tempContainer);
    return parserSetup;
}

ParameterParser::PositionalParserSetup ParameterParser::ParameterParser::addPositionalParameter(const std::string& paramName, size_t position)
{
    auto tempContainer = std::make_shared<ParameterContainer>(paramName);
    if(mPosContainer.size() < position)
        mPosContainer.resize(position);
    mPosContainer.insert(mPosContainer.begin()+position, tempContainer);
    return {tempContainer};
}

void ParameterParser::ParameterParser::parse(int argc,const char *argv[])
{
    std::vector<std::string> arguments(argv + 1, argv + argc);
    degroup(arguments);

    for(auto argument = arguments.begin(); argument != arguments.end(); argument++)
    {

        if(mKeyContainer.count(*argument))
        {
            mKeyContainer.at(*argument)->parse(argument, arguments.end());
        }
        else
        {
            if((*argument)[0] == '-')
                throw feedreaderException::argumentParsing("Invalid argument: %s", (*argument).c_str());
            if(mPosition >= mPosContainer.size())
                throw feedreaderException::argumentParsing("Positional argument %s out of range", (*argument).c_str());
            mPosContainer[mPosition++]->parse(argument, arguments.end());
        }
    }
}

void ParameterParser::ParameterParser::clear()
{
    mKeyContainer.clear();
    mPosContainer.clear();
    mPosition = 0;
}

std::map<std::string, std::string> ParameterParser::ParameterParser::getValues()
{
    std::map<std::string, std::string> returnMap;
    for(const auto &[argName, container] : mKeyContainer)
    {
        if(container->isValid())
            returnMap.insert({argName, container->getValue()});
    }
    for(const auto &container : mPosContainer)
    {
        if(container->isValid())
            returnMap.insert({container->getName(), container->getValue()});
    }
    return returnMap;
}

bool ParameterParser::ParameterParser::argParsed(const std::string& argName) const
{
    return (mKeyContainer.count(argName) && mKeyContainer.at(argName)->isValid());
}

std::string ParameterParser::ParameterParser::getValue(const std::string &argName) const
{
    if(mKeyContainer.count(argName))
    {
        if(mKeyContainer.at(argName)->isValid())
            return mKeyContainer.at(argName)->getValue();
    }
    for(auto && container: mPosContainer)
    {
        if(container->isValid() && (container->getName() == argName))
            return container->getValue();
    }
    return "";
}

void ParameterParser::ParameterParser::degroup(std::vector<std::string>& arguments)
{
    degroupStates state;
    ::std::vector<std::string> parsedArguments;
    for(auto& argument : arguments)
    {
        state = degroupStates::begin;
        for(size_t i = 0; i < argument.size(); i++)
        {
            switch(state)
            {
                case degroupStates::begin:
                    if(argument[i] == '-')
                        state = degroupStates::dash;
                    else
                        state = degroupStates::value;
                    break;
                case degroupStates::dash:
                    if(argument[i] == '-')
                    {
                        state = degroupStates::value;
                        break;
                    }
                    else
                        state = degroupStates::dashParse;
                //fallthrough
                case degroupStates::dashParse:
                    parsedArguments.emplace_back("-" + std::string(1, argument[i]));
                    break;
                case degroupStates::value:
                    parsedArguments.emplace_back(argument);
                    state = degroupStates::begin;
                    break;
            }
            if(state == degroupStates::begin)
                break;
        }
    }
    arguments.clear();
    std::copy(parsedArguments.begin(), parsedArguments.end(), std::back_inserter(arguments));

}

ParameterParser::ValueParserSetup &ParameterParser::ValueParserSetup::setDefaultValue(std::string value)
{
    mContainer->setDefaultValue(std::move(value));
    return *this;
}

ParameterParser::ValueParserSetup &ParameterParser::ValueParserSetup::isDirectory()
{
    mContainer->addParser(std::make_shared<ParserFunctor::isDirectory>());
    return *this;
}

ParameterParser::ValueParserSetup &ParameterParser::ValueParserSetup::isFile()
{
    mContainer->addParser(std::make_shared<ParserFunctor::isFile>());
    return *this;
}

ParameterParser::PositionalParserSetup &ParameterParser::PositionalParserSetup::setDefaultValue(std::string value)
{
    mContainer->setDefaultValue(std::move(value));
    return *this;
}

ParameterParser::PositionalParserSetup &ParameterParser::PositionalParserSetup::isDirectory()
{
    mContainer->addParser(std::make_shared<ParserFunctor::isDirectory>());
    return *this;
}

ParameterParser::PositionalParserSetup &ParameterParser::PositionalParserSetup::isFile()
{
    mContainer->addParser(std::make_shared<ParserFunctor::isFile>());
    return *this;
}
