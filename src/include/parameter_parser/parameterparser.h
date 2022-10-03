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

class ValueParserSetup
{
public:

    ValueParserSetup(std::shared_ptr<ParameterContainer>& container) : mContainer(container)
    {
        withValue();
    };

    ValueParserSetup& setDefaultValue(std::string value);

    ValueParserSetup& isDirectory();

    ValueParserSetup& isFile();

protected:
    void withValue();

private:
    std::shared_ptr<ParameterContainer> mContainer;
};

class ParserSetup
{
public:

    ParserSetup(std::shared_ptr<ParameterContainer>& container) : mContainer(container) {};

private:
    std::shared_ptr<ParameterContainer> mContainer;
};


class ParameterParser
{
public:
    ParserSetup addParameter(const std::string& paramName);

    ValueParserSetup addValueParameter(const std::string& paramName);

    ValueParserSetup addPositionalParameter(size_t position);

    void parse(int argc,const char *argv[]);

    [[nodiscard]] std::map<std::string, std::string> getValues();

    [[nodiscard]] bool argParsed(const std::string& argName) const;

protected:

    static void degroup(std::vector<std::string>& arguments);

private:
    size_t mPosition = 0;

    std::unordered_map<std::string, std::shared_ptr<ParameterContainer>> mKeyContainer;
    std::vector<std::shared_ptr<ParameterContainer>> mPosContainer;
};

}
