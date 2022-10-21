#include "utils.h"
#include <algorithm>
#include <cctype>

std::pair<std::string, std::string> Utils::splitDelim(const std::string &source,const char delimiter)
{
    auto delimPos = std::find(source.begin(), source.end(), delimiter);
    std::string leftValue (std::string(source.begin(), delimPos));
    std::string rightvalue (std::string((delimPos == source.end()) ? delimPos : delimPos + 1, source.end()));
    return {lrTrim(leftValue, isspace), lrTrim(rightvalue, isspace)};
}

void Utils::normalizeInPlace(std::string &source)
{
    std::transform(source.begin(), source.end(), source.begin(), ::tolower);
}

std::string& Utils::lTrim(std::string &source, std::function<bool(char)> predicat)
{
    std::string::iterator startWord = std::find_if(source.begin(), source.end(),[&predicat] (char c) {return !predicat(c);});
    source.erase(source.begin(), startWord);
    return source;
}

std::string& Utils::rTrim(std::string &source, std::function<bool(char)> predicat)
{
    auto endWord = std::find_if(source.rbegin(), source.rend(), [&predicat] (char c) {return !predicat(c);});
    source.erase(endWord.base(), source.end());
    return source;
}

std::string &Utils::lrTrim(std::string &source, std::function<bool(char)> predicat)
{
    return lTrim(rTrim(source, predicat), predicat);
}

std::string &Utils::toLinuxEndline(std::string &buffer)
{
    buffer.erase(remove_if(buffer.begin(), buffer.end(), [] (char letter) {return letter == '\r'; }), buffer.end());
    return buffer;
}

std::string::iterator Utils::findIt(std::string& source,const std::string& pattern)
{
    auto index = source.find(pattern);
    if(index == std::string::npos)
        return source.end();
    return source.begin() + index;
}
