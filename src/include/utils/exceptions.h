#pragma once

#include <exception>

#include "utils/utils.h"

namespace feedreaderException
{

class baseException : public std::exception
{
public:
  explicit baseException(std::string msg) : mMsg(std::move(msg)){};

  template <typename... Args>
  baseException(const std::string& format, Args... args) :
    mMsg((format.empty()) ? "" : Utils::string_format(format, args...)){};

  const char* what() const noexcept override;

private:
  std::string mMsg;
};

class argumentParsing : public baseException
{
    using baseException::baseException;
};

class URLParsing : public baseException
{
    using baseException::baseException;
};

class downloader : public baseException
{
    using baseException::baseException;
};

}

