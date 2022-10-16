#include "parametercheckers.h"
#include "utils/exceptions.h"

#include <filesystem>

void ParserFunctor::withValue::operator()(
  std::string& value, std::vector<std::string>::iterator& actualValue, const std::vector<std::string>::iterator& end)
{

    if(actualValue + 1 == end)
    {
        throw feedreaderException::argumentParsing("Missing value");
    }
    actualValue++;
    value = std::move(*actualValue);
}

void ParserFunctor::isFile::operator()(std::string& value, std::vector<std::string>::iterator& /*actualValue*/,
  const std::vector<std::string>::iterator& /*end*/)
{
    if(!std::filesystem::is_regular_file(value))
        throw feedreaderException::argumentParsing("Cannot find specified file");
}

void ParserFunctor::isDirectory::operator()(std::string& value, std::vector<std::string>::iterator& /*actualValue*/,
  const std::vector<std::string>::iterator& /*end*/)
{
    if(!std::filesystem::is_directory(value))
        throw feedreaderException::argumentParsing("Cannot open specified file");
}

void ParserFunctor::positionalValue::operator()(std::string& value, std::vector<std::string>::iterator& actualValue,
  const std::vector<std::string>::iterator& /*end*/)
{
    value = std::move(*actualValue);
}
