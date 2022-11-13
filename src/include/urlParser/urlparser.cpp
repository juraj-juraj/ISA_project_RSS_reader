#include <fstream>
#include <sstream>
#include <algorithm>

#include <iostream>

#include "urlparser.h"
#include "utils/exceptions.h"


std::regex urlConstants::txt_regex(R"(^(https?:\/\/)?([a-zA-Z0-9\.\-\_\#\%\$\@\!]+)(\:\d+)?(\/[^\ \t\?]*)?(\?\S*)?$)");

void urlParser::parseURL(const std::string&  URL, struct URLAddress& address)
{
    std::smatch urlMatch;
    if(std::regex_search(URL, urlMatch, urlConstants::txt_regex))
    {
        for(size_t i = 0; i < urlMatch.size(); ++i)
        {
            urlParser::URLPart part{i};
            std::string temp(urlMatch[i].first, urlMatch[i].second);
            switch(part)
            {
                case urlParser::URLPart::whole:
                    address.original = URL;
                    break;
                case URLPart::protocol:
                    if(temp.empty())
                        address.protocol = urlConstants::https;
                    else
                        address.protocol = std::move(temp);
                    break;
                case URLPart::domain:
                    address.address = std::move(temp);
                    break;
                case URLPart::port:
                    address.port = std::move(temp);
                    break;
                case URLPart::path:
                    if(temp.empty())
                        address.path = "/";
                    else
                        address.path = std::move(temp);
                    break;
                case URLPart::options:
                    address.options = std::move(temp);
                    break;
            }

        }
    }
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
            if(std::regex_match(temp, urlConstants::txt_regex))
                mURLs.push_back(std::move(temp));
            else
                mLogger->errWrite("Error: Submitted URL %s is not valid\n", temp.c_str());
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
    if(!std::regex_match(url, urlConstants::txt_regex))
        throw feedreaderException::URLParsing("Submitted URL %s is not valid\n", url.c_str());
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
