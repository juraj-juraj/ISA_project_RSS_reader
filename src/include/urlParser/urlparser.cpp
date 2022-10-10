#include <fstream>
#include <sstream>
#include <algorithm>

#include <iostream>

#include "urlparser.h"
#include "utils/exceptions.h"


std::regex urlParser::txt_regex(R"(^((http)|(https))(\:\/\/)((www\.)?[a-zA-Z0-9\.\-\_]+[a-z]{1,8})(\:\d+)?(\/[^\ \t\?]*)?(\?\S+)?$)");

namespace urlParser
{
    void parseURL(std::string&  URL, struct URLAddress& address)
    {
        std::string::iterator startURL = URL.begin();
        std::string::iterator endURL = URL.end();
        std::string::iterator endProtocol = std::find(startURL, endURL, ':');
        std::string::iterator addressStart = endProtocol + 3; //skip ://
        std::string::iterator addressEnd = std::find(addressStart, endURL, '/');
        std::string::iterator portStart = std::find(addressStart, addressEnd, ':');
        std::string::iterator pathEnd = std::find(addressEnd, endURL, '?');
        address.protocol = std::string(startURL, endProtocol);
        address.address = std::string(addressStart, addressEnd);
        address.port = std::string(portStart, addressEnd);
        address.path = std::string(addressEnd, pathEnd);
        address.options = std::string(pathEnd, endURL);
        address.original = std::move(URL);
    }
}

urlParser::URLParser::URLParser(std::string &URL, std::string &feedfile)
{
    if(!URL.empty() && !feedfile.empty())
        throw feedreaderException::URLParsing("Cannot submit url and feedfile at same time");
    if(!feedfile.empty())
        mCallback = std::make_unique<urlParser::FileURLReader>(feedfile);
    else
        mCallback = std::make_unique<urlParser::RawURLReader>(URL);
}

urlParser::FileURLReader::FileURLReader(std::string file)
{
    std::ifstream feedfile;
    try {
        feedfile.open(file);
        std::stringstream buffer;
        buffer << feedfile.rdbuf();
        std::string contents(buffer.str());
        std::string::iterator actPos = contents.begin();
        std::string::iterator nextPos;
        std::string::iterator endPos = contents.end();
        while(nextPos != endPos)
        {

            nextPos = std::find(actPos, endPos, '\n');
            if(nextPos == endPos)
                break;
            std::string temp(actPos, nextPos);
            actPos = nextPos + 1;
            if(temp.size() == 0 || temp.front() == '#')
                continue;
            if(std::regex_match(temp, txt_regex))
                mURLs.push_back(std::move(temp));
            else
                throw feedreaderException::URLParsing("Submitted URL %s is not valid", temp.c_str());
        }
        mURLiterator = mURLs.begin();
    }
    catch (const std::ifstream::failure& e)
    {
        throw feedreaderException::URLParsing("Cannot open file: %s ", file);
    }

}

bool urlParser::FileURLReader::next(struct URLAddress &address)
{
    if(mURLiterator == mURLs.end())
        return false;
    parseURL(*mURLiterator, address);
    mURLiterator++;
    return true;
}

bool urlParser::RawURLReader::next(URLAddress &address)
{
    if(mURl.empty())
        return false;
    parseURL(mURl, address);
    return true;
}
