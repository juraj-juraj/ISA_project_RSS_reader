#include <fstream>
#include <sstream>
#include <algorithm>

#include <iostream>

#include "urlparser.h"
#include "utils/exceptions.h"

// TODO poriesit vidirtelnost do vonka
// zaistit aby pred .cz a podobne
std::regex urlParser::txt_regex(R"(^((http)|(https))(\:\/\/)((www\.)?[a-zA-Z0-9\.\-\_]+[a-z]{1,18})(\:\d+)?(\/[^\ \t\?]*)?(\?\S+)?$)");

void urlParser::parseURL(const std::string&  URL, struct URLAddress& address)
{
    std::string::const_iterator startURL = URL.begin();
    std::string::const_iterator endURL = URL.end();
    std::string::const_iterator endProtocol = std::find(startURL, endURL, ':');
    std::string::const_iterator addressStart = endProtocol + 3; //skip ://
    std::string::const_iterator addressEnd = std::find(addressStart, endURL, '/');
    std::string::const_iterator optionsStart = std::find(addressStart, endURL, '?');
    if(optionsStart < addressEnd)
        addressEnd = optionsStart;
    std::string::const_iterator portStart = std::find(addressStart, addressEnd, ':');

    address.port = std::string((portStart!=addressEnd) ? portStart+1 : portStart, addressEnd);
    address.path = std::string(addressEnd, optionsStart);
    if(address.path.empty())
        address.path = "/";
    if(portStart != addressEnd)
        addressEnd = std::find(addressStart, endURL, ':');
    address.address = std::string(addressStart, addressEnd);
    address.options = std::string(optionsStart, endURL);
    address.protocol = std::string(startURL, endProtocol);
    address.original = std::move(URL);
}

urlParser::URLParser::URLParser(std::string URL, std::string feedfile, std::shared_ptr<Utils::logger> logger)
{
    mLogger = logger;
    if(!URL.empty() && !feedfile.empty())
        throw feedreaderException::URLParsing("Cannot submit url and feedfile at same time");
    else if(URL.empty() && feedfile.empty())
        throw feedreaderException::URLParsing("You have to submit feedfile or URL");
    if(!feedfile.empty())
        mCallback = std::make_unique<urlParser::FileURLReader>(feedfile, mLogger);
    else
        mCallback = std::make_unique<urlParser::RawURLReader>(URL, mLogger);
}

urlParser::FileURLReader::FileURLReader(std::string& file, std::shared_ptr<Utils::logger> logger)
{
    mLogger = logger;
    std::ifstream feedfile;
    try {
        feedfile.open(file);
        if(feedfile.fail())
            throw feedreaderException::URLParsing("Cannot open file %s", file.c_str());
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
                mLogger->errWrite("Error: Submitted URL %s is not valid", temp.c_str());
        }
        mURLiterator = mURLs.begin();
    }
    catch (const std::ifstream::failure& e)
    {
        throw feedreaderException::URLParsing("Feedfile '%s' is damaged", file);
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

urlParser::RawURLReader::RawURLReader(std::string &url, std::shared_ptr<Utils::logger> logger)
{
    if(!std::regex_match(url, txt_regex))
        throw feedreaderException::URLParsing("Submitted URL %s is not valid", url.c_str());
    mURl = url;
    mLogger = logger;
}

bool urlParser::RawURLReader::next(URLAddress &address)
{
    if(mURl.empty())
        return false;
    parseURL(mURl, address);
    mURl.clear();
    return true;
}
