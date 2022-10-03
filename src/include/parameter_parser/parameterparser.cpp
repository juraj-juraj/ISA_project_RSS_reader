#include "parameterparser.h"
#include "utils/utils.h"
#include "utils/exceptions.h"

#include <iterator>
#include <iostream>


ParameterParser::ParserSetup ParameterParser::ParameterParser::addParameter(const std::string &paramName)
{
    auto tempContainer = std::make_shared<ParameterContainer>();
    mKeyContainer.insert({paramName, tempContainer});
    return {tempContainer};
}

ParameterParser::ValueParserSetup ParameterParser::ParameterParser::addValueParameter(const std::string &paramName)
{
    auto tempContainer = std::make_shared<ParameterContainer>();
    mKeyContainer.insert({paramName, tempContainer});
    ValueParserSetup parserSetup(tempContainer);
    return parserSetup;
}

ParameterParser::ValueParserSetup ParameterParser::ParameterParser::addPositionalParameter(size_t position)
{
    auto tempContainer = std::make_shared<ParameterContainer>();
    if(mPosContainer.size() < position)
        mPosContainer.resize(position);
    mPosContainer.assign(position, tempContainer);
    return {tempContainer};
}

void ParameterParser::ParameterParser::parse(int argc,const char *argv[])
{
    std::vector<std::string> arguments(argv + 1, argv + argc);
    degroup(arguments);
    auto argument = arguments.begin();
    try
    {
        for(; argument != arguments.end(); argument++)
        {

            if(mKeyContainer.count(*argument))
            {
                mKeyContainer.at(*argument)->parse(argument, arguments.end());
            }
            else
            {
                if((*argument)[0] == '-')
                    throw std::invalid_argument("Invalid argument");
                if(mPosition >= mPosContainer.size())
                    throw std::invalid_argument("Positional argument out of range");
                mPosContainer[mPosition++]->parse(argument, arguments.end());
            }
        }
    }
    catch(const feedreaderException::argumentParsing& err)
    {
        throw feedreaderException::argumentParsing("Argument %s : %s \n", (*argument).c_str(), err.what());
    }
}

std::map<std::string, std::string> ParameterParser::ParameterParser::getValues()
{
    std::map<std::string, std::string> returnMap;
    for(const auto &[argName, container] : mKeyContainer)
    {
        if(container->isValid())
            returnMap.insert({argName, container->getValue()});
    }
    return returnMap;
}

bool ParameterParser::ParameterParser::argParsed(const std::string& argName) const
{
    return (mKeyContainer.count(argName) && mKeyContainer.at(argName)->isValid());
}

void ParameterParser::ParameterParser::degroup(std::vector<std::string>& arguments)
{
    degroupStates state;
    ::std::vector<std::string> parsedArguments;
    for(auto& argument : arguments)
    {
        state = degroupStates::begin;
        for(size_t i = 0; i <= argument.size(); i++)
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
                    if(argument[i] != '\0')
                    {
                        parsedArguments.emplace_back("-" + std::string(1, argument[i]));
                    }
                    break;
                case degroupStates::value:
                    if(argument[i] == '\0')
                        parsedArguments.emplace_back(argument);
                    break;
            }
        }
        arguments.clear();
        std::copy(parsedArguments.begin(), parsedArguments.end(), std::back_inserter(arguments));
    }
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

void ParameterParser::ValueParserSetup::withValue()
{
    mContainer->addParser(std::make_shared<ParserFunctor::withValue>());
}
