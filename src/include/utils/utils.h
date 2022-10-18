#pragma once

#include <functional>
#include <memory>
#include <string>
#include <stdexcept>

namespace Utils
{

// function taken from https://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
template<typename ... Args>
std::string string_format(const std::string& format, Args ... args)
{
    int size_s = std::snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
    if( size_s <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
    auto size = static_cast<size_t>( size_s );
    std::unique_ptr<char[]> buf( new char[ size ] );
    std::snprintf( buf.get(), size, format.c_str(), args ... );
    return std::string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
}

/**
 * @brief splitDelim
 * splits exactly once string in delimiter
 * @param source
 * @param delimiter
 * @return pair with split strings
 */
std::pair<std::string, std::string> splitDelim(const std::string& source,const char delimiter);

void normalizeInPlace(std::string &source);

std::string& lTrim(std::string& source, std::function<bool(char)> predicat);

std::string& rTrim(std::string& source, std::function<bool(char)> predicat);

std::string& lrTrim(std::string& source, std::function<bool(char)> predicat);

}
