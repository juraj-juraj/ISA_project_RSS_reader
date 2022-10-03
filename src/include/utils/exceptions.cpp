#include "exceptions.h"



const char *feedreaderException::baseException::what() const noexcept
{
    return mMsg.c_str();
}
