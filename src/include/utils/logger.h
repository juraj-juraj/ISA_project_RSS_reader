#pragma once

#include <string>

namespace Utils
{
template<typename T>
class logger
{
public:
    logger(T& stream) : mLogStream(stream) {};

    void write(const std::string &msg)
    {
        mLogStream << msg;
    }

private:
    T& mLogStream;
};

}

