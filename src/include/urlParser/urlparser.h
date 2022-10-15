#pragma once

#include <string>
#include <vector>
#include <regex>

#include "utils/logger.h"

namespace urlParser
{

extern std::regex txt_regex;

struct URLAddress{
    std::string protocol;
    std::string address;
    std::string path;
    std::string port;
    std::string options;
    std::string original;
};

class URLReaderI
{
public:
    virtual ~URLReaderI() = default;
    virtual bool next(struct URLAddress &address) = 0;
};

class FileURLReader : public URLReaderI
{
public:
    FileURLReader(std::string& file, std::shared_ptr<Utils::logger> logger);

    [[nodiscard]] bool next(struct URLAddress &address) override;

private:
    std::vector<std::string> mURLs;
    std::vector<std::string>::iterator mURLiterator;
    std::shared_ptr<Utils::logger> mLogger;
};

class RawURLReader : public URLReaderI
{
public:
    RawURLReader(std::string& url, std::shared_ptr<Utils::logger> logger);

    [[nodiscard]] bool next(struct URLAddress &address) override;
private:
    std::string mURl;
    std::shared_ptr<Utils::logger> mLogger;
};

class URLParser
{
public:
    URLParser(std::string URL, std::string feedfile, std::shared_ptr<Utils::logger> logger);

    [[nodiscard]] bool next(struct URLAddress &address)
    {
        return mCallback->next(address);
    }

private:
    URLParser();
    std::unique_ptr<URLReaderI> mCallback;
    std::shared_ptr<Utils::logger> mLogger;
};

void parseURL(const std::string&  URL, struct URLAddress& address);

}
