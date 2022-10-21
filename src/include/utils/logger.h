#pragma once

#include <string>
#include "utils.h"

namespace Utils
{
class logger
{
public:
    logger(std::ostream& stream, std::ostream& errStream) : mLogStream(stream), mErrStream(errStream) {};

    template <typename... Args>
    void write(const std::string& format, Args... args)
    {
        write(Utils::string_format(format, args...));
    }
    void write(const std::string &msg)
    {
        mLogStream << msg;
    }

    template <typename... Args>
    void errWrite(const std::string& format, Args... args)
    {
        errWrite(Utils::string_format(format, args...));
    }
    void errWrite(const std::string& msg)
    {
        mErrStream << msg;
    }

private:
    std::ostream& mLogStream;
    std::ostream& mErrStream;
};

}

