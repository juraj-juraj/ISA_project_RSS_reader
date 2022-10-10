#pragma once

#include <string>
#include <vector>
#include <regex>

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
    FileURLReader(std::string file);

    [[nodiscard]] bool next(struct URLAddress &address) override;

private:
    std::vector<std::string> mURLs;
    std::vector<std::string>::iterator mURLiterator;
};

class RawURLReader : public URLReaderI
{
public:
    RawURLReader(std::string url): mURl(std::move(url)) {};

    [[nodiscard]] bool next(struct URLAddress &address) override;
private:
    std::string mURl;
};

class URLParser
{
public:
    URLParser(std::string& URL, std::string& feedfile);

    [[nodiscard]] bool next(struct URLAddress &address)
    {
        return mCallback->next(address);
    }

private:
    std::unique_ptr<URLReaderI> mCallback;
};

}
