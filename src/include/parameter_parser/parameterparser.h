#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <vector>
#include <map>

#include <parameter_parser/parametercontainer.h>

namespace ParameterParser
{

enum class degroupStates
{
  begin,
  dash,
  dashParse,
  value
};

class ParserSetup
{
public:

    ParserSetup(std::shared_ptr<ParameterContainer>& container) : mContainer(container) {};

protected:
    std::shared_ptr<ParameterContainer> mContainer;
};

class ValueParserSetup : public ParserSetup
{
public:

    ValueParserSetup(std::shared_ptr<ParameterContainer>& container) : ParserSetup(container)
    {
        mContainer->addParser(std::make_shared<ParserFunctor::withValue>());
    };

    ValueParserSetup& setDefaultValue(std::string value);

    ValueParserSetup& isDirectory();

    ValueParserSetup& isFile();

private:
    using ParserSetup::ParserSetup;
};

class PositionalParserSetup : ParserSetup
{
public:

    PositionalParserSetup(std::shared_ptr<ParameterContainer>& container) : ParserSetup(container)
    {
        mContainer->addParser(std::make_shared<ParserFunctor::positionalValue>());
    };

    PositionalParserSetup& setDefaultValue(std::string value);

    PositionalParserSetup& isDirectory();

    PositionalParserSetup& isFile();
};


class ParameterParser
{
public:
    ParserSetup addParameter(const std::string& paramName);

    ValueParserSetup addValueParameter(const std::string& paramName);

    PositionalParserSetup addPositionalParameter(const std::string& paramName, size_t position);

    void parse(int argc,const char *argv[]);

    void clear();

    [[nodiscard]] std::map<std::string, std::string> getValues();

    [[nodiscard]] bool argParsed(const std::string& argName) const;

    [[nodiscard]] std::string getValue(const std::string& argname) const;

protected:

    static void degroup(std::vector<std::string>& arguments);

private:
    size_t mPosition = 0;

    std::unordered_map<std::string, std::shared_ptr<ParameterContainer>> mKeyContainer;
    std::vector<std::shared_ptr<ParameterContainer>> mPosContainer;
};

}
